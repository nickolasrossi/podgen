@0xa5183219d53683b5;

struct ExternalType $ignore {
    type    @0 : Text;          # fully qualified external pod type name
    hdr     @1 : Text = "";     # header where the pod is found
}

struct Converter $ignore {
    type    @0 : Text;          # fully qualified external type name
    hdr     @1 : Text = "";     # header where the type is found
    from    @2 : Text;          # function to convert from type to field type
    to      @3 : Text;          # function to convert to type from field value
    out     @4 : Text;          # output function
    default @5 : Text = "";     # default value for the type
}

struct DefaultValue $ignore {
    value   @0 : Text;          # default value expression
    hdr     @1 : Text = "";     # any header required for the expression
}

# specify an external pod type for a struct
annotation external @0x97eb12f93bdd7cd3 (struct): ExternalType;

# specify an external type and converters for a field
annotation converter @0xa5a4d17a95451341 (field) : Converter;

# specify an alternate container for a list field
annotation container @0xaf26f446007ee38a (field): ExternalType;

# inherit other structs in the pod output if all their fields are present
annotation inherit @0x8955e2fd9635a433 (struct): List(Text);

# apply a default value expression to a field
annotation defaultValue @0xac6b15d7e51cadd7 (field) : DefaultValue;

# perform structure packing
annotation pack @0xea261d3e58c48d13 (struct): Void;

# don't generate a hash function for a struct
annotation nohash @0xa13d849658a9f9ac (struct): Void;

# ignore a struct, field, enum, enum value, whatever (don't generate code for it)
annotation ignore @0xc8e0b11182f5194a (*): Void;


# unordered set of c++ native type
const unorderedSet : ExternalType = (type="std::unordered_set<T>", hdr="<unordered_set>");

# unordered set of a capnp type
const unorderedSetHash : ExternalType = (type="std::unordered_set<T, T::_Hasher>", hdr="<unordered_set>");

# ordered set of c++ native type
const orderedSet : ExternalType = (type="std::set<T>", hdr="<set>");

# ordered set of a capnp type
const orderedSetHash : ExternalType = (type="std::set<T, T::_Hasher>", hdr="<set>");

# converter for timestamp (apply to UInt64 field)
const timestamp : Converter = (
        type = "podgen::Timestamp",
        hdr="timestamp.convert.hpp",
        from="podgen::timestampFromNanos",
        to="podgen::timestampToNanos",
        out="podgen::outTimestamp",
        default="podgen::timestamp_clock::now()"
) $ignore;
