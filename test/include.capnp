@0x9e773be01926aace;

using Cxx = import "/c++.capnp";
$Cxx.namespace("inc::capnp");

struct Point {
    x   @0 : Float64;
    y   @1 : Float64;
    z   @2 : Float64;
    frame : union {
        name    @3 : Text;
        null    @4 : Void;
    }
}

struct Info {
    info    @0 : Text;
}