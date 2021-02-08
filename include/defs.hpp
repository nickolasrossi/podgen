#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <iostream>
#include <experimental/type_traits>

namespace podgen {

/// alias for our plain version of the capnp Data (blob) type
using Data = std::vector<std::byte>;

/// for identifying types that can iterate with a for-range
template <typename T>
using is_iterable = decltype(std::declval<T>().begin());

/// for identifying a pair
template <typename T>
using is_pair = decltype(std::declval<T>().first);

/// for identifying an optional
template <typename T>
using is_optional = decltype(std::declval<T>().has_value());

/// for identifying a variant
template <typename T>
using is_variant = decltype(std::declval<T>().valueless_by_exception());

/// for identifying types that have a << operator defined
template <typename T>
using is_direct_output = decltype(std::declval<std::ostream&>().operator<<(std::declval<T>()));

template <typename T>
using is_indirect_output = decltype(operator<<(std::declval<std::ostream&>(), std::declval<T>()));

template <typename T>
using is_outputtable =
    typename std::integral_constant<bool, std::experimental::is_detected<is_direct_output, T>::value ||
                                              std::experimental::is_detected<is_indirect_output, T>::value>;

/// container is iterable but doesn't have an output defined (excludes std::string)
template <typename T>
using is_container =
    typename std::integral_constant<bool, std::experimental::is_detected<is_iterable, T>::value &&
                                              !is_outputtable<T>::value>;

/// whether an ADL function on this type requires a namespace qualifier
template <typename T>
using is_ambiguous_adl =
    typename std::integral_constant<bool, std::experimental::is_detected<is_optional, T>::value ||
                                              std::experimental::is_detected<is_variant, T>::value ||
                                              std::experimental::is_detected<is_pair, T>::value ||
                                              is_container<T>::value>;

template <typename T>
using has_std_hash = decltype(std::declval<std::hash<T>>().operator()(std::declval<const T&>()));

template <typename T>
using has_hasher = decltype(std::declval<T::_Hasher>().operator()(std::declval<const T&>()));

template <typename T>
using has_hasher = decltype(std::declval<T::_Hasher>().operator()(std::declval<const T&>()));

/// Convert enum value to string name
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
auto enumToName(T e, const std::vector<std::string>& names) {
    return names[static_cast<uint16_t>(e)];
}

/// Convert string name to enum value
template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
std::optional<T> enumFromName(const std::string_view& name, const std::vector<std::string>& names) {
    auto it = std::find(names.begin(), names.end(), name);
    if (it != names.end()) {
        return static_cast<T>(std::distance(names.begin(), it));
    } else {
        return {};
    }
}

/// Universal hash function for objects with a podgen _Hasher or std::hash specialization
template <typename T>
std::size_t hash(const T& obj) {
    if constexpr (std::experimental::is_detected<has_hasher, T>::value) {
        return typename T::_Hasher()(obj);
    } else {
        return std::hash<T>()(obj);
    }
}

template <typename T>
std::size_t hashCombine(std::size_t s, const std::optional<T>& obj);

template <typename... Types>
std::size_t hashCombine(std::size_t s, const std::variant<Types...>& obj);

template <typename A, typename B>
std::size_t hashCombine(std::size_t s, const std::pair<A, B>& obj);

template <typename... Args>
std::size_t hashCombine(std::size_t s, const std::tuple<Args...>& obj);

template <typename T, typename... Args>
std::size_t hashCombine(std::size_t s, const T& obj, Args&&... args);

/// Universal hash combine function
template <typename T>
std::size_t hashCombine(std::size_t s, const T& obj) {
    if constexpr (std::experimental::is_detected<is_iterable, T>::value &&
                  !std::experimental::is_detected<has_std_hash, T>::value &&
                  !std::experimental::is_detected<has_hasher, T>::value) {
        for (const auto& o : obj) {
            s = hashCombine(s, o);
        }
        return s;
    } else {
        return s ^ hash(obj) + 0x9e3779b9 + (s << 6) + (s >> 2);  // from boost::hash_combine
    }
}

/// Hash combine arbitrary varargs
template <typename T, typename... Args>
std::size_t hashCombine(std::size_t s, const T& obj, Args&&... args) {
    return hashCombine(hashCombine(s, obj), args...);
}

/// Hash arbitrary varargs together
template <typename T, typename... Args>
std::size_t hash(const T& obj, Args&&... args) {
    return hashCombine(0, obj, args...);
}

/// Hash combine an optional
template <typename T>
std::size_t hashCombine(std::size_t s, const std::optional<T>& obj) {
    return obj.has_value() ? hashCombine(s, *obj) : hashCombine(s, 0);
}

/// Hash combine a variant
template <typename... Types>
std::size_t hashCombine(std::size_t s, const std::variant<Types...>& obj) {
    return std::visit([s](auto&& arg) { return hashCombine(s, arg); }, obj);
}

/// Hash combine a pair
template <typename A, typename B>
std::size_t hashCombine(std::size_t s, const std::pair<A, B>& obj) {
    return hashCombine(s, obj.first, obj.second);
}

/// Hash combine a tuple
template <typename... Args>
std::size_t hashCombine(std::size_t s, const std::tuple<Args...>& obj) {
    return std::apply([s](auto&&... args) { return (hashCombine(s, args), ...); }, obj);
}

}  // namespace podgen
