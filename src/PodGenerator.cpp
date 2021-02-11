#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <list>

#include <kj/filesystem.h>
#include <capnp/dynamic.h>

#include "pod.capnp.h"
#include "schema.hpp"
#include "PodGenerator.hpp"

std::unordered_map<::capnp::schema::Type::Which, std::string> field_types {
    { ::capnp::schema::Type::Which::VOID, "std::monostate" },
    { ::capnp::schema::Type::Which::BOOL, "bool" },
    { ::capnp::schema::Type::Which::INT8, "int8_t" },
    { ::capnp::schema::Type::Which::INT16, "int16_t" },
    { ::capnp::schema::Type::Which::INT32, "int32_t" },
    { ::capnp::schema::Type::Which::INT64, "int64_t" },
    { ::capnp::schema::Type::Which::UINT8, "uint8_t" },
    { ::capnp::schema::Type::Which::UINT16, "uint16_t" },
    { ::capnp::schema::Type::Which::UINT32, "uint32_t" },
    { ::capnp::schema::Type::Which::UINT64, "uint64_t" },
    { ::capnp::schema::Type::Which::FLOAT32, "float" },
    { ::capnp::schema::Type::Which::FLOAT64, "double" },
    { ::capnp::schema::Type::Which::TEXT, "std::string" },
    { ::capnp::schema::Type::Which::DATA, "podgen::Data" }
};

std::unordered_map<::capnp::schema::Type::Which, std::string> capnp_type_enum {
    { ::capnp::schema::Type::Which::VOID, "VOID" },
    { ::capnp::schema::Type::Which::BOOL, "BOOL" },
    { ::capnp::schema::Type::Which::INT8, "INT8" },
    { ::capnp::schema::Type::Which::INT16, "INT16" },
    { ::capnp::schema::Type::Which::INT32, "INT32" },
    { ::capnp::schema::Type::Which::INT64, "INT64" },
    { ::capnp::schema::Type::Which::UINT8, "UINT8" },
    { ::capnp::schema::Type::Which::UINT16, "UINT16" },
    { ::capnp::schema::Type::Which::UINT32, "UINT32" },
    { ::capnp::schema::Type::Which::UINT64, "UINT64" },
    { ::capnp::schema::Type::Which::FLOAT32, "FLOAT32" },
    { ::capnp::schema::Type::Which::FLOAT64, "FLOAT64" },
    { ::capnp::schema::Type::Which::TEXT, "TEXT" },
    { ::capnp::schema::Type::Which::DATA, "DATA" },
    { ::capnp::schema::Type::Which::STRUCT, "STRUCT" },
    { ::capnp::schema::Type::Which::ENUM, "ENUM" },
    { ::capnp::schema::Type::Which::ANY_POINTER, "ANY_POINTER" },
    { ::capnp::schema::Type::Which::LIST, "LIST" }
};

std::unordered_map<::capnp::schema::Type::Which, std::string> capnp_kind_enum {
    { ::capnp::schema::Type::Which::VOID, "VOID" },
    { ::capnp::schema::Type::Which::BOOL, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::INT8, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::INT16, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::INT32, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::INT64, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::UINT8, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::UINT16, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::UINT32, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::UINT64, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::FLOAT32, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::FLOAT64, "PRIMITIVE" },
    { ::capnp::schema::Type::Which::TEXT, "BLOB" },
    { ::capnp::schema::Type::Which::DATA, "BLOB" },
    { ::capnp::schema::Type::Which::STRUCT, "STRUCT" },
    { ::capnp::schema::Type::Which::ENUM, "ENUM" },
    { ::capnp::schema::Type::Which::ANY_POINTER, "OTHER" },
    { ::capnp::schema::Type::Which::LIST, "OTHER" }
};

std::unordered_map<::capnp::schema::Type::Which, uint8_t> pack_sizes {
    { ::capnp::schema::Type::Which::VOID, sizeof(std::monostate) },
    { ::capnp::schema::Type::Which::BOOL, sizeof(bool) },
    { ::capnp::schema::Type::Which::INT8, sizeof(int8_t) },
    { ::capnp::schema::Type::Which::INT16, sizeof(int16_t) },
    { ::capnp::schema::Type::Which::INT32, sizeof(int32_t) },
    { ::capnp::schema::Type::Which::UINT8, sizeof(uint8_t) },
    { ::capnp::schema::Type::Which::UINT16, sizeof(uint16_t) },
    { ::capnp::schema::Type::Which::UINT32, sizeof(uint32_t) },
    { ::capnp::schema::Type::Which::FLOAT32, sizeof(float) }
};


/** Type parsing and writing **/

/// Scan the capnp file for all import statements, since they aren't available from the schema
std::vector<std::pair<std::string, std::string>> getImportsFromCapnp(const std::string& file) {
    std::vector<std::pair<std::string, std::string>> imports;

    std::regex pattern("import \"(.*?)\"");
    std::regex using_pattern(R"(using\s+(.*?)\s*=\s*import)");
    std::string line;
    std::smatch match;

    std::ifstream is(file);
    while (std::getline(is, line)) {
        auto pos = line.find('#');
        if (pos != std::string::npos) {
            line.resize(pos);
        }

        if (std::regex_search(line, match, pattern) && match.size() == 2) {
            auto import = match[1].str();
            if (!import.empty()) {
                std::string alias;
                if (std::regex_search(line, match, using_pattern) && match.size() == 2) {
                    alias = match[1].str();
                }

                imports.emplace_back(alias, import);
            }
        }
    }

    return imports;
}

std::filesystem::path capnpToPodImport(const std::filesystem::path& path) {
    auto p = path;
    p.replace_extension(".pod.hpp");
    return p;
}

std::filesystem::path capnpToConvertImport(const std::filesystem::path& path) {
    auto p = path;
    p.replace_extension(".convert.hpp");
    return p;
}

/// Populate an annotated container type with template arguments.
std::string fillTemplateArgs(const std::string& input, const std::unordered_map<std::string, std::string>& types) {
    std::ostringstream ss;

    size_t pos = 0;
    for (size_t n = 0; n < input.size(); n++) {
        char c = input[n];

        if (c == ',' || c == '<' || c == '>') {
            if (n > pos) {
                auto part = input.substr(pos, n - pos);
                auto it = types.find(part);
                if (it != types.end()) {
                    ss << it->second;
                } else {
                    for (size_t i = pos; i < n; i++) {
                        ss << input[i];
                    }
                }
            }

            ss << c;
            pos = n + 1;
        }
    }

    for (; pos < input.size(); pos++) {
        ss << input[pos];
    }

    return ss.str();
}

/// Whether the schema has the ignore annotation
template <typename E>
bool ignore(const E& element) {
    return hasAnnotation(element, IGNORE_ANNOTATION_ID);
}

/// Return the C++ namespace from the capnp annotation.
std::string getNamespace(const ::capnp::Schema& schema) {
    auto anno = getAnnotation(schema, NAMESPACE_ANNOTATION_ID);
    return anno ? anno->getText() : "";
}

/// Convert the capnp namespace to an appropriate pod namespace.
/// if capnp namespace ends with "::capnp", it is stripped. Otherwise, "::pod" as added.
std::string podFromCapnpNamespace(const std::string& ns) {
    static const std::string capnp_ns = "::capnp";

    if (ns.empty()) {
        return "";
    } else if (ns.size() >= capnp_ns.size() && ns.substr(ns.size() - capnp_ns.size()) == capnp_ns) {
        return ns.substr(0, ns.size() - capnp_ns.size());
    } else {
        return ns + "::pod";
    }
}

/**
 * Determine if type 1 and type 2 are equivalent:
 * - if primitive types, that they are the same
 * - if struct types, that they have the same union and non-union fields (same name and equivalent type of each field)
 * - if list types, that they share equivalent element types
 */
bool equivalentTypes(::capnp::Type t1, ::capnp::Type t2) {
    if (t1 == t2) {
        return true;
    }

    if (t1.which() != t2.which()) {
        return false;
    }

    switch (t1.which()) {
        case ::capnp::schema::Type::Which::ENUM:
            return false;

        case ::capnp::schema::Type::Which::LIST:
            return equivalentTypes(t1.asList().getElementType(), t2.asList().getElementType());

        case ::capnp::schema::Type::Which::STRUCT: {
            auto fs1 = t1.asStruct().getNonUnionFields();
            auto fs2 = t2.asStruct().getNonUnionFields();

            if (fs1.size() != fs2.size()) {
                return false;
            }

            auto ufs1 = t1.asStruct().getUnionFields();
            auto ufs2 = t2.asStruct().getUnionFields();

            if (ufs1.size() != ufs2.size()) {
                return false;
            }

            auto hasField = [](const ::capnp::StructSchema::FieldSubset& fields, ::capnp::StructSchema::Field field) -> bool {
                for (auto f : fields) {
                    if (f.getProto().getName() == field.getProto().getName()
                        && equivalentTypes(f.getType(), field.getType())) {
                        return true;
                    }
                }

                return false;
            };

            for (auto f : fs1) {
                if (!hasField(fs2, f)) {
                    return false;
                }
            }

            for (auto uf : ufs1) {
                if (!hasField(ufs2, uf)) {
                    return false;
                }
            }

            return true;
        }

        default:
            return false;
    }
}

/**
 * Collect all substituted but unbound parameters from the field declaration.
 * This is to properly render a field type such as A<X> below:
 *
 * template <typename T>
 * struct A {...}
 *
 * template <typename X>
 * struct B {
 *     A<X> a;
 * }
 *
 * capnproto is supposed to provide this information by way of
 * field.getType().asStruct().getBrandArgumentsAtScope(), but parameter substitutions
 * never appear there, only fully bound types do.
 */
std::unordered_map<uint64_t, std::vector<std::string>> PodGenStream::fieldTypeParameters() {
    std::unordered_map<uint64_t, std::vector<std::string>> paramsByScope;

    if (fieldContext_) {
        auto fp = fieldContext_->getProto();
        if (fp.isSlot() && fp.getSlot().getType().isStruct()) {
            auto scopes = fp.getSlot().getType().getStruct().getBrand().getScopes();
            for (auto scope : scopes) {
                std::vector<std::string> params;
                for (::capnp::schema::Brand::Binding::Reader b : scope.getBind()) {
                    if (b.getType().isAnyPointer()) {
                        auto p = b.getType().getAnyPointer().getParameter();
                        ::capnp::Schema s = info.internalTypesById.at(p.getScopeId());
                        params.emplace_back(s.getProto().getParameters()[p.getParameterIndex()].getName().cStr());
                    }
                }

                if (!params.empty()) {
                    paramsByScope.emplace(scope.getScopeId(), std::move(params));
                }
            }
        }
    }

    return paramsByScope;
}

PodGenStream& PodGenStream::podType(::capnp::StructSchema::Field field) {
    auto anno = getAnnotation(field, CONVERTER_ANNOTATION_ID);
    if (anno) {
        auto converter = anno->getStruct().getAs<Converter>();
        if (converter.getType().size() > 0) {
            return str(converter.getType().cStr());
        }
    }

    anno = getAnnotation(field, CONTAINER_ANNOTATION_ID);
    if (anno) {
        return podType(field, *anno);
    }

    // if the field type is an unbound generic parameter, emit it and return
    if (field.getProto().isSlot() && field.getProto().getSlot().getType().isAnyPointer()) {
        ::capnp::schema::Type::AnyPointer::Reader ap = field.getProto().getSlot().getType().getAnyPointer();
        if (ap.isParameter()) {
            ::capnp::StructSchema gschema = info.internalTypesById.at(ap.getParameter().getScopeId());
            if (gschema.getProto().hasParameters()) {
                auto p = gschema.getProto().getParameters()[ap.getParameter().getParameterIndex()];
                return str(p.getName().cStr());
            }
        }
    }

    // the field context tells the type outputter where to look for bound generic parameters
    setField(field);
    podType(field.getType(), true);
    clearField();

    return *this;
}

PodGenStream& PodGenStream::podType(::capnp::StructSchema::Field field, ::capnp::schema::Value::Reader containerAnnotation) {
    // handle container annotation

    if (field.getType().which() != ::capnp::schema::Type::Which::LIST) {
        throw std::runtime_error(std::string("container annotation only valid for List fields (") + field.getProto().getName().cStr() + " is not)");
    }

    // write out container type, substituting pod conversions for T, K, V
    std::string type = containerAnnotation.getStruct().getAs<ExternalType>().getType();

    // use the innermost list element type
    ::capnp::Type T = field.getType().asList().getElementType();
    while (T.which() == ::capnp::schema::Type::Which::LIST) {
        T = T.asList().getElementType();
    }

    // map key and value are taken from the element type's first two fields
    kj::Maybe<::capnp::Type> K, V;
    if (T.isStruct() && T.asStruct().getNonUnionFields().size() >= 2) {
        K = T.asStruct().findFieldByName("key").map([](auto& o) { return o.getType(); });
        V = T.asStruct().findFieldByName("value").map([](auto& o) { return o.getType(); });
    }

    size_t pos = 0;
    for (size_t n = 0; n < type.size(); n++) {
        char c = type[n];

        if (c == ',' || c == '<' || c == '>' || c == ' ' || c == ':') {
            if (n > pos) {
                auto part = type.substr(pos, n - pos);
                if (part == "T") {
                    podType(T);
                } else if (part == "K") {
                    KJ_IF_MAYBE(k, K) {
                        podType(*k);
                    }
                } else if (part == "V") {
                    KJ_IF_MAYBE(v, V) {
                        podType(*v);
                    }
                } else {
                    for (size_t i = pos; i < n; i++) {
                        chr(type[i]);
                    }
                }
            }

            chr(c);
            pos = n + 1;
        }
    }

    for (; pos < type.size(); pos++) {
        chr(type[pos]);
    }

    return *this;
}

PodGenStream& PodGenStream::podType(::capnp::Type type, bool isFunctionArg, bool asScope) {
    auto it = field_types.find(type.which());
    if (it != field_types.end()) {
        return str(it->second);
    }

    switch (type.which()) {
        case ::capnp::schema::Type::Which::LIST: {
            str("std::vector<");
            podType(type.asList().getElementType(), isFunctionArg, asScope);
            chr('>');
            break;
        }

        case ::capnp::schema::Type::Which::ENUM:
            podType(type.asEnum(), isFunctionArg);
            break;

        case ::capnp::schema::Type::Which::STRUCT:
            podType(type.asStruct(), isFunctionArg, asScope);
            break;

        case ::capnp::schema::Type::Which::ANY_POINTER: {
            KJ_IF_MAYBE(bp, type.getBrandParameter()) {
                str(info.internalTypesById.at(bp->scopeId).getProto().getParameters()[bp->index].getName().cStr());
            } else {
                str("void*");
            }
            break;
        }

        default:
            break;
    }

    return *this;
}

PodGenStream& PodGenStream::podType(::capnp::Schema schema, bool isFunctionArg, bool asScope) {
    std::string capnpName = schema.getProto().getDisplayName();

    // if there is an external converter for this type, render the external type name instead
    auto it = info.externalTypes.find(capnpName);
    if (it != info.externalTypes.end()) {
        return str(it->second);
    }

    auto parts = parseCapnpTypeName(capnpName);
    auto partsOrig = parts;
    auto ns = podFromCapnpNamespace(findNamespace(parts[0]));

    if (ns.empty()) {
        throw std::runtime_error("namespace not found for import " + parts[0]);
    }

    parts[0] = ns;

    // all parts after the main type that start lowercase are groups or unions, which need a _ prefix
    for (size_t i = 2; i < parts.size(); i++) {
        if (islower(parts[i][0])) {
            parts[i] = "_" + parts[i];
        }
    }

    // build up corresponding capnp scope in order to look up parent schemas
    std::stringstream capnpScope;

    // don't write out the type parts matching the current scope
    size_t i = 0;
    for (; i < parts.size() - 1 && i < scope_.size() && parts[i] == scope_[i]; i++) {
        capnpScope << (i > 0 ? (i == 1 ? ":" : ".") : "") << partsOrig[i];
    }

    if (isFunctionArg && schema.getProto().getIsGeneric() && isTypename(schema, asScope)) {
        str("typename ");
    }

    if (i == 0) {
        str("::");
    }

    // collect all unbound parameters
    auto parameters = fieldTypeParameters();

    bool hadFirstTemplatePart = false;

    for (; i < parts.size(); i++) {
        capnpScope << (i > 0 ? (i == 1 ? ":" : ".") : "") << partsOrig[i];

        // since unions are represented as variants, they can't be part of the type name path into nested groups
        // this bit of trickery checks if the current type path identifies a union, and if so, skips it in the name
        if (i < parts.size() - 1) {
            auto cname = mergeCapnpTypeName(partsOrig, i + 1);
            if (hasUnion(cname)) {
                continue;
            }
        }

        // if this part of the path is a generic, add the template parameters.
        // this handles either fully bound parameters, or partial or non-bound parameters.
        auto parentSchema = findInternalType(capnpScope.str());
        if (parentSchema && parentSchema->getProto().hasParameters()) {
            if (hadFirstTemplatePart) {
                str("template ");
            }

            token(parts[i]);
            str("<");

            auto bal = schema.getBrandArgumentsAtScope(parentSchema->getProto().getId());
            auto params = mapFind(parameters, parentSchema->getProto().getId());

            if (!params) {
                std::vector<std::string> ps;

                for (auto p : parentSchema->getProto().getParameters()) {
                    ps.emplace_back(p.getName().cStr());
                }

                params.emplace(std::move(ps));
            }

            if (bal.size() > 0) {
                for (size_t i = 0; i < bal.size(); i++) {
                    if (i > 0) {
                        str(", ");
                    }

                    if (!bal[i].isAnyPointer()) {
                        podType(bal[i]);
                    } else if (params) {
                        str((*params)[i]);
                        hadFirstTemplatePart = true;
                    }
                }
            } else if (!params->empty()) {
                for (size_t i = 0; i < params->size(); i++) {
                    if (i > 0) {
                        str(", ");
                    }
                    str((*params)[i]);
                }
                hadFirstTemplatePart = true;
            }

            str(">");
        } else {
            token(parts[i]);
        }

        if (i < parts.size() - 1) {
            str("::");
        }
    }

    return *this;
}

PodGenStream& PodGenStream::capnpType(::capnp::Type type, bool isFunctionArg, bool asScope) {
    auto it = field_types.find(type.which());
    if (it != field_types.end()) {
        return str(it->second);
    }

    switch (type.which()) {
        case ::capnp::schema::Type::Which::LIST: {
            auto t = type.asList().getElementType();
            str("::capnp::List<");
            capnpType(t, isFunctionArg, asScope);
            str(", ::capnp::Kind::");
            str(capnp_kind_enum.at(t.which()));
            chr('>');
            break;
        }

        case ::capnp::schema::Type::Which::ENUM:
            capnpType(type.asEnum(), isFunctionArg);
            break;

        case ::capnp::schema::Type::Which::STRUCT:
            capnpType(type.asStruct(), isFunctionArg, asScope);
            break;

        case ::capnp::schema::Type::Which::ANY_POINTER: {
            KJ_IF_MAYBE(bp, type.getBrandParameter()) {
                str(info.internalTypesById.at(bp->scopeId).getProto().getParameters()[bp->index].getName().cStr());
            } else {
                str("void*");
            }
            break;
        }

        default:
            break;
    }

    return *this;
}

PodGenStream& PodGenStream::capnpType(::capnp::Schema schema, bool isFunctionArg, bool asScope) {
    auto parts = parseCapnpTypeName(schema.getProto().getDisplayName());
    auto partsOrig = parts;

    auto ns = findNamespace(parts[0]);
    if (ns.empty()) {
        throw std::runtime_error("namespace not found for import " + parts[0]);
    }

    parts[0] = ns;

    // capnp union and group type names are first-char capitalized
    for (size_t i = 2; i < parts.size(); i++) {
        parts[i][0] = toupper(parts[i][0]);
    }

    // build up corresponding capnp scope in order to look up parent schemas
    std::stringstream capnpScope;

    // don't write out the type parts matching the current scope
    size_t i = 0;
    for (; i < parts.size() - 1 && i < scope_.size() && parts[i] == scope_[i]; i++) {
        capnpScope << (i > 0 ? (i == 1 ? ":" : ".") : "") << partsOrig[i];
    }

    if (isFunctionArg && schema.getProto().getIsGeneric() && isTypename(schema, asScope)) {
        str("typename ");
    }

    if (i == 0) {
        str("::");
    }

    auto parameters = fieldTypeParameters();
    bool hadFirstTemplatePart = false;

    for (; i < parts.size(); i++) {
        capnpScope << (i > 0 ? (i == 1 ? ":" : ".") : "") << partsOrig[i];

        auto parentSchema = findInternalType(capnpScope.str());
        if (parentSchema && parentSchema->getProto().hasParameters()) {
            if (hadFirstTemplatePart) {
                str("template ");
            }

            token(parts[i]);
            str("<");

            auto bal = schema.getBrandArgumentsAtScope(parentSchema->getProto().getId());
            auto params = mapFind(parameters, parentSchema->getProto().getId());

            if (!params) {
                std::vector<std::string> ps;

                for (auto p : parentSchema->getProto().getParameters()) {
                    ps.emplace_back(p.getName().cStr());
                }

                params.emplace(std::move(ps));
            }

            if (bal.size() > 0) {
                for (size_t i = 0; i < bal.size(); i++) {
                    if (i > 0) {
                        str(", ");
                    }

                    if (!bal[i].isAnyPointer()) {
                        capnpType(bal[i]);
                    } else if (params) {
                        str("decltype(capnpTypeOf(std::declval<");
                        str((*params)[i]);
                        str(">()))");

                        hadFirstTemplatePart = true;
                    }
                }
            } else if (!params->empty()) {
                for (size_t i = 0; i < params->size(); i++) {
                    if (i > 0) {
                        str(", ");
                    }
                    str("decltype(capnpTypeOf(std::declval<");
                    str((*params)[i]);
                    str(">()))");

                    hadFirstTemplatePart = true;
                }
            }

            str(">");
        } else {
            token(parts[i]);
        }

        if (i < parts.size() - 1) {
            str("::");
        }
    }

    return *this;
}


/** Generation of types **/

void generateOptional(PodGenStream& gs, ::capnp::StructSchema schema, bool isField) {
    for (auto uf : schema.getUnionFields()) {
        if (isGroupField(uf)) {
            generateGroupInUnion(gs, uf);
        }
    }

    auto name = schema.getShortDisplayName().cStr();

    if (isField) {
        gs.indent().str("std::optional<");
    } else {
        gs.indent().str("using ").str(name).str(" = std::optional<");
    }

    gs.podType(*getOptionalField(schema));

    gs.str(">");

    if (!isField) {
        gs.str(";").nl();
    }
}

void generateOptionalField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto fname = field.getProto().getName().cStr();
    auto name = std::string("_") + fname;

    generateOptional(gs, field.getType().asStruct(), true);

    gs.str(" ").token(fname).chr(';').nl();
}

void generateUnion(PodGenStream& gs, ::capnp::StructSchema schema, bool isField) {
    if (isOptional(schema)) {
        generateOptional(gs, schema, isField);
        gs.nl();
        return;
    }

    auto fname = schema.getShortDisplayName().cStr();
    auto name = std::string(isField ? "_" : "") + fname;
    auto unionFields = schema.getUnionFields();

    for (auto uf : unionFields) {
        if (isGroupField(uf)) {
            generateGroupInUnion(gs, uf);
//        } else if (isUnionField(uf)) {
//            generateUnion(gs, uf.getType().asStruct(), true);
        }
    }

    gs.indent().str("using ").str(name).str(" = std::variant<");
    for (uint i = 0; i < unionFields.size(); i++) {
        auto uf = unionFields[i];
        if (i > 0) {
            gs << ", ";
        }
        gs.podType(uf);
    }
    gs.str(">;").nl();

    // enum to identify which union field is assigned
    std::string which = name;
    which += "_which";

    gs.indent().str("enum ").str(which).sp().openBrace();
    for (uint i = 0; i < unionFields.size(); i++) {
        std::string uname = fname;
        uname += '_';
        uname += unionFields[i].getProto().getName();

        gs.indent().token(uname);
        if (i < unionFields.size() - 1) {
            gs.chr(',');
        }

        gs.nl();
    }
    gs.closeBrace(";");

    gs.nl();
}

void generateUnionField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    if (isOptionalField(field)) {
        generateOptionalField(gs, field);
        return;
    }

    auto fname = field.getProto().getName().cStr();
    auto name = std::string("_") + fname;
    std::string which = name;
    which += "_which";

    generateUnion(gs, field.getType().asStruct(), true);

    gs.indent().str(name).str(" ").token(fname).chr(';').nl();
    gs.indent().str(which).str(" ").str(fname).str("_which() const { return static_cast<").str(which).str(">(").token(fname).str(".index()); }").nl();
}

void generateStructFields(PodGenStream& gs, ::capnp::StructSchema schema, const std::string& name,
        InheritedFields inheritedFields = {}) {
    auto fs = schema.getNonUnionFields();

    // if marked for structure packing, pull out all fields less than 8 bytes wide, sort them, and declare at the end
    std::vector<::capnp::StructSchema::Field> fields;
    std::vector<::capnp::StructSchema::Field> packedFields;
    bool shouldPack = hasAnnotation(schema, PACK_ANNOTATION_ID);

    for (auto f : fs) {
        if (ignore(f)) continue;

        // only include field if it isn't in the inherited list
        auto it = inheritedFields.find(f.getProto().getName());
        if (it != inheritedFields.end()) {
            if (!equivalentTypes(it->second.getType(), f.getType())) {
                throw std::runtime_error(std::string("field type must match inherited field: ")
                    + schema.getShortDisplayName().cStr() + "." + f.getProto().getName().cStr());
            }

            inheritedFields.erase(it);
            continue;
        }

        if (shouldPack && pack_sizes.count(f.getType().which()) > 0) {
            packedFields.push_back(f);
        } else {
            fields.push_back(f);
        }
    }

    if (!inheritedFields.empty()) {
        std::ostringstream ss;
        ss << "must declare inherited fields in ";
        ss << schema.getShortDisplayName().cStr() << ":";
        for (auto& [name, _] : inheritedFields) {
            ss << " " << name;
        }
        throw std::runtime_error(ss.str());
    }

    std::sort(packedFields.begin(), packedFields.end(), [](const ::capnp::StructSchema::Field& f1, const ::capnp::StructSchema::Field& f2) {
        return pack_sizes[f1.getType().which()] > pack_sizes[f2.getType().which()];
    });

    gs.startStruct("_field");
    for (auto f : fields) {
        generateFieldConstant(gs, f);
    }
    for (auto f : packedFields) {
        generateFieldConstant(gs, f);
    }
    gs.endStruct();
    gs.nl();

    for (auto f : fields) {
        generateField(gs, f);
    }
    for (auto f : packedFields) {
        generateField(gs, f);
    }

    // equality operator
    gs.nl();
    gs.indent().str("bool operator==(const ").token(name).str("& o) const ").openBrace();
    gs.indent().str("return ");

    bool first = true;

    for (uint i = 0; i < fs.size(); i++) {
        if (ignore(fs[i])) continue;

        auto n = fs[i].getProto().getName().cStr();
        if (!first) {
            gs.nl().indent().tab().str("&& ");
        }

        first = false;
        gs.token(n).str(" == o.").token(n);
    }

    if (first) {
        gs.str("true");
    }

    gs.str(";").nl();

    gs.closeBrace().nl();

    // inequality operator
    gs.indent().str("bool operator!=(const ").token(name).str("& o) const ").openBrace();
    gs.indent().str("return !operator==(o);").nl();
    gs.closeBrace();

    if (schema.asStruct().getUnionFields().size() == 0
        && !hasAnnotation(schema, NOHASH_ANNOTATION_ID)) {

        gs.nl().startStruct("_Hasher");

        gs.indent().str("std::size_t operator()(const ");
        gs.podType(schema);
        gs.str("& o) const ").openBrace();

        gs.indent().str("std::size_t s = 0;").nl();

        for (auto f : schema.getNonUnionFields()) {
            if (ignore(f)) continue;
            auto name = f.getProto().getName().cStr();
            gs.indent().str("s = podgen::hashCombine(s, o.").token(name).str(");").nl();
        }

        gs.indent().str("return s;").nl();
        gs.closeBrace().nl();

        gs.endStruct();
    }
}

void generateGroup(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = std::string("_") + field.getProto().getName().cStr();
    gs.startStruct(name);
    generateStructFields(gs, field.getType().asStruct(), name);
    gs.endStruct(field.getProto().getName().cStr());
}

void generateGroupInUnion(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = std::string("_") + field.getProto().getName().cStr();
    gs.startStruct(name);
    generateStructFields(gs, field.getType().asStruct(), name);
    gs.endStruct();
}

void generateDefaultValue(PodGenStream& gs, ::capnp::Type type, ::capnp::schema::Value::Reader value) {
    switch (value.which()) {
        case ::capnp::schema::Value::Which::BOOL:
            gs.str(" = ").str(value.getBool() ? "true" : "false");
            break;
        case ::capnp::schema::Value::Which::INT8:
            gs.str(" = ").str(std::to_string(value.getInt8()));
            break;
        case ::capnp::schema::Value::Which::INT16:
            gs.str(" = ").str(value.getInt16());
            break;
        case ::capnp::schema::Value::Which::INT32:
            gs.str(" = ").str(value.getInt32());
            break;
        case ::capnp::schema::Value::Which::INT64:
            gs.str(" = ").str(value.getInt64()).str("LL");
            break;
        case ::capnp::schema::Value::Which::UINT8:
            gs.str(" = ").str(std::to_string(value.getUint8()));
            break;
        case ::capnp::schema::Value::Which::UINT16:
            gs.str(" = ").str(value.getUint16());
            break;
        case ::capnp::schema::Value::Which::UINT32:
            gs.str(" = ").str(value.getUint32());
            break;
        case ::capnp::schema::Value::Which::UINT64:
            gs.str(" = ").str(value.getUint64()).str("ULL");
            break;
        case ::capnp::schema::Value::Which::FLOAT32:
            gs.str(" = ").str(value.getFloat32());
            break;
        case ::capnp::schema::Value::Which::FLOAT64:
            gs.str(" = ").str(value.getFloat64());
            break;
        case ::capnp::schema::Value::Which::TEXT: {
            auto text = value.getText();
            if (text.size() > 0) {
                gs.str(" = \"").str(text.cStr()).chr('"');   // need to encode this
            }
            break;
        }
        case ::capnp::schema::Value::Which::DATA: {
            auto data = value.getData();
            if (data.size() > 0) {
                gs.str(" {");
                for (uint i = 0; i < data.size(); i++) {
                    if (i > 0) {
                        gs.chr(',');
                    }
                    gs.str("std::byte{").str(std::to_string(data[i])).chr('}');
                }
                gs.chr('}');
            }
            break;
        }
        case ::capnp::schema::Value::Which::ENUM: {
            auto enumType = type.asEnum();
            for (auto e : enumType.getEnumerants()) {
                if (e.getOrdinal() == value.getEnum()) {
                    gs.str(" = ");
                    gs.podType(enumType);
                    gs.str("::").cap(e.getProto().getName().cStr());
                    break;
                }
            }
            break;
        }
        default:
            break;
    }
}

void generatePlainField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    gs.indent();
    gs.podType(field);
    gs << ' ';
    gs.token(field.getProto().getName().cStr());

    if (hasAnnotation(field, DEFAULT_VALUE_ANNOTATION_ID)) {
        auto anno = getAnnotation(field, DEFAULT_VALUE_ANNOTATION_ID);
        auto dv = anno->getStruct().getAs<DefaultValue>();
        if (dv.getValue().size() > 0) {
            gs.str(" = ").str(dv.getValue().cStr());
        }
    } else if (hasAnnotation(field, CONVERTER_ANNOTATION_ID)) {
        auto anno = getAnnotation(field, CONVERTER_ANNOTATION_ID);
        auto dv = anno->getStruct().getAs<Converter>();
        if (dv.getDefault().size() > 0) {
            gs.str(" = ").str(dv.getDefault().cStr());
        }
    } else {
        auto slot = field.getProto().getSlot();
        if (slot.hasDefaultValue()) {
            generateDefaultValue(gs, field.getType(), slot.getDefaultValue());
        }
    }

    gs.str(";").nl();
}

void generateField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    if (isUnionField(field)) {
        generateUnionField(gs, field);
    } else if (field.getProto().isGroup()) {
        generateGroup(gs, field);
    } else {
        generatePlainField(gs, field);
    }
}

void generateFieldConstant(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    gs.indent().str("static inline const std::string ");
    gs.token(field.getProto().getName().cStr()).str(" = \"");
    gs.str(field.getProto().getName().cStr()).str("\";").nl();
}

InheritedFields getInheritedFields(PodGenStream& gs, const ::capnp::StructSchema& schema) {
    auto anno = getAnnotation(schema, INHERIT_ANNOTATION_ID);
    if (!anno) {
        return {};
    }

    auto rewriteType = [&gs, &schema](std::string type) {
        auto pos = type.find('.');
        if (pos != std::string::npos) {
            auto it = gs.info.importAliases.find(type.substr(0, pos));
            if (it != gs.info.importAliases.end()) {
                type[pos] = ':';
                type.replace(0, pos, it->second);
                return type;
            } else {
                throw std::runtime_error("invalid type alias in " + type);
            }
        } else {
            auto parts = parseCapnpTypeName(schema.getProto().getDisplayName());
            return parts[0] + ":" + type;
        }
    };

    InheritedFields inherits;

    auto list = anno->getList().getAs<::capnp::List<::capnp::Text>>();
    for (auto e : list) {
        std::string type = rewriteType(e.cStr());
        auto sch = gs.findInternalType(type);
        if (sch) {
            auto fs = sch->getNonUnionFields();
            if (fs.size() == 0) {
                throw std::runtime_error(std::string("inherited type must have non-union fields: ") + e.cStr());
            }

            for (auto f : fs) {
                std::string name = f.getProto().getName();
                if (inherits.count(name) > 0) {
                    throw std::runtime_error("duplicate inherited field name: " + name);
                }
                inherits.emplace(name, f);
            }
        }
    }

    return inherits;
}

void generateStruct(PodGenStream& gs, ::capnp::ParsedSchema schema) {
    auto name = schema.getShortDisplayName().cStr();

    // don't render a pod when an external type is defined
    if (gs.hasExternalType(schema.asStruct().getProto().getDisplayName())) {
        gs.indent().str("using ").str(name).str(" = ").podType(schema, false).str(";").nl().nl();
        return;
    }

    // generate template parameters
    if (schema.getProto().hasParameters()) {
        gs.indent().str("template <");
        for (size_t i = 0; i < schema.getProto().getParameters().size(); i++) {
            if (i > 0) {
                gs.str(", ");
            }
            gs.str("typename ").str(schema.getProto().getParameters()[i].getName().cStr());
        }
        gs.str(">").nl();
    }

    if (schema.asStruct().getUnionFields().size() > 0) {
        if (schema.asStruct().getNonUnionFields().size() == 0) {
            generateUnion(gs, schema.asStruct());
            return;
        } else {
            throw std::runtime_error(std::string("unsupported: anonymous union plus non-anonymous fields (")
                + schema.asStruct().getShortDisplayName().cStr() + ")");
        }
    }

    auto inherits = getInheritedFields(gs, schema.asStruct());

    CapnpTypeSet inheritedTypes;
    for (auto& [_, field] : inherits) {
        inheritedTypes.insert(field.getContainingStruct());
    }

    gs.startStruct(name, inheritedTypes);
    gs.indent().str("static inline const std::string TypeName = \"").str(name).str("\";").nl();
    gs.nl();
    generatePods(gs, schema);
    generateStructFields(gs, schema.asStruct(), name, inherits);
    gs.endStruct().nl();
}

void generateEnum(PodGenStream& gs, ::capnp::EnumSchema schema) {
    auto name = schema.getShortDisplayName().cStr();
    auto id = schema.getProto().getId();
    auto enumerants = schema.getEnumerants();

    char idHex[17];
    sprintf(idHex, "%016llx", (long long)id);

    std::string alias = std::string(name) + "_" + idHex;

    if (gs.inStruct()) {
        gs.startStruct(alias);
    } else {
        gs.startNamespace(alias);
    }

    gs.indent().str("enum ").token(name).str(" : uint16_t ").openBrace();
    for (uint i = 0; i < enumerants.size(); i++) {
        if (ignore(enumerants[i])) continue;

        gs.indent().cap(enumerants[i].getProto().getName().cStr());
        if (i < enumerants.size() - 1) {
            gs.chr(',');
        }
        gs.nl();
    }
    gs.closeBrace(";");

    if (gs.inStruct()) {
        gs.endStruct();
    } else {
        gs.endNamespace().nl();
    }

    gs.indent().str("using ").token(name).str(" = ").str(alias).str("::").token(name).str(";").nl().nl();

    // an enum to string convenience
    gs.indent();
    if (gs.inStruct()) {
        gs.str("static ");
    }
    gs.str("inline const std::string ").token(name).str("_TypeName = \"").str(name).str("\";").nl();

    gs.indent();
    if (gs.inStruct()) {
        gs.str("static ");
    }
    gs.str("inline const std::vector<std::string> ").token(name).str("_names ").openBrace();
    for (uint i = 0; i < enumerants.size(); i++) {
        if (ignore(enumerants[i])) continue;

        gs.indent().chr('"').cap(enumerants[i].getProto().getName().cStr()).chr('"');
        if (i < enumerants.size() - 1) {
            gs.chr(',');
        }
        gs.nl();
    }
    gs.closeBrace(";").nl();
}

void generateConst(PodGenStream& gs, ::capnp::ConstSchema schema) {
    gs.indent();
    if (gs.inStruct()) {
        gs.str("static ");
    }
    gs.str("inline const ");
    gs.podType(schema.getType());
    gs << ' ';

    auto parts = parseCapnpTypeName(schema.getProto().getDisplayName());
    gs.token(parts.back());
    generateDefaultValue(gs, schema.getType(), schema.getProto().getConst().getValue());
    gs.str(";").nl();
}

void generatePods(PodGenStream& gs, ::capnp::ParsedSchema schema) {
    bool wasConst = false;

    auto nodes = schema.getProto().getNestedNodes();
    for (::capnp::schema::Node::NestedNode::Reader n : nodes) {
        auto s = schema.getNested(n.getName());
        if (ignore(s)) continue;

        switch (s.getProto().which()) {
            case ::capnp::schema::Node::Which::STRUCT: {
                if (wasConst) gs.nl();
                generateStruct(gs, s);
                wasConst = false;
                break;
            }

            case ::capnp::schema::Node::Which::ENUM: {
                if (wasConst) gs.nl();
                generateEnum(gs, s.asEnum());
                wasConst = false;
                break;
            }

            case ::capnp::schema::Node::Which::CONST: {
                generateConst(gs, s.asConst());
                wasConst = true;
                break;
            }

            default:
                break;
        }
    }

    if (wasConst) gs.nl();
}

void generateConvertFromStructFunctionLine(PodGenStream& gs, ::capnp::StructSchema schema) {
    gs.podType(schema, true);
    gs.str(" podFromCapnp(");
    gs.capnpType(schema, true, true);
    gs.str("::Reader r)");
}

void generateConvertFromEnumFunction(PodGenStream& gs, ::capnp::EnumSchema schema) {
    gs.str("inline auto podFromCapnp(");
    gs.capnpType(schema, true);
    gs.str(" e) { return static_cast<");
    gs.podType(schema, true);
    gs.str(">(e); }").nl();
}

void generateConvertToStructFunctionLine(PodGenStream& gs, ::capnp::StructSchema schema) {
    gs.str("void podToCapnp(");
    gs.capnpType(schema, true, true);
    gs.str("::Builder b, const ");
    gs.podType(schema, true);
    gs.str("& obj)");
}

void generateConvertToEnumFunction(PodGenStream& gs, ::capnp::EnumSchema schema) {
    gs.str("inline auto podToCapnp(");
    gs.podType(schema, true);
    gs.str(" e) { return static_cast<");
    gs.capnpType(schema, true);
    gs.str(">(e); }").nl();
}

void generateCapnpTypeFunction(PodGenStream& gs, ::capnp::Schema schema) {
    gs.capnpType(schema, true);
    gs.str(" capnpTypeOf(");
    if (schema.getProto().isStruct()) {
        gs.podType(schema.asStruct(), true);
    } else if (schema.getProto().isEnum()) {
        gs.podType(schema.asEnum(), true);
    }
    gs.str(");").nl();

    gs.str("::capnp::List<");
    gs.capnpType(schema, true);
    gs.str(", ::capnp::Kind::");

    if (schema.getProto().isStruct()) {
        gs.str("STRUCT");
    } else if (schema.getProto().isEnum()) {
        gs.str("ENUM");
    }

    gs.str("> capnpTypeOf(");
    gs.str("std::vector<");
    if (schema.getProto().isStruct()) {
        gs.podType(schema.asStruct(), true);
    } else if (schema.getProto().isEnum()) {
        gs.podType(schema.asEnum(), true);
    }
    gs.str(">);").nl();
}

void generateOutputStructFunction(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent) {
    if (schema.getProto().getIsGeneric() && !gs.hasField()) {
        return;
    }

    auto name = schema.getProto().getDisplayName();
    bool hasExternal = gs.hasExternalType(name);

    auto parts = parseCapnpTypeName(name);
    auto ns = podFromCapnpNamespace(gs.findNamespace(parts[0]));
    auto inAltNamespace = ns != gs.namesp;

    if (inAltNamespace) {
        gs.endNamespace();
        gs.startNamespace(ns);
    }

    if (!isUnion(schema)) {
        if (!hasExternal) {
            gs.str("inline ");
        }

        gs.str("void _out(std::ostream& os, const ");
        gs.podType(schema, true);
        gs.str("& o)");

        if (hasExternal) {
            gs.str(";").nl();
        } else {
            gs.str(" ").openBrace();

            gs.indent().str("os << \"").str(schema.getShortDisplayName().cStr()).str("{\";").nl();
            bool first = true;

            for (auto f : schema.getNonUnionFields()) {
                if (ignore(f)) continue;

                if (!first) {
                    gs.indent().str("os << ' ';").nl();
                }
                first = false;

                auto name = f.getProto().getName().cStr();
                gs.indent().str("os << \"").token(name).str("=\";").nl();

                gs.indent();
                auto anno = getAnnotation(getOptionalField(f), CONVERTER_ANNOTATION_ID);
                if (anno) {
                    auto converter = anno->getStruct().getAs<Converter>();
                    gs.str(converter.getOut().cStr());
                } else {
                    if (f.getType().isList() || (f.getType().isStruct() && isUnion(f.getType().asStruct()))) {
                        gs.ns().str("::");
                    }

                    gs.str("_out");
                }

                // don't ever print password fields in debug output
                if (f.getProto().getName() == "password") {
                    gs.str("(os, \"*\");").nl();
                    continue;
                }

                gs.str("(os, o.").token(name).str(");").nl();
            }

            gs.indent().str("os << '}';").nl();
            gs.closeBrace().nl();
        }
    }

    if (!hasExternal) {
        gs.str("inline std::ostream& operator<<(std::ostream& os, const ");
        gs.podType(schema, true);
        gs.str("& o) ").openBrace();
        if (isUnion(schema)) {
            gs.ns().str("::");
        }
        gs.indent().str("_out(os, o);").nl();
        gs.indent().str("return os;").nl();
        gs.closeBrace().nl();
    }

    if (inAltNamespace) {
        gs.endNamespace();
        gs.startNamespace(gs.namesp);
    }
}

void generateOutputEnumFunction(PodGenStream& gs, ::capnp::EnumSchema schema, ::capnp::Schema parent) {
    bool inNamespace = false;

    auto parts = parseCapnpTypeName(schema.getProto().getDisplayName().cStr());
    if (parts.size() <= 2) {
        auto name = schema.getShortDisplayName().cStr();
        auto id = schema.getProto().getId();

        char idHex[17];
        sprintf(idHex, "%016llx", (long long)id);

        std::string alias = std::string(name) + "_" + idHex;
        gs.startNamespace(alias);
        inNamespace = true;
    }

    gs.str("inline void _out(std::ostream& os, const ");
    gs.podType(schema);
    gs.str("& o) ").openBrace();
    gs.indent().str("os << podgen::enumToName(o, ");
    gs.podType(schema);
    gs.str("_names);").nl();
    gs.closeBrace().nl();

    gs.str("inline std::ostream& operator<<(std::ostream& os, const ");
    gs.podType(schema);
    gs.str("& o) ").openBrace();
    gs.indent().str("_out(os, o);").nl();
    gs.indent().str("return os;").nl();
    gs.closeBrace().nl();

    if (inNamespace) {
        gs.endNamespace();
    }
}

void generateHashSpecialization(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent) {
    if (schema.getUnionFields().size() > 0) {
        // nothing to hash (union, optional)
        return;
    }

    if (hasAnnotation(schema, NOHASH_ANNOTATION_ID)) {
        return;
    }

    if (schema.getProto().getIsGeneric() && !gs.hasField()) {
        return;
    }

    gs.str("template <> struct hash<");
    gs.podType(schema, true);
    gs.str("> ").openBrace();
    gs.indent().str("std::size_t operator()(const ");
    gs.podType(schema, true);
    gs.str("& o) const");

    if (gs.hasExternalType(schema.getProto().getDisplayName())) {
        gs.str(";").nl();
    } else {
        gs.str(" ").openBrace();
        gs.indent().str("return ").podType(schema, true, true).str("::_Hasher()(o);").nl();
        gs.closeBrace();
    }

    gs.closeBrace(";").nl();
}

void generateStructConverterSignatures(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent) {
    if (schema.getProto().getIsGeneric() && !gs.hasField()) {
        return;
    }

    generateConvertFromStructFunctionLine(gs, schema);
    gs.chr(';').nl();
    generateConvertToStructFunctionLine(gs, schema);
    gs.chr(';').nl();
//                    generateOutputStructFunctionLine(gs, group);
//                    gs.chr(';').nl();
    generateCapnpTypeFunction(gs, schema);
    gs.nl();
}

void generateEnumConverters(PodGenStream& gs, ::capnp::EnumSchema schema, ::capnp::Schema parent) {
    generateConvertFromEnumFunction(gs, schema);
    generateConvertToEnumFunction(gs, schema);
    generateCapnpTypeFunction(gs, schema);
    gs.nl();
}


// convert from

void generateAssignFromOptional(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto uname = field.getProto().getName();
    auto f = getOptionalField(field);

    gs.indent().openBrace();
    gs.indent().str("auto ur = r.get").cap(uname).str("();").nl();

    std::string from = "_fromCapnp";
    auto anno = getAnnotation(f, CONVERTER_ANNOTATION_ID);
    if (anno) {
        auto converter = anno->getStruct().getAs<Converter>();
        from = converter.getFrom().cStr();
    }

    auto name = f.getProto().getName();
    gs.indent().str("if (ur.is").cap(name).str("()) ").openBrace();
    gs.indent().str("obj.").token(uname).str(".emplace(");
    gs.str(from).str("(ur.get").cap(name).str("())");
    gs.str(");").nl();
    gs.tab(gs.getIndentLevel() - 1).str("} else {").nl();
    gs.indent().str("obj.").token(uname).str(".reset();").nl();
    gs.closeBrace();

    gs.closeBrace();
}

void generateAssignFromUnion(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto uname = field.getProto().getName();
    size_t count = 0;

    gs.indent().openBrace();
    gs.indent().str("auto ur = r.get").cap(uname).str("();").nl();

    for (auto f : field.getType().asStruct().getUnionFields()) {
        if (ignore(f)) continue;

        std::string from = "_fromCapnp";
        auto anno = getAnnotation(f, CONVERTER_ANNOTATION_ID);
        if (anno) {
            auto converter = anno->getStruct().getAs<Converter>();
            from = converter.getFrom().cStr();
        }

        auto name = f.getProto().getName();
        gs.indent().str("if (ur.is").cap(name).str("()) ").openBrace();
        gs.indent().str("obj.").token(uname).str(".emplace<").str(count).str(">(");
        gs.str(from).str("(ur.get").cap(name).str("())");
        gs.str(");").nl();
        gs.closeBrace();
        count++;
    }

    gs.closeBrace();
}

void generateAssignFromGroup(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("obj.").token(name).str(" = _fromCapnp(r.get").cap(name).str("());").nl();
}

void generateAssignFromPointer(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("if (r.has").cap(name).str("()) ").openBrace();
    gs.indent().str("obj.").token(name).str(" = _fromCapnp(r.get").cap(name).str("());").nl();
    gs.closeBrace();
}

void generateAssignFromList(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("if (r.has").cap(name).str("()) ").openBrace();
    gs.indent().str("obj.").token(name).str(" = ");

    if (hasAnnotation(field, CONTAINER_ANNOTATION_ID)) {
        gs.str("_fromCapnpList<");
        gs.podType(field);
        gs.str(">");
    } else {
        gs.str("_fromCapnp");
    }

    gs.str("(r.get").cap(name).str("());").nl();
    gs.closeBrace();
}

void generateAssignFromEnum(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("obj.").token(name).str(" = _fromCapnp(r.get").cap(name).str("());").nl();
}

void generateAssignFromPlain(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("obj.").token(name).str(" = r.get").cap(name).str("();").nl();
}

void generateAssignFromConverter(PodGenStream& gs, ::capnp::StructSchema::Field field, const std::string& from) {
    auto name = field.getProto().getName();
    gs.indent().str("obj.").token(name).str(" = ").str(from).str("(r.get").cap(name).str("());").nl();
}

void generateAssignFromField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto anno = getAnnotation(field, CONVERTER_ANNOTATION_ID);
    if (anno) {
        auto converter = anno->getStruct().getAs<Converter>();
        generateAssignFromConverter(gs, field, converter.getFrom().cStr());
        return;
    }

    switch (field.getType().which()) {
        case ::capnp::schema::Type::Which::VOID:
            return;

        case ::capnp::schema::Type::Which::STRUCT: {
            if (isOptionalField(field)) {
                generateAssignFromOptional(gs, field);
            } else if (isUnionField(field)) {
                generateAssignFromUnion(gs, field);
            } else if (field.getProto().isGroup()) {
                generateAssignFromGroup(gs, field);
            } else {
                generateAssignFromPointer(gs, field);
            }
            break;
        }

        case ::capnp::schema::Type::Which::TEXT:
        case ::capnp::schema::Type::Which::DATA:
            generateAssignFromPointer(gs, field);
            break;

        case ::capnp::schema::Type::Which::LIST:
            generateAssignFromList(gs, field);
            break;

        case ::capnp::schema::Type::Which::ENUM:
            generateAssignFromEnum(gs, field);
            break;

        default:
            generateAssignFromPlain(gs, field);
            break;
    }

}

void generateConvertFromStructFunction(PodGenStream& gs, ::capnp::StructSchema schema) {
    generateConvertFromStructFunctionLine(gs, schema);
    gs.str(" ").openBrace();
    gs.indent().str("using namespace ").ns().str(";").nl();

    gs.indent();
    gs.podType(schema, true);
    gs.str(" obj;").nl();

    if (isOptional(schema)) {
        auto f = *getOptionalField(schema);
        auto name = f.getProto().getName();
        gs.indent().str("if (r.is").cap(name).str("()) ").openBrace();
        gs.indent().str("obj.emplace(");
        gs.str("_fromCapnp(r.get").cap(name).str("())");
        gs.str(");").nl();
        gs.tab(gs.getIndentLevel() - 1).str("} else {").nl();
        gs.indent().str("obj.reset();").nl();
        gs.closeBrace();
    } else if (isUnion(schema)) {
        size_t count = 0;

        for (auto f : schema.getUnionFields()) {
            if (ignore(f)) continue;

            auto name = f.getProto().getName();
            gs.indent().str("if (r.is").cap(name).str("()) ").openBrace();
            gs.indent().str("obj.emplace<").str(count).str(">(");
            gs.str("_fromCapnp(r.get").cap(name).str("())");
            gs.str(");").nl();
            gs.closeBrace();
            count++;
        }
    } else {
        for (auto f : schema.getNonUnionFields()) {
            if (ignore(f)) continue;
            generateAssignFromField(gs, f);
        }
    }

    gs.indent().str("return obj;").nl();
    gs.closeBrace().nl();
}


// convert to

void generateAssignToListNamespace(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    std::string ns;
    auto elementType = getNestedListElementType(field.getType());
    if (elementType.isStruct()) {
        auto parts = parseCapnpTypeName(elementType.asStruct().getProto().getDisplayName());
        ns = gs.findNamespace(parts[0]);
    }

    if (!ns.empty() && ns != gs.namesp) {
        gs.str("::").str(podFromCapnpNamespace(ns)).str("::");
    }
}

void generateAssignToOptional(PodGenStream& gs, ::capnp::StructSchema schema, const std::string& uname) {
    auto initUnion = [&gs, &uname]() {
        if (!uname.empty()) {
            return gs.str("b.init").cap(uname).str("().");
        } else {
            return gs.str("b.");
        }
    };

    auto getValue = [&gs, &uname]() {
        gs.str("obj");
        if (!uname.empty()) {
            gs.str(".").token(uname);
        }
        gs.str(".value()");
    };

    auto ufs = schema.getUnionFields();
    auto valueField = !ufs[0].getType().isVoid() ? ufs[0] : ufs[1];
    auto voidField = ufs[0].getType().isVoid() ? ufs[0] : ufs[1];

    gs.indent().str("if (obj");
    if (!uname.empty()) {
        gs.str(".").token(uname);
    }
    gs.str(") ").openBrace();
    gs.indent();

    std::string to;
    auto anno = getAnnotation(valueField, CONVERTER_ANNOTATION_ID);
    if (anno) {
        auto converter = anno->getStruct().getAs<Converter>();
        to = converter.getTo().cStr();
    }

    // value clause
    auto name = valueField.getProto().getName();
    switch (valueField.getType().which()) {
        case ::capnp::schema::Type::Which::STRUCT: {
            gs.str("podToCapnp(");
            initUnion();
            gs.str("init").cap(name).str("(), ");
            getValue();
            gs.str(");").nl();
            break;
        }

        case ::capnp::schema::Type::Which::TEXT:
        case ::capnp::schema::Type::Which::DATA: {
            if (to.empty()) {
                to = "_toCapnp";
            }

            initUnion();
            gs.str("set").cap(name).str("(").str(to).str("(");
            getValue();
            gs.str("));").nl();
            break;
        }

        case ::capnp::schema::Type::Which::LIST: {
            generateAssignToListNamespace(gs, valueField);
            gs.str("_toCapnpList(");
            initUnion();
            gs.str("init").cap(name).str("(");
            getValue();
            gs.str(".size()), ");
            getValue();
            gs.str(");").nl();
            break;
        }

        case ::capnp::schema::Type::Which::ENUM: {
            initUnion();
            gs.str("set").cap(name).str("(podToCapnp(");
            getValue();
            gs.str("));").nl();
            break;
        }

        default: {
            initUnion();
            gs.str("set").cap(name).str("(");
            if (!to.empty()) {
                gs.str(to).str("(");
            }
            getValue();
            if (!to.empty()) {
                gs.str(")");
            }
            gs.str(");").nl();
            break;
        }
    }

    // nullopt clause
    name = voidField.getProto().getName();
    gs.tab(gs.getIndentLevel() - 1).str("} else {").nl();
    gs.indent();
    initUnion();
    gs.str("set").cap(name).str("();").nl();

    gs.closeBrace();
}

void generateAssignToOptionalField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto uname = field.getProto().getName();
    generateAssignToOptional(gs, field.getType().asStruct(), uname);
}

void generateAssignToOptionalStruct(PodGenStream& gs, ::capnp::StructSchema schema) {
    generateAssignToOptional(gs, schema, "");
}

void generateAssignToUnion(PodGenStream& gs, ::capnp::StructSchema schema, const std::string& uname) {
    size_t count = 0;

    auto initUnion = [&gs, &uname]() {
        if (!uname.empty()) {
            return gs.str("b.init").cap(uname).str("().");
        } else {
            return gs.str("b.");
        }
    };

    auto getVariant = [&gs, &uname](size_t count) {
        gs.str("std::get<").str(count).str(">(obj");
        if (!uname.empty()) {
            gs.str(".").token(uname);
        }
        return gs.str(")");
    };

    gs.indent().str("switch (obj");
    if (!uname.empty()) {
        gs.str(".").token(uname);
    }
    gs.str(".index()) ").openBrace();

    for (auto f : schema.getUnionFields()) {
        if (ignore(f)) continue;

        auto name = f.getProto().getName();

        std::string to = "";
        auto anno = getAnnotation(f, CONVERTER_ANNOTATION_ID);
        if (anno) {
            auto converter = anno->getStruct().getAs<Converter>();
            to = converter.getTo().cStr();
        }

        gs.indent().str("case ").str(count).str(": ").openBrace();
        gs.indent();

        switch (f.getType().which()) {
            case ::capnp::schema::Type::Which::VOID: {
                initUnion();
                gs.str("set").cap(name).str("();").nl();
                break;
            }

            case ::capnp::schema::Type::Which::STRUCT: {
                gs.str("podToCapnp(");
                initUnion();
                gs.str("init").cap(name).str("(), ");
                getVariant(count);
                gs.str(");").nl();
                break;
            }

            case ::capnp::schema::Type::Which::TEXT:
            case ::capnp::schema::Type::Which::DATA: {
                if (to.empty()) {
                    to = "_toCapnp";
                }

                gs.indent();
                initUnion();
                gs.str("set").cap(name).str("(").str(to).str("(");
                getVariant(count);
                gs.str("));").nl();
                break;
            }

            case ::capnp::schema::Type::Which::LIST: {
                gs.indent();
                generateAssignToListNamespace(gs, f);
                gs.str("_toCapnpList(");
                initUnion();
                gs.str("init").cap(name).str("(");
                getVariant(count);
                gs.str(".size()), ");
                getVariant(count);
                gs.str(");").nl();
                break;
            }

            case ::capnp::schema::Type::Which::ENUM: {
                initUnion();
                gs.str("set").cap(name).str("(podToCapnp(");
                getVariant(count);
                gs.str("));").nl();
                break;
            }

            default: {
                initUnion();
                gs.str("set").cap(name).str("(");
                if (!to.empty()) {
                    gs.str(to).str("(");
                }
                getVariant(count);
                if (!to.empty()) {
                    gs.str(")");
                }
                gs.str(");").nl();
                break;
            }
        }

        gs.indent().str("break;").nl();
        gs.closeBrace();

        count++;
    }

    gs.closeBrace();
}

void generateAssignToUnionField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto uname = field.getProto().getName();
    generateAssignToUnion(gs, field.getType().asStruct(), uname);
}

void generateAssignToUnionStruct(PodGenStream& gs, ::capnp::StructSchema schema) {
    generateAssignToUnion(gs, schema, "");
}

void generateAssignToList(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();

    gs.indent();
    generateAssignToListNamespace(gs, field);
    gs.str("_toCapnpList(b.init").cap(name).str("(obj.").token(name).str(".size()), obj.").token(name).str(");").nl();
}

void generateAssignToStruct(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("podToCapnp(b.init").cap(name).str("(), obj.").token(name).str(");").nl();
}

void generateAssignToPointer(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("b.set").cap(name).str("(_toCapnp(obj.").token(name).str("));").nl();
}

void generateAssignToEnum(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("b.set").cap(name).str("(podToCapnp(obj.").token(name).str("));").nl();
}

void generateAssignToPlain(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto name = field.getProto().getName();
    gs.indent().str("b.set").cap(name).str("(obj.").token(name).str(");").nl();
}

void generateAssignToConverter(PodGenStream& gs, ::capnp::StructSchema::Field field, const std::string& to) {
    auto name = field.getProto().getName();
    gs.indent().str("b.set").cap(name).str("(").str(to).str("(obj.").token(name).str("));").nl();
}

void generateAssignToField(PodGenStream& gs, ::capnp::StructSchema::Field field) {
    auto anno = getAnnotation(field, CONVERTER_ANNOTATION_ID);
    if (anno) {
        auto converter = anno->getStruct().getAs<Converter>();
        generateAssignToConverter(gs, field, converter.getTo().cStr());
        return;
    }

    switch (field.getType().which()) {
        case ::capnp::schema::Type::Which::VOID:
            return;

        case ::capnp::schema::Type::Which::STRUCT: {
            if (isOptionalField(field)) {
                generateAssignToOptionalField(gs, field);
            } else if (isUnionField(field)) {
                generateAssignToUnionField(gs, field);
            } else {
                generateAssignToStruct(gs, field);
            }
            break;
        }

        case ::capnp::schema::Type::Which::TEXT:
        case ::capnp::schema::Type::Which::DATA:
            generateAssignToPointer(gs, field);
            break;

        case ::capnp::schema::Type::Which::LIST:
            generateAssignToList(gs, field);
            break;

        case ::capnp::schema::Type::Which::ENUM:
            generateAssignToEnum(gs, field);
            break;

        default:
            generateAssignToPlain(gs, field);
            break;
    }
}

void generateConvertToStructFunction(PodGenStream& gs, ::capnp::StructSchema schema) {
    generateConvertToStructFunctionLine(gs, schema);
    gs.str(" ").openBrace();
    gs.indent().str("using namespace ").ns().str(";").nl();

    if (isOptional(schema)) {
        generateAssignToOptionalStruct(gs, schema);
    } else if (isUnion(schema)) {
        generateAssignToUnionStruct(gs, schema);
    } else {
        for (auto f : schema.getNonUnionFields()) {
            if (ignore(f)) continue;
            generateAssignToField(gs, f);
        }
    }

    gs.closeBrace().nl();
}

void generateStructConverters(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent) {
    if (schema.getProto().getIsGeneric() && !gs.hasField()) {
        return;
    }

    if (hasAnnotation(schema, EXTTYPE_ANNOTATION_ID)) {
        return;
    }

    generateConvertFromStructFunction(gs, schema);
    generateConvertToStructFunction(gs, schema);
}

void recurseStructFields(::capnp::StructSchema schema, const std::function<void(::capnp::StructSchema::Field)>& fn) {
    for (auto f : schema.getFields()) {
        fn(f);
        if (f.getType().isStruct()) {
            recurseStructFields(f.getType().asStruct(), fn);
        }
    }
}

/// Run the from-struct generator for the struct schema and all its nested group structs.
void generateFromStruct(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent,
    const FromStructGenerator& sgen, std::unordered_set<size_t>& processed) {

    // only process this schema once
    auto h = schema.hashCode();
    if (processed.count(h) > 0) {
        return;
    }

    processed.insert(h);

    // this block digs into all struct fields looking for fully specialized generic types.
    // converters/outputters are generated only for generics with all type parameters filled in,
    // which are only found in field definitions.
    if (!gs.info.schemaParentOf.empty()) {
        if (!schema.getProto().getIsGeneric() || gs.hasField()) {
            auto saveField = gs.getField();

            recurseStructFields(schema, [&gs, &sgen, &processed](::capnp::StructSchema::Field f) {
                if (ignore(f)) return;
                if (f.getType().isStruct() && !isOptionalField(f)) {
                    auto s = f.getType().asStruct();
                    if (s.getProto().getIsGeneric()) {
                        gs.setField(f);
                        generateFromStruct(gs, f.getType().asStruct(), *gs.findParentOf(s), sgen, processed);
                        gs.clearField();
                    }
                }
            });

            if (saveField) {
                gs.setField(*saveField);
            }
        }
    }

    // this block digs into struct/union fields to find all group types.
    for (auto f : schema.getNonUnionFields()) {
        if (ignore(f)) continue;
        if (isGroupField(f)) {
            auto group = f.getType().asStruct();
            generateFromStruct(gs, group, schema, sgen, processed);
        } else if (isUnionField(f)) {
            for (auto uf : f.getType().asStruct().getUnionFields()) {
                if (ignore(uf)) continue;
                if (isGroupField(uf)) {
                    auto group = uf.getType().asStruct();
                    generateFromStruct(gs, group, schema, sgen, processed);
                }
            }
        }
    }

    sgen(gs, schema, parent);
}

/// Run the from-struct and from-enum generators on all types encountered in nested schemas.
size_t generateFromSchema(PodGenStream& gs, ::capnp::ParsedSchema schema,
                         const FromStructGenerator& sgen, const FromEnumGenerator& egen) {
    size_t count = 0;
    std::unordered_set<size_t> processed;

    auto nodes = schema.getProto().getNestedNodes();
    for (::capnp::schema::Node::NestedNode::Reader n : nodes) {
        auto s = schema.getNested(n.getName());
        if (ignore(s)) continue;

        switch (s.getProto().which()) {
            case ::capnp::schema::Node::Which::STRUCT: {
                if (sgen) {
                    if (!isUnion(s.asStruct())) {
                        count += generateFromSchema(gs, s, sgen, egen);
                    } else if (s.getProto().getNestedNodes().size() > 0) {
                        throw std::runtime_error(std::string("unsupported: anonymous union with nested structs (")
                            + s.getShortDisplayName().cStr() + ")");
                    }

                    generateFromStruct(gs, s.asStruct(), schema, sgen, processed);
                }

                count++;
                break;
            }

            case ::capnp::schema::Node::Which::ENUM: {
                if (egen) {
                    egen(gs, s.asEnum(), schema);
                }

                count++;
                break;
            }

            default:
                break;
        }
    }

    return count;
}

void generateFile(std::istream& is, std::ostream& os, const SchemaInfo& info,
    const std::filesystem::path& capnpFile) {

    auto namespaces = info.importNamespaces;
    auto aliases = info.importAliases;
    namespaces.merge(aliases);

    auto currentNamespace = podFromCapnpNamespace(getNamespace(info.schema));
    PodGenStream gs(os, currentNamespace, info);

    std::string line;
    while (std::getline(is, line)) {
        if (line == "{{include_pod}}") {
            auto header = capnpToPodImport(capnpFile);
            gs.str("#include \"").str(header.string()).str("\"").nl();
        } else if (line == "{{include_converter}}") {
            auto header = capnpToConvertImport(capnpFile);
            gs.str("#include \"").str(header.string()).str("\"").nl();
        } else if (line == "{{import_pods}}") {
            for (const auto& [import, _] : info.importNamespaces) {
                if (import != capnpFile.string()) {
                    auto p = capnpToPodImport(import);
                    gs.str("#include \"").str(p.string()).str("\"").nl();
                }
            }

            for (const auto& import : info.podHeaders) {
                if (import.empty()) {
                    continue;
                } else if (import[0] == '<') {
                    // allow for system headers
                    gs.str("#include ").str(import).nl();
                } else {
                    gs.str("#include \"").str(import).str("\"").nl();
                }
            }
        } else if (line == "{{import_converters}}") {
            auto capnpHeader = capnpFile;
            capnpHeader.replace_extension(".capnp.h");
            gs.str("#include \"").str(capnpHeader.string()).str("\"").nl();

            for (const auto& [import, _] : info.importNamespaces) {
                if (import != capnpFile.string()) {
                    auto p = capnpToConvertImport(import);
                    gs.str("#include \"").str(p.string()).str("\"").nl();
                }
            }
        } else if (line == "{{namespace_start}}") {
            if (!gs.namesp.empty()) {
                gs.startNamespace(gs.namesp);
            }
        } else if (line == "{{schema}}") {
            generatePods(gs, info.schema);
        } else if (line == "{{output}}") {
            generateFromSchema(gs, info.schema, generateOutputStructFunction, generateOutputEnumFunction);
        } else if (line == "{{hash}}") {
            generateFromSchema(gs, info.schema, generateHashSpecialization);
        } else if (line == "{{converter_function_lines}}") {
            generateFromSchema(gs, info.schema, generateStructConverterSignatures, generateEnumConverters);
        } else if (line == "{{converter_functions}}") {
            generateFromSchema(gs, info.schema, generateStructConverters);
        } else if (line == "{{namespace_end}}") {
            if (!gs.namesp.empty()) {
                gs.endNamespace();
            }
        } else {
            static const std::string NS_TAG = "{{namespace}}";
            auto p = line.find(NS_TAG);
            if (p != std::string::npos) {
                line.replace(p, NS_TAG.size(), currentNamespace);
            }

            gs.str(line).nl();
        }
    }
}

/// Look up all union names within the schema, including unions in nested structs and groups.
Unions findUnionFields(const ::capnp::ParsedSchema& schema) {
    PodGenStream gs(std::cout, "", {});   // dummy
    Unions unions;

    auto findUnions = [&unions](PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent) {
        for (auto f : schema.getNonUnionFields()) {
            if (ignore(f)) continue;
            if (isUnionField(f)) {
                unions.emplace(f.getType().asStruct().getProto().getDisplayName().cStr());
            }
        }
    };

    generateFromSchema(gs, schema, findUnions);
    return unions;
}

void findExternals(PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent, ExternalTypes& externals) {
    auto anno = getAnnotation(schema, EXTTYPE_ANNOTATION_ID);
    if (anno) {
        auto exttype = anno->getStruct().getAs<ExternalType>();
        externals.typeMap.emplace(schema.getProto().getDisplayName(), exttype.getType());
        if (exttype.getHdr().size() > 0) {
            externals.podHeaders.insert(exttype.getHdr());
        }
    }

    for (auto f : schema.getFields()) {
        if (ignore(f)) continue;

        auto anno = getAnnotation(f, CONTAINER_ANNOTATION_ID);
        if (anno) {
            auto exttype = anno->getStruct().getAs<ExternalType>();
            if (exttype.getHdr().size() > 0) {
                externals.podHeaders.insert(exttype.getHdr());
            }
        }

        anno = getAnnotation(f, DEFAULT_VALUE_ANNOTATION_ID);
        if (anno) {
            auto defaultValue = anno->getStruct().getAs<DefaultValue>();
            if (defaultValue.getHdr().size() > 0) {
                externals.podHeaders.insert(defaultValue.getHdr());
            }
        }

        anno = getAnnotation(f, CONVERTER_ANNOTATION_ID);
        if (anno) {
            auto converter = anno->getStruct().getAs<Converter>();
            if (converter.getHdr().size() > 0) {
                externals.podHeaders.insert(converter.getHdr());
            }
        }

        // must dig down into nested structs/groups/unions
        if (f.getType().isStruct()) {
            findExternals(gs, f.getType().asStruct(), schema, externals);
        }
    }
};

ExternalTypes findExternalTypes(const ::capnp::ParsedSchema& schema) {
    PodGenStream gs(std::cout, "", {});   // dummy
    ExternalTypes externals;

    auto find = [&externals](PodGenStream& gs, ::capnp::StructSchema schema, ::capnp::Schema parent) {
        findExternals(gs, schema, parent, externals);
    };

    generateFromSchema(gs, schema, find);
    return externals;
}

