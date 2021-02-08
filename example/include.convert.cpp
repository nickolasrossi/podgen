#include "include.convert.hpp"

::inc::Point podFromCapnp(::inc::capnp::Point::Reader r) {
    using namespace inc;
    ::inc::Point obj;
    obj.x = r.getX();
    obj.y = r.getY();
    obj.z = r.getZ();
    {
        auto ur = r.getFrame();
        if (ur.isName()) {
            obj.frame.emplace(_fromCapnp(ur.getName()));
        } else {
            obj.frame.reset();
        }
    }
    return obj;
}

void podToCapnp(::inc::capnp::Point::Builder b, const ::inc::Point& obj) {
    using namespace inc;
    b.setX(obj.x);
    b.setY(obj.y);
    b.setZ(obj.z);
    if (obj.frame) {
        b.initFrame().setName(_toCapnp(obj.frame.value()));
    } else {
        b.initFrame().setNull();
    }
}

::inc::Info podFromCapnp(::inc::capnp::Info::Reader r) {
    using namespace inc;
    ::inc::Info obj;
    if (r.hasInfo()) {
        obj.info = _fromCapnp(r.getInfo());
    }
    return obj;
}

void podToCapnp(::inc::capnp::Info::Builder b, const ::inc::Info& obj) {
    using namespace inc;
    b.setInfo(_toCapnp(obj.info));
}

