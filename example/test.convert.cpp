#include "test.convert.hpp"

::test::Name::Part::_pug::_pugu2 podFromCapnp(::test::capnp::Name::Part::Pu::Pug::Pug2::Pugu2::Reader r) {
    using namespace test;
    ::test::Name::Part::_pug::_pugu2 obj;
    if (r.hasPugg()) {
        obj.pugg = _fromCapnp(r.getPugg());
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::Part::Pu::Pug::Pug2::Pugu2::Builder b, const ::test::Name::Part::_pug::_pugu2& obj) {
    using namespace test;
    b.setPugg(_toCapnp(obj.pugg));
}

::test::Name::Part::_pug podFromCapnp(::test::capnp::Name::Part::Pu::Pug::Reader r) {
    using namespace test;
    ::test::Name::Part::_pug obj;
    if (r.hasPug1()) {
        obj.pug1 = _fromCapnp(r.getPug1());
    }
    {
        auto ur = r.getPug2();
        if (ur.isPugu1()) {
            obj.pug2.emplace<0>(_fromCapnp(ur.getPugu1()));
        }
        if (ur.isPugu2()) {
            obj.pug2.emplace<1>(_fromCapnp(ur.getPugu2()));
        }
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::Part::Pu::Pug::Builder b, const ::test::Name::Part::_pug& obj) {
    using namespace test;
    b.setPug1(_toCapnp(obj.pug1));
    switch (obj.pug2.index()) {
        case 0: {
                        b.initPug2().setPugu1(_toCapnp(std::get<0>(obj.pug2)));
            break;
        }
        case 1: {
            podToCapnp(b.initPug2().initPugu2(), std::get<1>(obj.pug2));
            break;
        }
    }
}

::test::Name::Part podFromCapnp(::test::capnp::Name::Part::Reader r) {
    using namespace test;
    ::test::Name::Part obj;
    if (r.hasPa()) {
        obj.pa = _fromCapnp(r.getPa());
    }
    if (r.hasPb()) {
        obj.pb = _fromCapnp(r.getPb());
    }
    {
        auto ur = r.getPu();
        if (ur.isPu1()) {
            obj.pu.emplace<0>(_fromCapnp(ur.getPu1()));
        }
        if (ur.isPu2()) {
            obj.pu.emplace<1>(_fromCapnp(ur.getPu2()));
        }
        if (ur.isPug()) {
            obj.pu.emplace<2>(_fromCapnp(ur.getPug()));
        }
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::Part::Builder b, const ::test::Name::Part& obj) {
    using namespace test;
    b.setPa(_toCapnp(obj.pa));
    b.setPb(_toCapnp(obj.pb));
    switch (obj.pu.index()) {
        case 0: {
                        _toCapnpList(b.initPu().initPu1(std::get<0>(obj.pu).size()), std::get<0>(obj.pu));
            break;
        }
        case 1: {
                        b.initPu().setPu2(_toCapnp(std::get<1>(obj.pu)));
            break;
        }
        case 2: {
            podToCapnp(b.initPu().initPug(), std::get<2>(obj.pu));
            break;
        }
    }
}

::test::Name::_g1::_g2::_g3 podFromCapnp(::test::capnp::Name::G1::G2::G3::Reader r) {
    using namespace test;
    ::test::Name::_g1::_g2::_g3 obj;
    if (r.hasN3()) {
        obj.n3 = _fromCapnp(r.getN3());
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::G1::G2::G3::Builder b, const ::test::Name::_g1::_g2::_g3& obj) {
    using namespace test;
    b.setN3(_toCapnp(obj.n3));
}

::test::Name::_g1::_g2 podFromCapnp(::test::capnp::Name::G1::G2::Reader r) {
    using namespace test;
    ::test::Name::_g1::_g2 obj;
    if (r.hasN2()) {
        obj.n2 = _fromCapnp(r.getN2());
    }
    obj.g3 = _fromCapnp(r.getG3());
    return obj;
}

void podToCapnp(::test::capnp::Name::G1::G2::Builder b, const ::test::Name::_g1::_g2& obj) {
    using namespace test;
    b.setN2(_toCapnp(obj.n2));
    podToCapnp(b.initG3(), obj.g3);
}

::test::Name::_g1 podFromCapnp(::test::capnp::Name::G1::Reader r) {
    using namespace test;
    ::test::Name::_g1 obj;
    if (r.hasN1()) {
        obj.n1 = _fromCapnp(r.getN1());
    }
    obj.g2 = _fromCapnp(r.getG2());
    return obj;
}

void podToCapnp(::test::capnp::Name::G1::Builder b, const ::test::Name::_g1& obj) {
    using namespace test;
    b.setN1(_toCapnp(obj.n1));
    podToCapnp(b.initG2(), obj.g2);
}

::test::Name::_gu::_vc podFromCapnp(::test::capnp::Name::U::Gu::Gu3::Vc::Reader r) {
    using namespace test;
    ::test::Name::_gu::_vc obj;
    if (r.hasVcg1()) {
        obj.vcg1 = _fromCapnp(r.getVcg1());
    }
    if (r.hasVcg2()) {
        obj.vcg2 = _fromCapnp(r.getVcg2());
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::U::Gu::Gu3::Vc::Builder b, const ::test::Name::_gu::_vc& obj) {
    using namespace test;
    b.setVcg1(_toCapnp(obj.vcg1));
    b.setVcg2(_toCapnp(obj.vcg2));
}

::test::Name::_gu podFromCapnp(::test::capnp::Name::U::Gu::Reader r) {
    using namespace test;
    ::test::Name::_gu obj;
    if (r.hasGu1()) {
        obj.gu1 = _fromCapnp(r.getGu1());
    }
    if (r.hasGu2()) {
        obj.gu2 = _fromCapnp(r.getGu2());
    }
    {
        auto ur = r.getGu3();
        if (ur.isVa()) {
            obj.gu3.emplace<0>(_fromCapnp(ur.getVa()));
        }
        if (ur.isVb()) {
            obj.gu3.emplace<1>(_fromCapnp(ur.getVb()));
        }
        if (ur.isVc()) {
            obj.gu3.emplace<2>(_fromCapnp(ur.getVc()));
        }
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::U::Gu::Builder b, const ::test::Name::_gu& obj) {
    using namespace test;
    b.setGu1(_toCapnp(obj.gu1));
    b.setGu2(_toCapnp(obj.gu2));
    switch (obj.gu3.index()) {
        case 0: {
                        b.initGu3().setVa(_toCapnp(std::get<0>(obj.gu3)));
            break;
        }
        case 1: {
                        b.initGu3().setVb(_toCapnp(std::get<1>(obj.gu3)));
            break;
        }
        case 2: {
            podToCapnp(b.initGu3().initVc(), std::get<2>(obj.gu3));
            break;
        }
    }
}

::test::Name podFromCapnp(::test::capnp::Name::Reader r) {
    using namespace test;
    ::test::Name obj;
    obj.id = r.getId();
    if (r.hasName()) {
        obj.name = _fromCapnp(r.getName());
    }
    obj.g1 = _fromCapnp(r.getG1());
    {
        auto ur = r.getU();
        if (ur.isAu()) {
            obj.u.emplace<0>(_fromCapnp(ur.getAu()));
        }
        if (ur.isBu()) {
            obj.u.emplace<1>(_fromCapnp(ur.getBu()));
        }
        if (ur.isGu()) {
            obj.u.emplace<2>(_fromCapnp(ur.getGu()));
        }
    }
    return obj;
}

void podToCapnp(::test::capnp::Name::Builder b, const ::test::Name& obj) {
    using namespace test;
    b.setId(obj.id);
    b.setName(_toCapnp(obj.name));
    podToCapnp(b.initG1(), obj.g1);
    switch (obj.u.index()) {
        case 0: {
                        b.initU().setAu(_toCapnp(std::get<0>(obj.u)));
            break;
        }
        case 1: {
                        b.initU().setBu(_toCapnp(std::get<1>(obj.u)));
            break;
        }
        case 2: {
            podToCapnp(b.initU().initGu(), std::get<2>(obj.u));
            break;
        }
    }
}

::test::Simple::_gg podFromCapnp(::test::capnp::Simple::Gg::Reader r) {
    using namespace test;
    ::test::Simple::_gg obj;
    if (r.hasStr()) {
        obj.str = _fromCapnp(r.getStr());
    }
    obj.float_ = r.getFloat();
    return obj;
}

void podToCapnp(::test::capnp::Simple::Gg::Builder b, const ::test::Simple::_gg& obj) {
    using namespace test;
    b.setStr(_toCapnp(obj.str));
    b.setFloat(obj.float_);
}

::test::Simple podFromCapnp(::test::capnp::Simple::Reader r) {
    using namespace test;
    ::test::Simple obj;
    obj.bool_ = r.getBool();
    obj.int8 = r.getInt8();
    obj.int16 = r.getInt16();
    obj.int32 = r.getInt32();
    obj.int64 = r.getInt64();
    obj.uint8 = r.getUint8();
    obj.uint16 = r.getUint16();
    obj.uint32 = r.getUint32();
    obj.uint64 = r.getUint64();
    obj.float_ = r.getFloat();
    obj.double_ = r.getDouble();
    if (r.hasString()) {
        obj.string = _fromCapnp(r.getString());
    }
    if (r.hasData()) {
        obj.data = _fromCapnp(r.getData());
    }
    obj.enumer = _fromCapnp(r.getEnumer());
    if (r.hasList()) {
        obj.list = _fromCapnp(r.getList());
    }
    if (r.hasObj()) {
        obj.obj = _fromCapnp(r.getObj());
    }
    if (r.hasObjlist()) {
        obj.objlist = _fromCapnp(r.getObjlist());
    }
    if (r.hasLlist()) {
        obj.llist = _fromCapnp(r.getLlist());
    }
    if (r.hasLnlist()) {
        obj.lnlist = _fromCapnp(r.getLnlist());
    }
    if (r.hasElist()) {
        obj.elist = _fromCapnp(r.getElist());
    }
    if (r.hasIlist()) {
        obj.ilist = _fromCapnp(r.getIlist());
    }
    obj.gg = _fromCapnp(r.getGg());
    {
        auto ur = r.getOpt();
        if (ur.isValue()) {
            obj.opt.emplace(_fromCapnp(ur.getValue()));
        } else {
            obj.opt.reset();
        }
    }
    {
        auto ur = r.getUn();
        if (ur.isUnBool()) {
            obj.un.emplace<0>(_fromCapnp(ur.getUnBool()));
        }
        if (ur.isUnInt()) {
            obj.un.emplace<1>(_fromCapnp(ur.getUnInt()));
        }
        if (ur.isUnUint()) {
            obj.un.emplace<2>(_fromCapnp(ur.getUnUint()));
        }
        if (ur.isUnString()) {
            obj.un.emplace<3>(_fromCapnp(ur.getUnString()));
        }
        if (ur.isUnList()) {
            obj.un.emplace<4>(_fromCapnp(ur.getUnList()));
        }
        if (ur.isUnEnum()) {
            obj.un.emplace<5>(_fromCapnp(ur.getUnEnum()));
        }
        if (ur.isUnData()) {
            obj.un.emplace<6>(_fromCapnp(ur.getUnData()));
        }
        if (ur.isUnObj()) {
            obj.un.emplace<7>(_fromCapnp(ur.getUnObj()));
        }
        if (ur.isUnObjlist()) {
            obj.un.emplace<8>(_fromCapnp(ur.getUnObjlist()));
        }
    }
    {
        auto ur = r.getVn();
        if (ur.isVInt()) {
            obj.vn.emplace<0>(_fromCapnp(ur.getVInt()));
        }
        if (ur.isVoyd()) {
            obj.vn.emplace<1>(_fromCapnp(ur.getVoyd()));
        }
        if (ur.isUnBool()) {
            obj.vn.emplace<2>(_fromCapnp(ur.getUnBool()));
        }
    }
    return obj;
}

void podToCapnp(::test::capnp::Simple::Builder b, const ::test::Simple& obj) {
    using namespace test;
    b.setBool(obj.bool_);
    b.setInt8(obj.int8);
    b.setInt16(obj.int16);
    b.setInt32(obj.int32);
    b.setInt64(obj.int64);
    b.setUint8(obj.uint8);
    b.setUint16(obj.uint16);
    b.setUint32(obj.uint32);
    b.setUint64(obj.uint64);
    b.setFloat(obj.float_);
    b.setDouble(obj.double_);
    b.setString(_toCapnp(obj.string));
    b.setData(_toCapnp(obj.data));
    b.setEnumer(podToCapnp(obj.enumer));
    _toCapnpList(b.initList(obj.list.size()), obj.list);
    podToCapnp(b.initObj(), obj.obj);
    ::test::_toCapnpList(b.initObjlist(obj.objlist.size()), obj.objlist);
    _toCapnpList(b.initLlist(obj.llist.size()), obj.llist);
    ::test::_toCapnpList(b.initLnlist(obj.lnlist.size()), obj.lnlist);
    _toCapnpList(b.initElist(obj.elist.size()), obj.elist);
    _toCapnpList(b.initIlist(obj.ilist.size()), obj.ilist);
    podToCapnp(b.initGg(), obj.gg);
    if (obj.opt) {
        b.initOpt().setValue(_toCapnp(obj.opt.value()));
    } else {
        b.initOpt().setNull();
    }
    switch (obj.un.index()) {
        case 0: {
            b.initUn().setUnBool(std::get<0>(obj.un));
            break;
        }
        case 1: {
            b.initUn().setUnInt(std::get<1>(obj.un));
            break;
        }
        case 2: {
            b.initUn().setUnUint(std::get<2>(obj.un));
            break;
        }
        case 3: {
                        b.initUn().setUnString(_toCapnp(std::get<3>(obj.un)));
            break;
        }
        case 4: {
                        _toCapnpList(b.initUn().initUnList(std::get<4>(obj.un).size()), std::get<4>(obj.un));
            break;
        }
        case 5: {
            b.initUn().setUnEnum(podToCapnp(std::get<5>(obj.un)));
            break;
        }
        case 6: {
                        b.initUn().setUnData(_toCapnp(std::get<6>(obj.un)));
            break;
        }
        case 7: {
            podToCapnp(b.initUn().initUnObj(), std::get<7>(obj.un));
            break;
        }
        case 8: {
                        ::test::_toCapnpList(b.initUn().initUnObjlist(std::get<8>(obj.un).size()), std::get<8>(obj.un));
            break;
        }
    }
    switch (obj.vn.index()) {
        case 0: {
            b.initVn().setVInt(std::get<0>(obj.vn));
            break;
        }
        case 1: {
            b.initVn().setVoyd();
            break;
        }
        case 2: {
            b.initVn().setUnBool(std::get<2>(obj.vn));
            break;
        }
    }
}

::test::Variant podFromCapnp(::test::capnp::Variant::Reader r) {
    using namespace test;
    ::test::Variant obj;
    if (r.isString()) {
        obj.emplace<0>(_fromCapnp(r.getString()));
    }
    if (r.isInt16()) {
        obj.emplace<1>(_fromCapnp(r.getInt16()));
    }
    if (r.isData()) {
        obj.emplace<2>(_fromCapnp(r.getData()));
    }
    if (r.isChoice()) {
        obj.emplace<3>(_fromCapnp(r.getChoice()));
    }
    return obj;
}

void podToCapnp(::test::capnp::Variant::Builder b, const ::test::Variant& obj) {
    using namespace test;
    switch (obj.index()) {
        case 0: {
                        b.setString(_toCapnp(std::get<0>(obj)));
            break;
        }
        case 1: {
            b.setInt16(std::get<1>(obj));
            break;
        }
        case 2: {
                        b.setData(_toCapnp(std::get<2>(obj)));
            break;
        }
        case 3: {
            b.setChoice(podToCapnp(std::get<3>(obj)));
            break;
        }
    }
}

::test::MaybeText podFromCapnp(::test::capnp::MaybeText::Reader r) {
    using namespace test;
    ::test::MaybeText obj;
    if (r.isValue()) {
        obj.emplace(_fromCapnp(r.getValue()));
    } else {
        obj.reset();
    }
    return obj;
}

void podToCapnp(::test::capnp::MaybeText::Builder b, const ::test::MaybeText& obj) {
    using namespace test;
    if (obj) {
        _toCapnpList(b.initValue(obj.value().size()), obj.value());
    } else {
        b.setNull();
    }
}

::test::Container podFromCapnp(::test::capnp::Container::Reader r) {
    using namespace test;
    ::test::Container obj;
    if (r.hasV()) {
        obj.v = _fromCapnp(r.getV());
    }
    if (r.hasT()) {
        obj.t = _fromCapnp(r.getT());
    }
    return obj;
}

void podToCapnp(::test::capnp::Container::Builder b, const ::test::Container& obj) {
    using namespace test;
    podToCapnp(b.initV(), obj.v);
    podToCapnp(b.initT(), obj.t);
}

::test::Lists podFromCapnp(::test::capnp::Lists::Reader r) {
    using namespace test;
    ::test::Lists obj;
    obj.timestamp = r.getTimestamp();
    if (r.hasList()) {
        obj.list = _fromCapnp(r.getList());
    }
    if (r.hasNlist()) {
        obj.nlist = _fromCapnpList<std::unordered_set<::test::Name, ::test::Name::_Hasher>>(r.getNlist());
    }
    if (r.hasRlist()) {
        obj.rlist = _fromCapnpList<std::list<::test::Name>>(r.getRlist());
    }
    if (r.hasSset()) {
        obj.sset = _fromCapnpList<std::set<std::string>>(r.getSset());
    }
    if (r.hasMap()) {
        obj.map = _fromCapnpList<std::unordered_map<std::string, uint64_t>>(r.getMap());
    }
    if (r.hasListoflists()) {
        obj.listoflists = _fromCapnpList<std::list<std::list<std::string>>>(r.getListoflists());
    }
    if (r.hasEmpty()) {
        obj.empty = _fromCapnp(r.getEmpty());
    }
    return obj;
}

void podToCapnp(::test::capnp::Lists::Builder b, const ::test::Lists& obj) {
    using namespace test;
    b.setTimestamp(obj.timestamp);
    _toCapnpList(b.initList(obj.list.size()), obj.list);
    ::test::_toCapnpList(b.initNlist(obj.nlist.size()), obj.nlist);
    ::test::_toCapnpList(b.initRlist(obj.rlist.size()), obj.rlist);
    _toCapnpList(b.initSset(obj.sset.size()), obj.sset);
    ::test::_toCapnpList(b.initMap(obj.map.size()), obj.map);
    _toCapnpList(b.initListoflists(obj.listoflists.size()), obj.listoflists);
    _toCapnpList(b.initEmpty(obj.empty.size()), obj.empty);
}

::test::Base podFromCapnp(::test::capnp::Base::Reader r) {
    using namespace test;
    ::test::Base obj;
    obj.id = r.getId();
    if (r.hasOwner()) {
        obj.owner = _fromCapnp(r.getOwner());
    }
    obj.created = r.getCreated();
    if (r.hasCreatedBy()) {
        obj.createdBy = _fromCapnp(r.getCreatedBy());
    }
    obj.modified = r.getModified();
    if (r.hasModifiedBy()) {
        obj.modifiedBy = _fromCapnp(r.getModifiedBy());
    }
    return obj;
}

void podToCapnp(::test::capnp::Base::Builder b, const ::test::Base& obj) {
    using namespace test;
    b.setId(obj.id);
    b.setOwner(_toCapnp(obj.owner));
    b.setCreated(obj.created);
    b.setCreatedBy(_toCapnp(obj.createdBy));
    b.setModified(obj.modified);
    b.setModifiedBy(_toCapnp(obj.modifiedBy));
}

::test::Titled podFromCapnp(::test::capnp::Titled::Reader r) {
    using namespace test;
    ::test::Titled obj;
    if (r.hasTitle()) {
        obj.title = _fromCapnp(r.getTitle());
    }
    if (r.hasDescription()) {
        obj.description = _fromCapnp(r.getDescription());
    }
    return obj;
}

void podToCapnp(::test::capnp::Titled::Builder b, const ::test::Titled& obj) {
    using namespace test;
    b.setTitle(_toCapnp(obj.title));
    b.setDescription(_toCapnp(obj.description));
}

::test::Entity podFromCapnp(::test::capnp::Entity::Reader r) {
    using namespace test;
    ::test::Entity obj;
    obj.id = r.getId();
    if (r.hasOwner()) {
        obj.owner = _fromCapnp(r.getOwner());
    }
    obj.created = r.getCreated();
    if (r.hasCreatedBy()) {
        obj.createdBy = _fromCapnp(r.getCreatedBy());
    }
    obj.modified = r.getModified();
    if (r.hasModifiedBy()) {
        obj.modifiedBy = _fromCapnp(r.getModifiedBy());
    }
    if (r.hasTitle()) {
        obj.title = _fromCapnp(r.getTitle());
    }
    if (r.hasDescription()) {
        obj.description = _fromCapnp(r.getDescription());
    }
    if (r.hasInfo()) {
        obj.info = _fromCapnp(r.getInfo());
    }
    if (r.hasLocation()) {
        obj.location = _fromCapnp(r.getLocation());
    }
    return obj;
}

void podToCapnp(::test::capnp::Entity::Builder b, const ::test::Entity& obj) {
    using namespace test;
    b.setId(obj.id);
    b.setOwner(_toCapnp(obj.owner));
    b.setCreated(obj.created);
    b.setCreatedBy(_toCapnp(obj.createdBy));
    b.setModified(obj.modified);
    b.setModifiedBy(_toCapnp(obj.modifiedBy));
    b.setTitle(_toCapnp(obj.title));
    b.setDescription(_toCapnp(obj.description));
    b.setInfo(_toCapnp(obj.info));
    b.setLocation(_toCapnp(obj.location));
}

::test::Empty podFromCapnp(::test::capnp::Empty::Reader r) {
    using namespace test;
    ::test::Empty obj;
    return obj;
}

void podToCapnp(::test::capnp::Empty::Builder b, const ::test::Empty& obj) {
    using namespace test;
}

::test::Sparse podFromCapnp(::test::capnp::Sparse::Reader r) {
    using namespace test;
    ::test::Sparse obj;
    obj.id = r.getId();
    {
        auto ur = r.getName();
        if (ur.isValue()) {
            obj.name.emplace(_fromCapnp(ur.getValue()));
        } else {
            obj.name.reset();
        }
    }
    return obj;
}

void podToCapnp(::test::capnp::Sparse::Builder b, const ::test::Sparse& obj) {
    using namespace test;
    b.setId(obj.id);
    if (obj.name) {
        b.initName().setValue(_toCapnp(obj.name.value()));
    } else {
        b.initName().setNull();
    }
}

::test::User podFromCapnp(::test::capnp::User::Reader r) {
    using namespace test;
    ::test::User obj;
    obj.id = r.getId();
    if (r.hasEmail()) {
        obj.email = _fromCapnp(r.getEmail());
    }
    return obj;
}

void podToCapnp(::test::capnp::User::Builder b, const ::test::User& obj) {
    using namespace test;
    b.setId(obj.id);
    b.setEmail(_toCapnp(obj.email));
}

::test::Account podFromCapnp(::test::capnp::Account::Reader r) {
    using namespace test;
    ::test::Account obj;
    obj.id = r.getId();
    if (r.hasName()) {
        obj.name = _fromCapnp(r.getName());
    }
    return obj;
}

void podToCapnp(::test::capnp::Account::Builder b, const ::test::Account& obj) {
    using namespace test;
    b.setId(obj.id);
    b.setName(_toCapnp(obj.name));
}

::test::TextError podFromCapnp(::test::capnp::TextError::Reader r) {
    using namespace test;
    ::test::TextError obj;
    if (r.hasMsg()) {
        obj.msg = _fromCapnp(r.getMsg());
    }
    return obj;
}

void podToCapnp(::test::capnp::TextError::Builder b, const ::test::TextError& obj) {
    using namespace test;
    b.setMsg(_toCapnp(obj.msg));
}

::test::IntError podFromCapnp(::test::capnp::IntError::Reader r) {
    using namespace test;
    ::test::IntError obj;
    obj.code = r.getCode();
    return obj;
}

void podToCapnp(::test::capnp::IntError::Builder b, const ::test::IntError& obj) {
    using namespace test;
    b.setCode(obj.code);
}

::test::Get<::test::User, std::vector<::test::User>> podFromCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Reader r) {
    using namespace test;
    ::test::Get<::test::User, std::vector<::test::User>> obj;
    if (r.hasGetOne()) {
        obj.getOne = _fromCapnp(r.getGetOne());
    }
    if (r.hasGetAll()) {
        obj.getAll = _fromCapnp(r.getGetAll());
    }
    return obj;
}

void podToCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Builder b, const ::test::Get<::test::User, std::vector<::test::User>>& obj) {
    using namespace test;
    podToCapnp(b.initGetOne(), obj.getOne);
    ::test::_toCapnpList(b.initGetAll(obj.getAll.size()), obj.getAll);
}

::test::Get<::test::User, std::vector<::test::User>>::Result podFromCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Result::Reader r) {
    using namespace test;
    ::test::Get<::test::User, std::vector<::test::User>>::Result obj;
    if (r.hasMsg()) {
        obj.msg = _fromCapnp(r.getMsg());
    }
    return obj;
}

void podToCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Result::Builder b, const ::test::Get<::test::User, std::vector<::test::User>>::Result& obj) {
    using namespace test;
    b.setMsg(_toCapnp(obj.msg));
}

::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError> podFromCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Error<::test::capnp::IntError>::Reader r) {
    using namespace test;
    ::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError> obj;
    if (r.hasError()) {
        obj.error = _fromCapnp(r.getError());
    }
    return obj;
}

void podToCapnp(::test::capnp::Get<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Error<::test::capnp::IntError>::Builder b, const ::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>& obj) {
    using namespace test;
    podToCapnp(b.initError(), obj.error);
}

::test::Get<::test::Account, std::vector<::test::Account>> podFromCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Reader r) {
    using namespace test;
    ::test::Get<::test::Account, std::vector<::test::Account>> obj;
    if (r.hasGetOne()) {
        obj.getOne = _fromCapnp(r.getGetOne());
    }
    if (r.hasGetAll()) {
        obj.getAll = _fromCapnp(r.getGetAll());
    }
    return obj;
}

void podToCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Builder b, const ::test::Get<::test::Account, std::vector<::test::Account>>& obj) {
    using namespace test;
    podToCapnp(b.initGetOne(), obj.getOne);
    ::test::_toCapnpList(b.initGetAll(obj.getAll.size()), obj.getAll);
}

::test::Get<::test::Account, std::vector<::test::Account>>::Result podFromCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Result::Reader r) {
    using namespace test;
    ::test::Get<::test::Account, std::vector<::test::Account>>::Result obj;
    if (r.hasMsg()) {
        obj.msg = _fromCapnp(r.getMsg());
    }
    return obj;
}

void podToCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Result::Builder b, const ::test::Get<::test::Account, std::vector<::test::Account>>::Result& obj) {
    using namespace test;
    b.setMsg(_toCapnp(obj.msg));
}

::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError> podFromCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Error<::test::capnp::TextError>::Reader r) {
    using namespace test;
    ::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError> obj;
    if (r.hasError()) {
        obj.error = _fromCapnp(r.getError());
    }
    return obj;
}

void podToCapnp(::test::capnp::Get<::test::capnp::Account, ::capnp::List<::test::capnp::Account, ::capnp::Kind::STRUCT>>::Error<::test::capnp::TextError>::Builder b, const ::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>& obj) {
    using namespace test;
    podToCapnp(b.initError(), obj.error);
}

::test::GetInterface podFromCapnp(::test::capnp::GetInterface::Reader r) {
    using namespace test;
    ::test::GetInterface obj;
    if (r.hasGetUser()) {
        obj.getUser = _fromCapnp(r.getGetUser());
    }
    if (r.hasUserResult()) {
        obj.userResult = _fromCapnp(r.getUserResult());
    }
    if (r.hasUserError()) {
        obj.userError = _fromCapnp(r.getUserError());
    }
    if (r.hasGetAccount()) {
        obj.getAccount = _fromCapnp(r.getGetAccount());
    }
    if (r.hasAccountResult()) {
        obj.accountResult = _fromCapnp(r.getAccountResult());
    }
    if (r.hasAccountError()) {
        obj.accountError = _fromCapnp(r.getAccountError());
    }
    return obj;
}

void podToCapnp(::test::capnp::GetInterface::Builder b, const ::test::GetInterface& obj) {
    using namespace test;
    podToCapnp(b.initGetUser(), obj.getUser);
    podToCapnp(b.initUserResult(), obj.userResult);
    podToCapnp(b.initUserError(), obj.userError);
    podToCapnp(b.initGetAccount(), obj.getAccount);
    podToCapnp(b.initAccountResult(), obj.accountResult);
    podToCapnp(b.initAccountError(), obj.accountError);
}

::test::AltGet<::test::User, std::vector<::test::User>> podFromCapnp(::test::capnp::AltGet<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Reader r) {
    using namespace test;
    ::test::AltGet<::test::User, std::vector<::test::User>> obj;
    if (r.hasGet()) {
        obj.get = _fromCapnp(r.getGet());
    }
    return obj;
}

void podToCapnp(::test::capnp::AltGet<::test::capnp::User, ::capnp::List<::test::capnp::User, ::capnp::Kind::STRUCT>>::Builder b, const ::test::AltGet<::test::User, std::vector<::test::User>>& obj) {
    using namespace test;
    podToCapnp(b.initGet(), obj.get);
}

::test::AltInterface podFromCapnp(::test::capnp::AltInterface::Reader r) {
    using namespace test;
    ::test::AltInterface obj;
    if (r.hasUser()) {
        obj.user = _fromCapnp(r.getUser());
    }
    return obj;
}

void podToCapnp(::test::capnp::AltInterface::Builder b, const ::test::AltInterface& obj) {
    using namespace test;
    podToCapnp(b.initUser(), obj.user);
}

