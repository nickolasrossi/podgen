#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <optional>
#include <functional>

#include <capnp/schema-parser.h>

#include "reserved_words.hpp"

// from compiler/capnpc-c++.c++
constexpr uint64_t NAMESPACE_ANNOTATION_ID = 0xb9c6f99ebf805f2cull;
constexpr uint64_t NAME_ANNOTATION_ID = 0xf264a779fef191ceull;

// from messaging/capnp/pod.capnp
constexpr uint64_t EXTTYPE_ANNOTATION_ID = 0x97eb12f93bdd7cd3ull;
constexpr uint64_t CONVERTER_ANNOTATION_ID = 0xa5a4d17a95451341ull;
constexpr uint64_t CONTAINER_ANNOTATION_ID = 0xaf26f446007ee38aull;
constexpr uint64_t INHERIT_ANNOTATION_ID = 0x8955e2fd9635a433ull;
constexpr uint64_t DEFAULT_VALUE_ANNOTATION_ID = 0xac6b15d7e51cadd7ull;
constexpr uint64_t PACK_ANNOTATION_ID = 0xea261d3e58c48d13ull;
constexpr uint64_t NOHASH_ANNOTATION_ID = 0xa13d849658a9f9acull;
constexpr uint64_t IGNORE_ANNOTATION_ID = 0xc8e0b11182f5194aull;

struct TypeHasher {
    std::size_t operator()(const ::capnp::Type& t) const {
        return t.hashCode();
    }
};

struct SchemaHasher {
    std::size_t operator()(const ::capnp::Schema& s) const {
        return s.hashCode();
    }
};

using ImportToNamespace = std::unordered_map<std::string, std::string>;
using CapnpToExternalType = std::unordered_map<std::string, std::string>;

using Unions = std::unordered_set<std::string>;
using InheritedFields = std::unordered_map<std::string, ::capnp::StructSchema::Field>;

using CapnpTypeSet = std::unordered_set<::capnp::Type, TypeHasher>;

struct ExternalTypes {
    CapnpToExternalType typeMap;
    std::unordered_set<std::string> podHeaders;
};

struct SchemaInfo {
    ::capnp::ParsedSchema schema;
    std::vector<::capnp::ParsedSchema> imports;
    ImportToNamespace importNamespaces;
    ImportToNamespace importAliases;
    std::unordered_map<std::string, ::capnp::StructSchema> internalTypesByName;
    std::unordered_map<uint64_t, ::capnp::StructSchema> internalTypesById;
    std::unordered_map<uint64_t, uint64_t> schemaParentOf;
    Unions unions;
    CapnpToExternalType externalTypes;
    std::unordered_set<std::string> podHeaders;
};


class last_char_outbuf : public std::streambuf {
public:
    last_char_outbuf(std::streambuf* buf) : buf(buf), last(traits_type::eof()) {}

    char get_last_char() const { return last; }

    std::streamsize xsputn(const char_type* s, std::streamsize n) override {
        if (n > 0) {
            last = s[n - 1];
        }
        return buf->sputn(s, n);
    };

    virtual int_type overflow(int_type c) override {
        buf->sputc(c);
        last = c;
        return c;
    }

private:
    std::streambuf* buf;
    char last;
};


template <typename M>
std::optional<typename M::mapped_type> mapFind(const M& map, const typename M::key_type& key) {
    auto it = map.find(key);
    if (it != map.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}


/// Write bits of code-related text to an output stream.
class PodGenStream {
public:
    using OnWrite = std::function<void(PodGenStream&)>;

    PodGenStream(std::ostream& os, std::string ns, SchemaInfo info)
        : namesp(std::move(ns)), info(std::move(info)), os(os) {}

    std::string findNamespace(const std::string& import) const {
        auto it = info.importNamespaces.find(import);
        return it != info.importNamespaces.end() ? it->second : "";
    }

    std::optional<::capnp::StructSchema> findInternalType(const std::string& type) const {
        return mapFind(info.internalTypesByName, type);
    }

    std::optional<::capnp::StructSchema> findInternalType(uint64_t id) const {
        return mapFind(info.internalTypesById, id);
    }

    std::optional<::capnp::Schema> findParentOf(::capnp::Schema s) const {
        auto parentId = mapFind(info.schemaParentOf, s.getProto().getId());
        if (parentId) {
            return findInternalType(*parentId);
        } else {
            return {};
        }
    };

    bool isTypename(const ::capnp::Schema schema, bool asScope) const {
        for (std::optional<::capnp::Schema> s = asScope ? schema : findParentOf(schema);
            s.has_value(); s = findParentOf(*s)) {
            if (s->getProto().getIsGeneric() && s->getProto().hasParameters()) {
                auto bal = schema.getBrandArgumentsAtScope(s->getProto().getId());
                if (bal.size() == 0) {
                    return true;
                } else {
                    for (auto ba : bal) {
                        if (ba.isAnyPointer()) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool hasUnion(const std::string& u) const {
        return info.unions.count(u) > 0;
    }

    bool hasExternalType(const std::string& capnpType) const {
        return info.externalTypes.count(capnpType) > 0;
    }


    PodGenStream& startNamespace(const std::string& s) {
        scope_.push_back(s);
        indent().str("namespace ").str(s).chr(' ').openBrace();
        indent_--;
        return *this;
    }

    PodGenStream& endNamespace() {
        if (braces_ != scope_.size()) {
            throw std::runtime_error("namespace ended with brace still open");
        }

        scope_.pop_back();
        return closeBrace();
    }

    PodGenStream& startStruct(const std::string& s, const CapnpTypeSet& inherits = {}) {
        structs_++;
        scope_.push_back(s);
        indent().str("struct ").str(s);

        if (!inherits.empty()) {
            str(" : ");
            bool first = true;
            for (auto& inherit : inherits) {
                if (!first) {
                    str(", ");
                }
                podType(inherit);
                first = false;
            }
        }

        return chr(' ').openBrace();
    }

    PodGenStream& endStruct(const char* var = nullptr) {
        if (braces_ != scope_.size()) {
            throw std::runtime_error("namespace ended with brace still open");
        }
        if (structs_ == 0) {
            throw std::runtime_error("not in a struct");
        }

        structs_--;
        scope_.pop_back();
        if (var) {
            return closeBrace([var](PodGenStream& s) { s.chr(' ').token(var).chr(';'); });
        } else {
            return closeBrace(";");
        }
    }

    void setField(::capnp::StructSchema::Field f) { fieldContext_ = f; }
    void clearField() { fieldContext_.reset(); }
    bool hasField() { return fieldContext_.has_value(); }
    std::optional<::capnp::StructSchema::Field> getField() { return fieldContext_; }
    std::unordered_map<uint64_t, std::vector<std::string>> fieldTypeParameters();

    PodGenStream& podType(::capnp::StructSchema::Field);
    PodGenStream& podType(::capnp::StructSchema::Field, ::capnp::schema::Value::Reader containerAnnotation);
    PodGenStream& podType(::capnp::Type, bool isFunctionArg = false, bool asScope = false);
    PodGenStream& podType(::capnp::Schema, bool isFunctionArg = false, bool asScope = false);
    PodGenStream& capnpType(::capnp::Type, bool isFunctionArg = false, bool asScope = false);
    PodGenStream& capnpType(::capnp::Schema, bool isFunctionArg, bool asScope = false);

    PodGenStream& nl() {
        os << std::endl;
        return *this;
    }

    PodGenStream& sp(uint16_t n = 1) {
        while (n-- > 0) {
            os.put(' ');
        }
        return *this;
    }

    PodGenStream& tab(uint16_t n = 1) {
        return sp(n * 4);
    }

    PodGenStream& chr(char c) {
        os.put(c);
        return *this;
    }

    template <typename T>
    PodGenStream& str(T&& t) {
        os << t;
        return *this;
    }

    PodGenStream& token(const char* name) {
        os << name;
        if (reservedWords.count(name) > 0) {
            os << '_';
        }
        return *this;
    }

    PodGenStream& token(const std::string& s) {
        return token(s.c_str());
    }

    PodGenStream& cap(const char* name) {
        if (name[0]) {
            os.put(toupper(name[0]));
            os << &name[1];
        }
        return *this;
    }

    PodGenStream& cap(const std::string& s) {
        return cap(s.c_str());
    }

    PodGenStream& ns() {
        if (!namesp.empty()) {
            os << namesp;
        }
        return *this;
    }

    PodGenStream& indent() {
        return tab(indent_);
    }

    PodGenStream& openBrace() {
        braces_++;
        indent_++;
        return chr('{').nl();
    }

    PodGenStream& closeBrace(OnWrite write = nullptr) {
        braces_--;
        if (indent_ > 0) {
            indent_--;
        }

        indent();
        chr('}');
        if (write) {
            write(*this);
        }
        return nl();
    }

    PodGenStream& closeBrace(const std::string& post) {
        return closeBrace([&post](PodGenStream& gs) {
            gs.str(post);
        });
    }

    PodGenStream& closeBraceSemi() {
        return closeBrace(";");
    }

    uint16_t getIndentLevel() const { return indent_; }
    bool inStruct() const { return structs_ > 0; }
    const std::vector<std::string>& scope() const { return scope_; }

    template <typename T>
    friend PodGenStream& operator<<(PodGenStream& gs, T&& t) {
        gs.os << t;
        return gs;
    }

    const std::string namesp;
    const SchemaInfo info;

private:
    std::ostream& os;
    last_char_outbuf* buf;

    std::vector<std::string> scope_;
    uint16_t structs_ = 0;
    uint16_t braces_ = 0;
    uint16_t indent_ = 0;

    std::optional<::capnp::StructSchema::Field> fieldContext_;
};

std::vector<std::pair<std::string, std::string>> getImportsFromCapnp(const std::string& file);
std::filesystem::path capnpToPodImport(const std::filesystem::path& path);
std::filesystem::path capnpToConvertImport(const std::filesystem::path& path);
std::string getNamespace(const ::capnp::Schema& schema);

using FromStructGenerator = std::function<void(PodGenStream&, ::capnp::StructSchema, ::capnp::Schema parent)>;
using FromEnumGenerator = std::function<void(PodGenStream&, ::capnp::EnumSchema, ::capnp::Schema parent)>;

size_t generateFromSchema(PodGenStream& gs, ::capnp::ParsedSchema schema,
                         const FromStructGenerator& sgen, const FromEnumGenerator& egen = FromEnumGenerator());
void generateFromStruct(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent,
    const FromStructGenerator& sgen, std::unordered_set<::capnp::Schema, SchemaHasher>& processed);

void generateField(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateFieldConstant(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateOptional(PodGenStream& gs, ::capnp::StructSchema schema, bool isField = false);
void generateOptionalField(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateUnion(PodGenStream& gs, ::capnp::StructSchema schema, bool isField = false);
void generateUnionField(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generatePlainField(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateGroup(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateGroupInUnion(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateDefaultValue(PodGenStream& gs, ::capnp::StructSchema::Field field, ::capnp::schema::Value::Reader value);
void generateStruct(PodGenStream& gs, ::capnp::ParsedSchema schema, ::capnp::Schema parent);
void generateEnum(PodGenStream& gs, ::capnp::EnumSchema schema, ::capnp::Schema parent);
void generatePods(PodGenStream& gs, ::capnp::ParsedSchema schema);

void generateAssignFromField(PodGenStream& gs, ::capnp::StructSchema::Field field);
void generateConvertFromGroupFunction(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent);
void generateConvertFromStructFunction(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent);
void generateConvertToStructFunction(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent);
void _generateStructConverters(PodGenStream& gs, ::capnp::StructSchema schema);

void generateFile(std::istream& is, std::ostream& os, const SchemaInfo& info,
                  const std::filesystem::path& capnpFile);

Unions findUnionFields(const ::capnp::ParsedSchema& schema);
ExternalTypes findExternalTypes(const ::capnp::ParsedSchema& schema);

