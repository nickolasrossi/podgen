### podgen ###

##### What and why #####

`podgen` is a crucial tool that takes a capnproto schema definition (a `*.capnp` file) and
generates:
 
* an equivalent C++ POD (plain old data type as a struct) for each struct in the capnproto schema
* converter functions that copy the data both directions between a capnproto Builder/Reader and the POD

Why is this necessary? Dealing directly with capnproto Builders and Readers can be cumbersome,
especially for complex types. The underlying data is specially encoded inside a "message". 
That message may be a multi-segment buffer referring to a memory arena, an input or output stream, 
or even an mmapped file. Either way, a type instance is not easily moved around in a program
as a fundamental entity.

We found that we were writing a lot of PODs and converters by hand. This required a lot of work 
and maintenance, and resulted in divergent implementations. A code generator was needed.

`podgen` takes care of all this boilerplate. Application code can deal with simple PODs
and convert them to capnproto messages only when it's time to send the data somewhere.
Capnproto can then be the canonical, platform-independent representation of types in
our applications.

```cpp
    pkg::Command command;
    command.id = 99999;
    command.type = "stop";
    command.args.push_back("motors");
    command.args.push_back("cameras");
    command.args.push_back("sensors");

    ::capnp::MallocMessageBuilder msg;
    auto builder = msg.initRoot<pkg::capnp::Command>();
    
    // "podToCapnp" is a code-generated function
    podToCapnp(builder, command);
```

##### Capabilities #####

A more thorough list of what podgen handles:

* [capnproto structs](https://capnproto.org/language.html#structs) and their converter functions
* [capnproto enumerations](https://capnproto.org/language.html#enums) and their converter functions
* [capnproto constant](https://capnproto.org/language.html#constants) definitions, whether inside or outside of a struct
* [capnproto generic structs](https://capnproto.org/language.html#generic-types) as C++ templates
* [capnproto group](https://capnproto.org/language.html#groups) fields are generated as inner struct types
* [capnproto union](https://capnproto.org/language.html#unions) types are generated as `std::variant`, with a companion `which` 
  enumeration containing the union field names 
* capnproto union of two fields where one is `Void` is generated as `std::optional`
  (instead of a variant)
* [capnproto imports](https://capnproto.org/language.html#imports) (including another POD-generated schema)
* crazily [nested](https://capnproto.org/language.html#nesting-scope-and-aliases) structs/groups/unions
* struct equality and inequality operators
* struct hash computation with `std::hash` specialization 
* struct and enum to-string output functions for debug output (overloaded `operator<<`)
* enum-to-string converters
* map a capnproto type to an existing external type (rather than generating a POD and converters)
* customize the container type for any capnproto `List` field
* generate a field's default value with a piece of code instead of a constant
* generate an inheritance hierarchy between structs (capnproto does not support this natively for structs)
* [structure packing](http://www.catb.org/esr/structure-packing/) as an option
* `ignore` annotation to omit any element from generated code
* function signature `capnpTypeOf` that maps every pod type to its capnproto type (for use with `decltype`)

Some capnproto things that podgen does not handle:

* [interfaces](https://capnproto.org/language.html#interfaces)
* [AnyPointer](https://capnproto.org/language.html#dynamically-typed-fields)
* anonymous union nested inside another anonymous union


##### Header files #####

`podgen` generates two header files for every capnproto schema. For a schema named `x.capnp`, the headers are:

* `x.pod.hpp`: contains only the PODs with no dependency on capnproto
* `x.convert.hpp`: contains converter functions, which depends on capnproto

It is best practice to include just `*.pod.hpp` throughout an application, and 
include `*.convert.hpp` in source files only where pods need to be converted.

##### pod.hpp #####
`*.pod.hpp` contains the PODs along with the equality, hash and debug output functions.

The output functions are a combination of:
 
* `operator<<` overloads
* overloads of a function called `_out`

We rely on [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)
in the compiler to find the right output functions. (For some template or container types, this
can sometimes lead to conflicts if the container and its elements are in different namespaces.
I think these cases have been worked out, but please file a bug if this crops up.)

`operator<<` defers to `_out`. `_out` is where the output implementations live.

##### convert.hpp #####

Inside `*.convert.hpp`, you'll see sets of functions named `podFromCapnp`, `podToCapnp` and `capnpTypeOf`.
These function names are overloaded for all pod types. We rely on 
[argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl) to find the right function.

`podFromCapnp` converts a capnproto message into the equivalent pod. It takes a capnproto Reader and
returns a pod:

```cpp
    ::capnp::FlatArrayMessageReader msg;    // or any message reader
    pkg::Command command = podFromCapnp(msg.getRoot<pkg::capnp::Command>());
```

`podToCapnp` converts a pod to a capnproto message. It takes a capnproto Builder and the pod.

The capnproto Builder must already be initialized.

```cpp
    ::capnp::MallocMessageBuilder msg;      // or any message builder
    podToCapnp(msg.initRoot<pkg::capnp::Command>(), command);
```

`capnpTypeOf` is a function signature that maps a pod type to its capnproto type.

It is not an implemented function. It's meant to be used with `decltype` in template-based code:

```cpp
    template <typename T>
    void sendAnyPod(const T& pod) {
        using CapnpType = decltype(capnpTypeOf(std::declval<T>()));
        
        ::capnp::MallocMessageBuilder msg;
        podToCapnp(msg.initRoot<CapnpType>(), pod);
        send(msg);
    }
```

When using converters in code, header include order is sometimes important.
Any header-only code that references `podFromCapnp` or `podToCapnp` needs to be
included after the `*.convert.hpp` header, otherwise the code won't find those functions.

```cpp
    // include a blank line below the converter so clang-format doesn't reorder it
    #include "pkg.convert.hpp"
    
    #include "my_template_code.hpp"

```

##### Namespace selection #####

`podgen` generates a pod namespace from the declared capnproto namespace.

The rule is: 

* If the capnproto namespace ends with `::capnp`, it is stripped and the remainder becomes
the pod namespace. This is the preferred style.
* Otherwise, the pod namespace is the capnproto namespace plus `::pod`.

```
    using Cxx = import "/capnp/c++.capnp";
    $Cxx.namespace("ns::capnp");
```

becomes

```
    namespace ns {
```


##### Generics #####

Capnproto supports what they call [generics](https://capnproto.org/language.html#generic-types),
which maps to C++ templates.

`podgen` will generate template structs for capnproto generics, with a few limitations:

Capnproto allows only a struct, list, text, or data type as a template argument
(because these are represented as pointers). Primitive types like enums, ints and floats are not allowed.
  
`podgen` will not generate templatized converters, hash functions, or output functions. 
These will be generated only for types with fully specialized arguments.
This is necessary for a couple of reasons:

* Converter code will be different depending on which types are substituted in the
template. A field converter for a substituted struct will be different from a list, and so on.

* It allows overloaded functions such as `podFromCapnp` to continue with all types.
The overloads would not work if some cases required template arguments.

For example:

```cpp
    template <typename T>
    struct A {...};
    
    struct X {...};
    struct Y {...};
    struct Z {...};

    struct Command {
        A<X> ax;
        A<Y> ay;
        A<Z> az;
    };
    
    // there is no templatized converter for A<T>.
    // converters for all fully specialized versions of A are generated.
    A<X> podFromCapnp(pkg::capnp::A<pkg::capnp::X>::Reader);
    A<Y> podFromCapnp(pkg::capnp::A<pkg::capnp::Y>::Reader);
    A<Z> podFromCapnp(pkg::capnp::A<pkg::capnp::Z>::Reader);
```

##### Variant type #####

A capnproto union is turned into a `std::variant`.

An anonymous union becomes a stand-alone variant, while a union field becomes a variant
internal to the struct it's in.

```cpp
    # capnproto definition of an anonymous union:
    struct VariantType {
        union {
            none @0 : Void;
            a    @1 : Text;
            b    @2 : Int32;
        }
    }
    
    // generated code:
    using VariantType = std::variant<std::monostate, std::string, int32_t>;
    enum VariantType_which {
        VariantType_none,
        VariantType_a,
        VariantType_b
    };
```

##### Optional type #####

`podgen` has a special way of generating a `std::optional` type, which is useful to us
but not supported directly by capnproto.

A union with two fields, where one of the fields is a capnp `Void`, is converted to `std::optional`
instead of a variant.

It's a good idea to put the `Void` field first, so that this field in a newly created capnproto
message will have the `nullopt` value by default.

```cpp
    # capnproto definition:
    struct Command {
        id      @0 : UInt64;
        name    @1 : Text;
        arg : union {
            none    @2 : Void;
            value   @3 : Text;
        }
    }

    // generated code:
    struct Command {
        uint64_t id;
        std::string name;
        std::optional<std::string> arg;
    }
```


##### List containers #####

A capnproto `List(T)` is converted to a `std::vector<T>` by default.
This works for any type T.

It is possible to specify a different type of container other than `std::vector`.
Sometimes `std::unordered_set`, `std::list` or a custom container is more appropriate
in the data model.

This can be customized with an annotation.

See the examples in `test/test.capnp`.

##### External types #####

It is also possible to map an external type to a capnproto struct.

This is useful for mapping types from existing libraries into a capnproto message.

In this case, `podgen` will not generate a struct or converter functions for the type.
Instead, it will substitute the external type name wherever the capnproto type
is used to declare a field.

However, `podgen` _will_ generate function signatures for `podFromCapnp` and `podToCapnp`
for this type. You must implement these functions in a linked source file, otherwise
the converters for the containing struct will fail to compile.

See `test/test.capnp` for an example with `Eigen::Vector3d`.

##### Inheritance #####

Capnproto does not support inheritance with structs, but `podgen` will generate an
inheritance hierarchy if a few things are set up just right:

* A derived class must specify an annotation for each inherited base class.
* A derived class must exactly duplicate all fields from the base classes (order is not important).

The duplication of fields is a necessary evil, because capnproto needs to know
that each of these types have the same fields.

If all is defined correctly, `podgen` will omit the duplicate fields from the derived
class and inherit the derived class from the base classes.

See the example in `test/test.capnp`.
