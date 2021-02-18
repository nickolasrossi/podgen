@0x9e773be01926aacd;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("test::capnp");

using Include = import "include.capnp";

using Pod = import "/pod.capnp";

const whatever : Text = "whatever";

struct Name {
    id      @0  : UInt16;
    name    @1  : Text;

    # testing some nested craziness
    g1 : group {
        n1 @2 : Text;
        g2 : group {
            n2 @3 : Text;
            g3 : group {
                n3 @4 : Text;
            }
        }
    }

    # testing more nested craziness
    u : union {
        au @5 : Text;
        bu @6 : Text;
        gu : group {
            gu1 @7 : Text;
            gu2 @8 : Text;
            gu3 : union {
                va @9 : Text;
                vb @10 : Text;
                vc : group {
                    vcg1 @11 : Text;
                    vcg2 @12 : Text;
                }
            }
        }
    }

    # testing insane nested craziness
    struct Part {
        pa @0 : Text;
        pb @1 : Text;
        pu : union {
            pu1 @2 : List(Text);
            pu2 @3 : Text;
            pug : group {
                pug1 @4 : Text;
                pug2 : union {
                    pugu1 @5 : Text;
                    pugu2 : group {
                        pugg @6 : Text;
                    }
                }
            }
        }
    }
}

enum Choice
{
    a       @0;
    c       @1;
    b       @2;
    new     @3;
    camelCase @4;
    ignored @5 $Pod.ignore;
}

struct Simple
{
    const band : Text = "minds";
    const answer : UInt16 = 42;
    const choice : Choice = b;

    bool    @0  : Bool;
    int8    @1  : Int8 = -4;
    int16   @2  : Int16;
    int32   @3  : Int32;
    int64   @4  : Int64;
    uint8   @5  : UInt8 = 22;
    uint16  @6  : UInt16;
    uint32  @7  : UInt32 = 211;
    uint64  @8  : UInt64;
    float   @9  : Float32 = 1.1;
    double  @10 : Float64 = 0.0000000053;
    string  @11 : Text;
    data    @12 : Data = 0x"41 42 43 44";
    enumer  @13 : Choice = camelCase;
    list    @14 : List(Text);
    obj     @15 : Name;
    objlist @16 : List(Name);
    llist   @17 : List(List(Int8));
    lnlist  @18 : List(List(Name));
    elist   @19 : List(Choice);
    ilist   @20 : List(UInt64);

    gg : group {
        str   @21 : Text;
        float @22 : Float32;
    }

    opt         : union {
        null  @23 : Void;
        value @24 : Text;
    }

    un          : union {
        unBool  @25  : Bool;
        unInt   @26  : Int32;
        unUint  @27  : UInt32;
        unString @28 : Text;
        unList  @29  : List(Int8);
        unEnum  @30  : Choice;
        unData  @31  : Data;
        unObj   @32  : Name;
        unObjlist @33 : List(Name);
    }

    vn          : union {
        vInt    @34  : Int32;
        voyd    @35  : Void;
        unBool  @36  : Bool;
    }

    string2     @37  : Text $Pod.ignore;
}

# turns into a standalone variant in the pod header
struct Variant {
    union {
        string @0 : Text;
        int16  @1 : Int16;
        data   @2 : Data;
        choice @3 : Choice;
    }
}

# turns into a standalone optional
struct MaybeText {
    union {
        value @0 : List(Text);
        null  @1 : Void;
    }
}

# referencing the standalone variant and optional in another pod
struct Container {
    v   @0 : Variant;
    t   @1 : MaybeText;
}

# external type example
struct Vector3d $Pod.external(type="Eigen::Vector3d", hdr="Eigen/Core") {
    x   @0 : Float64;
    y   @1 : Float64;
    z   @2 : Float64;
}

# demonstrates including an external type in another pod
struct Pose {
    nanos           @0 : UInt64 $Pod.defaultValue(value="podgen::timestampToNanos(std::chrono::system_clock::now())", hdr="<chrono>");
    timestamp       @1 : UInt64 $Pod.converter(Pod.timestamp);
    translation     @2 : Vector3d;
    rotation        @3 : Vector3d;
    point           @4 : Include.Point;
    times : group {
        start       @5 : UInt64 $Pod.converter(Pod.timestamp);
        end         @6 : UInt64 $Pod.converter(Pod.timestamp);
    }
}

# demonstrates custom container types for capnp lists.
# T, K, V in the type templates are specially recognized:
# - if T is present, the type name inside List(*) is converted to pod and substituted in the template
# - if K is present, substitutes the pod type of a field named "key" on the List element type
# - if V is present, substitutes the pod type of a field named "value" on the List element type
#
# Use of T, K, V are not required. The type can be specified fully explicit as well.
struct Lists {
    struct Entry $Pod.ignore {
        key     @0 : Text;
        value   @1 : UInt64;
    }

    timestamp       @0 : UInt64;
    list            @1 : List(Text);
    nlist           @2 : List(Name) $Pod.container(type="std::unordered_set<T, T::_Hasher>", hdr="<unordered_set>");
    rlist           @3 : List(Name) $Pod.container(type="std::list<T>", hdr="<list>");
    sset            @4 : List(Text) $Pod.container(type="std::set<T>", hdr="<set>");
    map             @5 : List(Entry) $Pod.container(type="std::unordered_map<K, V>", hdr="<unordered_map>");
    listoflists     @6 : List(List(Text)) $Pod.container(type="std::list<std::list<T>>", hdr="<list>");
    empty           @7 : List(Text);
}


struct Base {
    id          @0 : UInt64;
    owner       @1 : Text;
    created     @2 : UInt64;
    createdBy   @3 : Text;
    modified    @4 : UInt64;
    modifiedBy  @5 : Text;
}

struct Titled {
    title       @0 : Text;
    description @1 : Text;
}

struct Entity $Pod.inherit(["Base", "Titled", "Include.Info"]) {
    id          @0 : UInt64;    # inherited fields must be duplicated with same type
    owner       @1 : Text;
    created     @2 : UInt64;
    createdBy   @3 : Text;
    modified    @4 : UInt64;
    modifiedBy  @5 : Text;

    title       @6 : Text;
    description @7 : Text;
    info        @8 : Text;

    location    @9 : Text;
}

# make sure there are no problems with an empty struct
struct Empty {
}

struct Sparse {
    id          @0 : UInt64;
    name : union {
        null    @1 : Void;
        value   @2 : Text;
    }
}


# generics

struct Get(T, U) {
    getOne  @0 : T;
    getAll  @1 : U;

    struct Result {
        msg     @0 : Text;
    }

    struct Error(E) {
        error   @0 : E;
    }
}

struct User {
    id      @0 : UInt64;
    email   @1 : Text;
}

struct Account {
    id      @0 : UInt64;
    name    @1 : Text;
}

struct TextError {
    msg     @0 : Text;
}

struct IntError {
    code    @0 : UInt16;
}

struct GetInterface {
    getUser         @0 : Get(User, List(User));
    userResult      @1 : Get(User, List(User)).Result;
    userError       @2 : Get(User, List(User)).Error(IntError);
    getAccount      @3 : Get(Account, List(Account));
    accountResult   @4 : Get(Account, List(Account)).Result;
    accountError    @5 : Get(Account, List(Account)).Error(TextError);
}

struct AltGet(V, W) {
    get     @0 : Get(V, W);
}

struct AltInterface {
    user    @0 : AltGet(User, List(User));
}