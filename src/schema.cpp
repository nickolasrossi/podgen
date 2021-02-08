#include <vector>
#include <sstream>

#include "schema.hpp"

bool isUnion(::capnp::StructSchema schema) {
    return schema.getUnionFields().size() > 0;
}

bool isOptional(::capnp::StructSchema schema) {
    auto ufs = schema.getUnionFields();
    return ufs.size() == 2
        && ((ufs[0].getType().isVoid() && !ufs[1].getType().isVoid())
            || (ufs[1].getType().isVoid() && !ufs[0].getType().isVoid()));
}

std::optional<::capnp::StructSchema::Field> getOptionalField(::capnp::StructSchema schema) {
    auto ufs = schema.getUnionFields();
    if (ufs.size() == 2) {
        if (ufs[0].getType().isVoid() && !ufs[1].getType().isVoid()) {
            return ufs[1];
        } else if (ufs[1].getType().isVoid() && !ufs[0].getType().isVoid()) {
            return ufs[0];
        }
    }

    return {};
}

std::optional<::capnp::StructSchema::Field> getOptionalNull(::capnp::StructSchema schema) {
    auto ufs = schema.getUnionFields();
    if (ufs.size() == 2) {
        if (ufs[0].getType().isVoid() && !ufs[1].getType().isVoid()) {
            return ufs[0];
        } else if (ufs[1].getType().isVoid() && !ufs[0].getType().isVoid()) {
            return ufs[1];
        }
    }

    return {};
}

bool isGroupField(::capnp::StructSchema::Field field) {
    return field.getType().which() == ::capnp::schema::Type::Which::STRUCT
           && field.getType().asStruct().getNonUnionFields().size() > 0
           && field.getProto().isGroup();
}

bool isUnionField(::capnp::StructSchema::Field field) {
    // a capitalized type name indicates a standalone type, not an anonymous field type
    return field.getType().which() == ::capnp::schema::Type::Which::STRUCT
           && isUnion(field.getType().asStruct())
           && !isupper(field.getType().asStruct().getShortDisplayName()[0]);
}

bool isOptionalField(::capnp::StructSchema::Field field) {
    // a capitalized type name indicates a standalone type, not an anonymous field type
    return field.getType().which() == ::capnp::schema::Type::Which::STRUCT
           && isOptional(field.getType().asStruct())
           && !isupper(field.getType().asStruct().getShortDisplayName()[0]);
}

::capnp::StructSchema::Field getOptionalField(::capnp::StructSchema::Field field) {
    return field.getType().isStruct() ? getOptionalField(field.getType().asStruct()).value_or(field) : field;
}

std::optional<::capnp::StructSchema::Field> getOptionalNull(::capnp::StructSchema::Field field) {
    return field.getType().isStruct() ? getOptionalNull(field.getType().asStruct()) : std::nullopt;
}

::capnp::Type getNestedListElementType(::capnp::Type type) {
    auto elementType = type.asList().getElementType();
    return elementType.isList() ? getNestedListElementType(elementType) : elementType;
}

bool isNumeric(::capnp::Type type) {
    auto w = type.which();
    return isInteger(type)
        || w == ::capnp::schema::Type::FLOAT32
        || w == ::capnp::schema::Type::FLOAT64;
}

bool isInteger(::capnp::Type type) {
    auto w = type.which();
    return w == ::capnp::schema::Type::INT8
        || w == ::capnp::schema::Type::INT16
        || w == ::capnp::schema::Type::INT32
        || w == ::capnp::schema::Type::INT64
        || w == ::capnp::schema::Type::UINT8
        || w == ::capnp::schema::Type::UINT16
        || w == ::capnp::schema::Type::UINT32
        || w == ::capnp::schema::Type::UINT64;
}


/// Parse a type name from the capnp schema into its source file and name parts.
/// "messaging/capnp/test/test.capnp:Simple.choice" => [ "messaging/capnp/test/test.capnp", "Simple", "choice" ]
std::vector<std::string> parseCapnpTypeName(const std::string& name) {
    std::vector<std::string> parts;

    size_t pos = name.find(':');
    if (pos != std::string::npos) {
        parts.push_back(name.substr(0, pos));
    } else {
        throw std::runtime_error("source file not found in capnp display name: " + name);
    }

    while (true) {
        auto lpos = pos + 1;
        pos = name.find('.', lpos);
        if (pos != std::string::npos) {
            parts.push_back(name.substr(lpos, pos - lpos));
        } else {
            parts.push_back(name.substr(lpos));
            break;
        }
    }

    return parts;
}

std::string mergeCapnpTypeName(const std::vector<std::string>& parts, size_t n) {
    std::ostringstream ss;
    if (n == std::numeric_limits<size_t>::max()) {
        n = parts.size();
    }

    ss << parts[0];

    for (size_t i = 1; i < n; i++) {
        if (i < parts.size() - 1) {
            ss << (i > 1 ? "." : ":");
        }
        ss << parts[i];
    }

    return ss.str();
}
