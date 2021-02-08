#pragma once

#include <unistd.h>
#include <sys/stat.h>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <optional>

#include <cctz/time_zone.h>

/**
 * pod field converter for storing a chrono timestamp in a capnp uint64 as nanoseconds.
 */

namespace podgen {

using timestamp_clock = std::chrono::system_clock;
using Timestamp = std::chrono::time_point<timestamp_clock>;

/// Some ugliness to find the system time zone.
/// It's either the contents of /etc/timezone, or a symlink from /etc/timezone into /usr/share/zoneinfo.
inline cctz::time_zone getSystemTimeZone() {
    static const char* tzfile = "/etc/timezone";
    cctz::time_zone tz;
    struct stat st;

    if (lstat(tzfile, &st) == 0 && S_ISLNK(st.st_mode)) {
        char buf[64];
        auto n = readlink(tzfile, buf, sizeof(buf));

        bool foundOne = false;
        for (auto i = n - 1; i >= 0; i--) {
            if (buf[i] == '/') {
                if (foundOne) {
                    cctz::load_time_zone(&buf[i+1], &tz);
                } else {
                    foundOne = true;
                }
            }
        }
    } else {
        std::ifstream is("/etc/timezone");
        if (is.good()) {
            std::string tzname;
            is >> std::ws >> tzname;
            cctz::load_time_zone(tzname, &tz);
        }
    }

    return tz;
}

inline Timestamp timestampFromNanos(uint64_t value) {
    // system clock doesn't construct from nanoseconds
    std::chrono::microseconds dur(value / 1000 + ((value % 1000) >= 500 ? 1 : 0));
    return Timestamp(dur);
}

inline uint64_t timestampToNanos(const Timestamp& timestamp) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp.time_since_epoch()).count();
}

inline void outTimestamp(std::ostream& os, const Timestamp& o) {
    static const auto tz = getSystemTimeZone();

    os << '[';
    os << cctz::format("%Y-%m-%d %H:%M:%S", o, tz);

    auto nanos = timestampToNanos(o);
    static const uint64_t NS_PER_SEC = 1000000000L;
    auto ns = nanos % NS_PER_SEC;

    std::string us = "000000" + std::to_string(ns / 1000);
    os << "." << us.substr(us.size() - 6) << cctz::format(" %z", o, tz) << ']';
}

/// Include an std::optional output function in case the field is declared as an optional.
/// (ADL cannot find an _out function for the Timestamp type since it is in the std namespace.)
inline void outTimestamp(std::ostream& os, const std::optional<Timestamp>& o) {
    if (o) {
        outTimestamp(os, *o);
    } else {
        os << "{}";
    }
}

}


namespace std {

template <> struct hash<podgen::Timestamp> {
    std::size_t operator()(const podgen::Timestamp& o) const {
        return podgen::timestampToNanos(o);
    }
};

}
