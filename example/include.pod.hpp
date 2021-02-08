#pragma once

#include <functional>
#include "defs.hpp"


namespace inc {

struct Point {
    static inline const std::string TypeName = "Point";

    struct _field {
        static inline const std::string x = "x";
        static inline const std::string y = "y";
        static inline const std::string z = "z";
        static inline const std::string frame = "frame";
    };

    double x = 0;
    double y = 0;
    double z = 0;
    std::optional<std::string> frame;

    bool operator==(const Point& o) const {
        return x == o.x
            && y == o.y
            && z == o.z
            && frame == o.frame;
    }

    bool operator!=(const Point& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Point& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.x);
            s = podgen::hashCombine(s, o.y);
            s = podgen::hashCombine(s, o.z);
            s = podgen::hashCombine(s, o.frame);
            return s;
        }

    };
};

struct Info {
    static inline const std::string TypeName = "Info";

    struct _field {
        static inline const std::string info = "info";
    };

    std::string info;

    bool operator==(const Info& o) const {
        return info == o.info;
    }

    bool operator!=(const Info& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Info& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.info);
            return s;
        }

    };
};


constexpr size_t MAX_CONTAINER_PRINT = 256;

/// default output function
template <typename T, typename std::enable_if<podgen::is_outputtable<T>::value, T>::type* = nullptr>
void _out(std::ostream& os, const T& o) {
    if constexpr (std::is_same<T, int8_t>::value) {
        os << (int16_t) o;
    } else if constexpr (std::is_same<T, uint8_t>::value) {
        os << (uint16_t) o;
    } else if constexpr (std::is_same<T, std::string>::value) {
        os << '"' << o << '"';
    } else {
        os << o;
    }
}

/// output monostate
inline void _out(std::ostream& os, const std::monostate& ms) {
    os << "void";
}

/// output Data type
inline void _out(std::ostream& os, const podgen::Data& data) {
    char buf[3];

    os << "(" << data.size() << ")[";

    size_t n = 0;
    for (auto& b : data) {
        if (n > 0) {
            os << ' ';
        }

        uint8_t c = static_cast<uint8_t>(b);
        sprintf(buf, "%02x", c);
        os << buf;

        if (n++ >= MAX_CONTAINER_PRINT) {
            os << " ..." << data.size() - n << " more";
            break;
        }
    }

    os << ']';
}

template <typename C,
        typename std::enable_if<podgen::is_container<C>::value, C>::type* = nullptr>
void _out(std::ostream& os, const C& c);

template <typename A, typename B>
void _out(std::ostream& os, const std::pair<A, B>& t);

template <typename... Args>
void _out(std::ostream& os, const std::variant<Args...>& v);

template <typename T>
void _out(std::ostream& os, const std::optional<T>& o);

template <typename T>
void _out_adl(std::ostream& os, const T& o);

inline void _out(std::ostream& os, const Point& o) {
    os << "Point{";
    os << "x=";
    _out(os, o.x);
    os << ' ';
    os << "y=";
    _out(os, o.y);
    os << ' ';
    os << "z=";
    _out(os, o.z);
    os << ' ';
    os << "frame=";
    inc::_out(os, o.frame);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Point& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Info& o) {
    os << "Info{";
    os << "info=";
    _out(os, o.info);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Info& o) {
    _out(os, o);
    return os;
}


/// output any container that doesn't already have an operator<<
template <typename C,
          typename std::enable_if<podgen::is_container<C>::value, C>::type*>
void _out(std::ostream& os, const C& c) {
    bool first = true;
    if (c.size() > 16) {
        os << '(' << c.size() << ')';
    }
    os << '[';
    size_t n = 0;

    for (const auto& o : c) {
        if (!first) {
            os << ", ";
        }

        if constexpr (std::experimental::is_detected<podgen::is_pair, decltype(o)>::value) {
            // special case for maps
            os << '{';
            inc::_out_adl(os, o.first);
            os << " -> ";
            inc::_out_adl(os, o.second);
            os << '}';
        } else {
            inc::_out_adl(os, o);
        }
        first = false;

        if (n++ >= MAX_CONTAINER_PRINT) {
            os << ", ..." << c.size() - n << " more";
            break;
        }
    }

    os << ']';
}

/// output any pair
template <typename A, typename B>
void _out(std::ostream& os, const std::pair<A, B>& t) {
    os << '{';
    inc::_out_adl(os, t.first);
    os << ", ";
    inc::_out_adl(os, t.second);
    os << '}';
}

/// output any variant
template <typename... Args>
void _out(std::ostream& os, const std::variant<Args...>& v) {
    std::visit([&os](const auto& e) -> void { inc::_out_adl(os, e); }, v);
}

template <typename T>
void _out(std::ostream& os, const std::optional<T>& o) {
    if (o) {
        inc::_out_adl(os, *o);
    } else {
        os << "{}";
    }
}

template <typename T>
void _out_adl(std::ostream& os, const T& o) {
    if constexpr (podgen::is_ambiguous_adl<T>::value) {
        inc::_out(os, o);
    } else {
        _out(os, o);
    }
}

}

namespace std {

template <> struct hash<::inc::Point> {
    std::size_t operator()(const ::inc::Point& o) const {
        return ::inc::Point::_Hasher()(o);
    }
};

template <> struct hash<::inc::Info> {
    std::size_t operator()(const ::inc::Info& o) const {
        return ::inc::Info::_Hasher()(o);
    }
};

}
