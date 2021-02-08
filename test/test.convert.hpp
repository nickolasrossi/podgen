#pragma once

#include <experimental/type_traits>
#include <capnp/schema.h>

#include "test.pod.hpp"
#include "test.capnp.h"
#include "include.convert.hpp"

::test::Name::Part::_pug::_pugu2 podFromCapnp(::test::capnp::Name::Part::Pu::Pug::Pug2::Pugu2::Reader r);
void podToCapnp(::test::capnp::Name::Part::Pu::Pug::Pug2::Pugu2::Builder b, const ::test::Name::Part::_pug::_pugu2& obj);
::test::capnp::Name::Part::Pu::Pug::Pug2::Pugu2 capnpTypeOf(::test::Name::Part::_pug::_pugu2);
::capnp::List<::test::capnp::Name::Part::Pu::Pug::Pug2::Pugu2, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::Part::_pug::_pugu2>);

::test::Name::Part::_pug podFromCapnp(::test::capnp::Name::Part::Pu::Pug::Reader r);
void podToCapnp(::test::capnp::Name::Part::Pu::Pug::Builder b, const ::test::Name::Part::_pug& obj);
::test::capnp::Name::Part::Pu::Pug capnpTypeOf(::test::Name::Part::_pug);
::capnp::List<::test::capnp::Name::Part::Pu::Pug, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::Part::_pug>);

::test::Name::Part podFromCapnp(::test::capnp::Name::Part::Reader r);
void podToCapnp(::test::capnp::Name::Part::Builder b, const ::test::Name::Part& obj);
::test::capnp::Name::Part capnpTypeOf(::test::Name::Part);
::capnp::List<::test::capnp::Name::Part, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::Part>);

::test::Name::_g1::_g2::_g3 podFromCapnp(::test::capnp::Name::G1::G2::G3::Reader r);
void podToCapnp(::test::capnp::Name::G1::G2::G3::Builder b, const ::test::Name::_g1::_g2::_g3& obj);
::test::capnp::Name::G1::G2::G3 capnpTypeOf(::test::Name::_g1::_g2::_g3);
::capnp::List<::test::capnp::Name::G1::G2::G3, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::_g1::_g2::_g3>);

::test::Name::_g1::_g2 podFromCapnp(::test::capnp::Name::G1::G2::Reader r);
void podToCapnp(::test::capnp::Name::G1::G2::Builder b, const ::test::Name::_g1::_g2& obj);
::test::capnp::Name::G1::G2 capnpTypeOf(::test::Name::_g1::_g2);
::capnp::List<::test::capnp::Name::G1::G2, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::_g1::_g2>);

::test::Name::_g1 podFromCapnp(::test::capnp::Name::G1::Reader r);
void podToCapnp(::test::capnp::Name::G1::Builder b, const ::test::Name::_g1& obj);
::test::capnp::Name::G1 capnpTypeOf(::test::Name::_g1);
::capnp::List<::test::capnp::Name::G1, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::_g1>);

::test::Name::_gu::_vc podFromCapnp(::test::capnp::Name::U::Gu::Gu3::Vc::Reader r);
void podToCapnp(::test::capnp::Name::U::Gu::Gu3::Vc::Builder b, const ::test::Name::_gu::_vc& obj);
::test::capnp::Name::U::Gu::Gu3::Vc capnpTypeOf(::test::Name::_gu::_vc);
::capnp::List<::test::capnp::Name::U::Gu::Gu3::Vc, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::_gu::_vc>);

::test::Name::_gu podFromCapnp(::test::capnp::Name::U::Gu::Reader r);
void podToCapnp(::test::capnp::Name::U::Gu::Builder b, const ::test::Name::_gu& obj);
::test::capnp::Name::U::Gu capnpTypeOf(::test::Name::_gu);
::capnp::List<::test::capnp::Name::U::Gu, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name::_gu>);

::test::Name podFromCapnp(::test::capnp::Name::Reader r);
void podToCapnp(::test::capnp::Name::Builder b, const ::test::Name& obj);
::test::capnp::Name capnpTypeOf(::test::Name);
::capnp::List<::test::capnp::Name, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Name>);

inline auto podFromCapnp(::test::capnp::Choice e) { return static_cast<::test::Choice>(e); }
inline auto podToCapnp(::test::Choice e) { return static_cast<::test::capnp::Choice>(e); }
::test::capnp::Choice capnpTypeOf(::test::Choice);
::capnp::List<::test::capnp::Choice, ::capnp::Kind::ENUM> capnpTypeOf(std::vector<::test::Choice>);

::test::Simple::_gg podFromCapnp(::test::capnp::Simple::Gg::Reader r);
void podToCapnp(::test::capnp::Simple::Gg::Builder b, const ::test::Simple::_gg& obj);
::test::capnp::Simple::Gg capnpTypeOf(::test::Simple::_gg);
::capnp::List<::test::capnp::Simple::Gg, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Simple::_gg>);

::test::Simple podFromCapnp(::test::capnp::Simple::Reader r);
void podToCapnp(::test::capnp::Simple::Builder b, const ::test::Simple& obj);
::test::capnp::Simple capnpTypeOf(::test::Simple);
::capnp::List<::test::capnp::Simple, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Simple>);

::test::Variant podFromCapnp(::test::capnp::Variant::Reader r);
void podToCapnp(::test::capnp::Variant::Builder b, const ::test::Variant& obj);
::test::capnp::Variant capnpTypeOf(::test::Variant);
::capnp::List<::test::capnp::Variant, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Variant>);

::test::MaybeText podFromCapnp(::test::capnp::MaybeText::Reader r);
void podToCapnp(::test::capnp::MaybeText::Builder b, const ::test::MaybeText& obj);
::test::capnp::MaybeText capnpTypeOf(::test::MaybeText);
::capnp::List<::test::capnp::MaybeText, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::MaybeText>);

::test::Container podFromCapnp(::test::capnp::Container::Reader r);
void podToCapnp(::test::capnp::Container::Builder b, const ::test::Container& obj);
::test::capnp::Container capnpTypeOf(::test::Container);
::capnp::List<::test::capnp::Container, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Container>);

::test::Lists podFromCapnp(::test::capnp::Lists::Reader r);
void podToCapnp(::test::capnp::Lists::Builder b, const ::test::Lists& obj);
::test::capnp::Lists capnpTypeOf(::test::Lists);
::capnp::List<::test::capnp::Lists, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Lists>);

::test::Base podFromCapnp(::test::capnp::Base::Reader r);
void podToCapnp(::test::capnp::Base::Builder b, const ::test::Base& obj);
::test::capnp::Base capnpTypeOf(::test::Base);
::capnp::List<::test::capnp::Base, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Base>);

::test::Titled podFromCapnp(::test::capnp::Titled::Reader r);
void podToCapnp(::test::capnp::Titled::Builder b, const ::test::Titled& obj);
::test::capnp::Titled capnpTypeOf(::test::Titled);
::capnp::List<::test::capnp::Titled, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Titled>);

::test::Entity podFromCapnp(::test::capnp::Entity::Reader r);
void podToCapnp(::test::capnp::Entity::Builder b, const ::test::Entity& obj);
::test::capnp::Entity capnpTypeOf(::test::Entity);
::capnp::List<::test::capnp::Entity, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Entity>);

::test::Empty podFromCapnp(::test::capnp::Empty::Reader r);
void podToCapnp(::test::capnp::Empty::Builder b, const ::test::Empty& obj);
::test::capnp::Empty capnpTypeOf(::test::Empty);
::capnp::List<::test::capnp::Empty, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Empty>);

::test::Sparse podFromCapnp(::test::capnp::Sparse::Reader r);
void podToCapnp(::test::capnp::Sparse::Builder b, const ::test::Sparse& obj);
::test::capnp::Sparse capnpTypeOf(::test::Sparse);
::capnp::List<::test::capnp::Sparse, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Sparse>);

::test::User podFromCapnp(::test::capnp::User::Reader r);
void podToCapnp(::test::capnp::User::Builder b, const ::test::User& obj);
::test::capnp::User capnpTypeOf(::test::User);
::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::User>);

::test::Account podFromCapnp(::test::capnp::Account::Reader r);
void podToCapnp(::test::capnp::Account::Builder b, const ::test::Account& obj);
::test::capnp::Account capnpTypeOf(::test::Account);
::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Account>);

::test::TextError podFromCapnp(::test::capnp::TextError::Reader r);
void podToCapnp(::test::capnp::TextError::Builder b, const ::test::TextError& obj);
::test::capnp::TextError capnpTypeOf(::test::TextError);
::capnp::List<::test::capnp::TextError, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::TextError>);

::test::IntError podFromCapnp(::test::capnp::IntError::Reader r);
void podToCapnp(::test::capnp::IntError::Builder b, const ::test::IntError& obj);
::test::capnp::IntError capnpTypeOf(::test::IntError);
::capnp::List<::test::capnp::IntError, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::IntError>);

::test::Get<::test::User, std::vector<::test::User>> podFromCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Reader r);
void podToCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Builder b, const ::test::Get<::test::User, std::vector<::test::User>>& obj);
::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>> capnpTypeOf(::test::Get<::test::User, std::vector<::test::User>>);
::capnp::List<::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Get<::test::User, std::vector<::test::User>>>);

::test::Get<::test::User, std::vector<::test::User>>::Result podFromCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Result::Reader r);
void podToCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Result::Builder b, const ::test::Get<::test::User, std::vector<::test::User>>::Result& obj);
::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Result capnpTypeOf(::test::Get<::test::User, std::vector<::test::User>>::Result);
::capnp::List<::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Result, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Get<::test::User, std::vector<::test::User>>::Result>);

::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError> podFromCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Error<::test::capnp::IntError>::Reader r);
void podToCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Error<::test::capnp::IntError>::Builder b, const ::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>& obj);
::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Error<::test::capnp::IntError> capnpTypeOf(::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>);
::capnp::List<::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Error<::test::capnp::IntError>, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>>);

::test::Get<::test::Account, std::vector<::test::Account>> podFromCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Reader r);
void podToCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Builder b, const ::test::Get<::test::Account, std::vector<::test::Account>>& obj);
::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>> capnpTypeOf(::test::Get<::test::Account, std::vector<::test::Account>>);
::capnp::List<::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Get<::test::Account, std::vector<::test::Account>>>);

::test::Get<::test::Account, std::vector<::test::Account>>::Result podFromCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Result::Reader r);
void podToCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Result::Builder b, const ::test::Get<::test::Account, std::vector<::test::Account>>::Result& obj);
::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Result capnpTypeOf(::test::Get<::test::Account, std::vector<::test::Account>>::Result);
::capnp::List<::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Result, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Get<::test::Account, std::vector<::test::Account>>::Result>);

::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError> podFromCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Error<::test::capnp::TextError>::Reader r);
void podToCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Error<::test::capnp::TextError>::Builder b, const ::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>& obj);
::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Error<::test::capnp::TextError> capnpTypeOf(::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>);
::capnp::List<::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Error<::test::capnp::TextError>, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>>);

::test::GetInterface podFromCapnp(::test::capnp::GetInterface::Reader r);
void podToCapnp(::test::capnp::GetInterface::Builder b, const ::test::GetInterface& obj);
::test::capnp::GetInterface capnpTypeOf(::test::GetInterface);
::capnp::List<::test::capnp::GetInterface, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::GetInterface>);

::test::AltGet<::test::User, std::vector<::test::User>> podFromCapnp(::test::capnp::AltGet<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Reader r);
void podToCapnp(::test::capnp::AltGet<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Builder b, const ::test::AltGet<::test::User, std::vector<::test::User>>& obj);
::test::capnp::AltGet<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>> capnpTypeOf(::test::AltGet<::test::User, std::vector<::test::User>>);
::capnp::List<::test::capnp::AltGet<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::AltGet<::test::User, std::vector<::test::User>>>);

::test::AltInterface podFromCapnp(::test::capnp::AltInterface::Reader r);
void podToCapnp(::test::capnp::AltInterface::Builder b, const ::test::AltInterface& obj);
::test::capnp::AltInterface capnpTypeOf(::test::AltInterface);
::capnp::List<::test::capnp::AltInterface, ::capnp::Kind::STRUCT> capnpTypeOf(std::vector<::test::AltInterface>);

namespace test {

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
