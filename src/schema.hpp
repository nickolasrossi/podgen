#pragma once

#include <ctype.h>
#include <string>
#include <optional>
#include <unordered_set>
#include <limits>

#include <capnp/schema.h>

/// Whether the struct schema is a union
bool isUnion(::capnp::StructSchema schema);

/**
 * Whether the struct schema is what we consider an "optional" type:
 *
 * - a union
 * - with two fields
 * - one of the fields is Void (representing nullopt)
 * - the other field is not Void (representing the T type in optional<T>)
 */
bool isOptional(::capnp::StructSchema schema);

/// Whether the field is a group.
bool isGroupField(::capnp::StructSchema::Field field);

/// Whether the field is a union.
bool isUnionField(::capnp::StructSchema::Field field);

/// Whether the field is an optional type.
bool isOptionalField(::capnp::StructSchema::Field field);

/// Return the true typed field within an optional union (the one that isn't void).
/// If not an optional, returns the submitted field.
std::optional<::capnp::StructSchema::Field> getOptionalField(::capnp::StructSchema schema);
::capnp::StructSchema::Field getOptionalField(::capnp::StructSchema::Field field);

std::optional<::capnp::StructSchema::Field> getOptionalNull(::capnp::StructSchema schema);
std::optional<::capnp::StructSchema::Field> getOptionalNull(::capnp::StructSchema::Field field);

/// Get the nested list element type (the T in List<List<...<T>>>)
::capnp::Type getNestedListElementType(::capnp::Type type);

/// Returns whether the type is a numeric type
bool isNumeric(::capnp::Type type);

/// Returns whether the type is an integer type
bool isInteger(::capnp::Type type);

/// Parse a type name from the capnp schema into its source file and name parts.
/// "messaging/capnp/test/test.capnp:Simple.choice" => [ "messaging/capnp/test/test.capnp", "Simple", "choice" ]
std::vector<std::string> parseCapnpTypeName(const std::string& name);

/// Merge a parsed type name back to a capnp schema name.
std::string mergeCapnpTypeName(const std::vector<std::string>& parts, size_t n = std::numeric_limits<size_t>::max());

/// Look up whether the element contains the annotation by id.
template <typename E>
bool hasAnnotation(const E& element, uint64_t annotationId) {
    for (::capnp::schema::Annotation::Reader a : element.getProto().getAnnotations()) {
        if (a.getId() == annotationId) {
            return true;
        }
    }

    return false;
}

/// Look up whether the element contains the annotation by id.
template <typename E>
bool hasAnnotation(const E& element, const std::unordered_set<uint64_t>& annotationIds) {
    for (::capnp::schema::Annotation::Reader a : element.getProto().getAnnotations()) {
        if (annotationIds.count(a.getId()) > 0) {
            return true;
        }
    }

    return false;
}

/// Return annotation by id from the element if present.
template <typename E>
std::optional<::capnp::schema::Value::Reader> getAnnotation(const E& element, uint64_t annotationId) {
    for (::capnp::schema::Annotation::Reader a : element.getProto().getAnnotations()) {
        if (a.getId() == annotationId) {
            return a.getValue();
        }
    }

    return {};
}

/// Obtain the union field name from a pod variant.
template <typename U>
std::string getVariantFieldName(const U& un) {
    using CapnpType = decltype(capnpTypeOf(std::declval<U>()));
    ::capnp::StructSchema schema = ::capnp::Schema::from<CapnpType>();
    auto ufs = schema.getUnionFields();
    ::capnp::StructSchema::Field f = ufs[un.index()];
    return f.getProto().getName();
}


// Hash specializations for capnp schema elements

namespace std {

    template <> struct hash<::capnp::Schema> {
        size_t operator()(const ::capnp::Schema& s) const {
            return s.hashCode();
        }
    };

    template <> struct hash<::capnp::StructSchema::Field> {
        size_t operator()(const ::capnp::StructSchema::Field& f) const {
            return f.hashCode();
        }
    };

    template <> struct hash<::capnp::Type> {
        size_t operator()(const ::capnp::Type& t) const {
            return t.hashCode();
        }
    };

}
