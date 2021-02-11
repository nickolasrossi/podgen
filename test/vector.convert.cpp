#include <Eigen/Core>
#include "test.capnp.h"
#include "test.pod.hpp"

/**
 * Illustrates requirements for integrating an external type into a pod/capnp namespace.
 *
 * This example links Vector3d in test.capnp to eigen's Vector3d type.
 *
 * For an external type, it is necessary to define:
 * - a podFromCapnp converter
 * - a podToCapnp converter
 * - a std::hash
 * - an operator== equality function if one doesn't exist
 * - an output function
 *
 * The function signatures are already generated in the test.convert.hpp header,
 * but they are not implemented in test.convert.cpp. Implementations must be
 * provided in a separate cpp (shown here) and included in the pod library.
 *
 * The operator<< signature for an external type is NOT generated in the pod.hpp header,
 * since they often come with their own. One may be defined in the hdr="..." path
 * in the external annotation.
 */

/// Define converter for pod from capnp
Eigen::Vector3d podFromCapnp(::test::capnp::Vector3d::Reader r) {
    return Eigen::Vector3d(r.getX(), r.getY(), r.getZ());
}

/// Define converter for pod to capnp
void podToCapnp(::test::capnp::Vector3d::Builder b, const Eigen::Vector3d& obj) {
    b.setX(obj(0));
    b.setY(obj(1));
    b.setZ(obj(2));
}

/// Implement a meaningful hash function
std::size_t std::hash<Eigen::Vector3d>::operator()(const Eigen::Vector3d& o) const {
    std::size_t s = 0;
    s = ::podgen::hashCombine(s, o(0));
    s = ::podgen::hashCombine(s, o(1));
    s = ::podgen::hashCombine(s, o(2));
    return s;
};

namespace test {

/// Implement output function. This must be in the pod group's namespace.
void _out(std::ostream& os, const Eigen::Vector3d& o) {
    // do our own output for eigen vector, since eigen's operator<< writes newlines
    os << '[' << o(0) << ' ' << o(1) << ' ' << o(2) << ']';
}

}