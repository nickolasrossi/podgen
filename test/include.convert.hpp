#pragma once

#include <experimental/type_traits>
#include <capnp/schema.h>

#include "include.pod.hpp"
#include "include.capnp.h"

::inc::Point podFromCapnp(::inc::capnp::Point::Reader r);
void podToCapnp(::inc::capnp::Point::Builder b, const ::inc::Point& obj);
::inc::capnp::Point capnpTypeOf(::inc::Point);
::capnp::List<::inc::capnp::Point, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::inc::Point>);

::inc::Info podFromCapnp(::inc::capnp::Info::Reader r);
void podToCapnp(::inc::capnp::Info::Builder b, const ::inc::Info& obj);
::inc::capnp::Info capnpTypeOf(::inc::Info);
::capnp::List<::inc::capnp::Info, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::inc::Info>);

namespace inc {

// from

template <typename CT>
auto _fromCapnp(CT v) {
    if constexpr (std::is_same<CT, ::capnp::Void>::value) {
        return std::monostate();
    } else if constexpr (std::is_arithmetic<CT>::value) {
        return v;
    } else if constexpr (std::is_same<CT, ::capnp::Text::Reader>::value) {
        return static_cast<std::string>(v);
    } else if constexpr (std::is_same<CT, ::capnp::Data::Reader>::value) {
        auto bytes = v.asBytes();
        auto begin = reinterpret_cast<const podgen::Data::value_type*>(bytes.begin());
        return podgen::Data(begin, begin + bytes.size());
    } else if constexpr (std::experimental::is_detected<podgen::is_iterable, CT>::value) {
        using CE = decltype(std::declval<CT>().begin().operator*());
        using E = decltype(_fromCapnp(std::declval<CE>()));
        std::vector<E> out;
        out.reserve(v.size());
        for (auto&& ce : v) {
            out.push_back(_fromCapnp(ce));
        }
        return out;
    } else {
        return podFromCapnp(v);
    }
}

template <typename T, typename CL>
auto _fromCapnpList(CL list) {
    using CE = decltype(std::declval<CL>().begin().operator*());
    using E = typename std::decay<decltype(std::declval<T>().begin().operator*())>::type;

    T out;

    for (auto&& ce : list) {
        if constexpr (std::is_same<CE, ::capnp::Text::Reader>::value) {
            out.insert(out.end(), _fromCapnp(ce));
        } else if constexpr (std::is_same<CE, ::capnp::Data::Reader>::value) {
            out.insert(out.end(), _fromCapnp(ce));
        } else if constexpr (std::experimental::is_detected<podgen::is_iterable, CE>::value) {
            out.insert(out.end(), _fromCapnpList<E>(ce));
        } else if constexpr (std::experimental::is_detected<podgen::is_pair, E>::value) {
            out.insert(out.end(), std::make_pair(_fromCapnp(ce.getKey()), _fromCapnp(ce.getValue())));
        } else {
            out.insert(out.end(), _fromCapnp(ce));
        }
    }

    return out;
}


// to

template <typename T>
auto _toCapnp(T v) {
    if constexpr (std::is_arithmetic<T>::value) {
        return v;
    } else if constexpr (std::is_enum<T>::value) {
        return podToCapnp(v);
    } else if constexpr (std::is_same<T, std::string>::value) {
        return ::capnp::Text::Reader(v);
    } else if constexpr (std::is_same<T, podgen::Data>::value) {
        return ::capnp::Data::Reader(reinterpret_cast<const uint8_t*>(v.data()), v.size());
    } else {
        return;
    }
}

template <typename B, typename T>
void _toCapnpList(B builder, const T& container) {
    using E = typename std::decay<decltype(std::declval<T>().begin().operator*())>::type;

    if (builder.size() != container.size()) {
        throw std::invalid_argument("capnp list builder must be initialized to source vector size");
    }

    uint i = 0;
    for (const auto& e : container) {
        if constexpr (!std::is_same<void, decltype(_toCapnp(std::declval<E>()))>::value) {
            builder.set(i, _toCapnp(e));
        } else if constexpr (std::experimental::is_detected<podgen::is_pair, E>::value) {
            builder[i].setKey(_toCapnp(e.first));
            builder[i].setValue(_toCapnp(e.second));
        } else if constexpr (std::experimental::is_detected<podgen::is_iterable, E>::value) {
            auto b = builder.init(i, e.size());
            _toCapnpList(b, e);
        } else {
            podToCapnp(builder[i], e);
        }

        i++;
    }
}

}
