#include <unordered_set>
#include <unordered_map>

#include <capnp/message.h>

#include "test_simple.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace podgen;

void checkConversion(const test::Simple& simple) {
    capnp::MallocMessageBuilder out;

    // note the use of capnpTypeOf() to determine the capnp type corresponding to a pod type.
    // it's not an implemented function. the declaration only exists as a type mapper.
    ::test::capnp::Simple::Builder msg_out = out.initRoot<::test::capnp::Simple>();

    podToCapnp(msg_out, simple);
    std::cout << msg_out.toString().flatten().cStr() << std::endl;

    test::Simple s_out = podFromCapnp(msg_out.asReader());
    CHECK(simple == s_out);
    CHECK(hash(simple) == hash(s_out));

    s_out.int8--;
    CHECK(hash(simple) != hash(s_out));
    s_out.int8++;
    CHECK(hash(simple) == hash(s_out));

    s_out.lnlist.pop_back();
    CHECK(hash(simple) != hash(s_out));
}

TEST_CASE("convert from/to", "podgen") {
    using namespace test;
    auto simple = buildSimple();
    std::cout << std::endl << "Simple type outputter: " << simple << std::endl << std::endl;
    checkConversion(simple);
}

TEST_CASE("constants", "podgen") {
    using namespace test;
    REQUIRE(whatever == "whatever");
    REQUIRE(Simple::TypeName == "Simple");
    REQUIRE(Simple::band == "minds");
    REQUIRE(Simple::answer == 42);
    REQUIRE(Simple::choice == Choice::B);
}

#if 0
TEST_CASE("to buffer", "podgen") {
    using namespace test;
    auto simple = buildSimple();

    ::capnp::MallocMessageBuilder out;
    auto msg_out = out.initRoot<decltype(capnpTypeOf(simple))>();
    podToCapnp(msg_out, simple);

    auto str = messageToStringBuffer(out);
    std::cout << "simple obj size: " << str.size() << std::endl;
    CHECK(str.size() > 0);
}

TEST_CASE("large capn4", "podgen") {
    using namespace test;
    auto simple = buildSimple();

    for (size_t i = 0; i < 1000000; i++) {
        simple.objlist.push_back({99, "nested" + std::to_string(i)});
    }

    auto capn4 = podToCapn4(simple);
    std::cout << "capn4 size: " << capn4.size() << std::endl;

    auto simple_out = podFromCapn4<Simple>(capn4);
    CHECK(simple == simple_out);
}
#endif

TEST_CASE("external type", "podgen") {
    using namespace test;
    Pose pose;

    pose.translation << 4.4, 5.5, 6.6;
    pose.rotation << -2.2, -3.3, -1.1;

    std::cout << std::endl << "Pose type output: " << pose << std::endl << std::endl;

    ::capnp::MallocMessageBuilder out;
    auto msg_out = out.initRoot<decltype(capnpTypeOf(pose))>();
    podToCapnp(msg_out, pose);

    std::cout << kj::str(msg_out).cStr() << std::endl;

    Pose p_out = podFromCapnp(msg_out.asReader());
    CHECK(pose == p_out);
}

TEST_CASE("output", "podgen") {
    using namespace test;

    std::pair<std::unordered_set<std::string>, std::vector<std::string>> p;

    p.first.insert("who");
    p.first.insert("is");
    p.first.insert("there");

    p.second.emplace_back("what");
    p.second.emplace_back("is");
    p.second.emplace_back("that");

    std::ostringstream ss;
    _out(ss, p);
    CHECK(ss.str() == "{[\"there\", \"is\", \"who\"], [\"what\", \"is\", \"that\"]}");
    std::cout << ss.str() << std::endl;

    std::unordered_map<std::string, std::string> s;
    s.emplace("a", "hey");
    s.emplace("b", "what");
    s.emplace("c", "knock");

    ss.str("");
    _out(ss, s);
    CHECK(ss.str() == "[{\"b\" -> \"what\"}, {\"c\" -> \"knock\"}, {\"a\" -> \"hey\"}]");
    std::cout << ss.str() << std::endl;

    std::vector<Data> v;
    v.push_back(Data { std::byte(0x44) });
    v.push_back(Data { std::byte(0x45), std::byte(0x46) });

    ss.str("");
    _out(ss, v);
    CHECK(ss.str() == "[(1)[44], (2)[45 46]]");
    std::cout << ss.str() << std::endl;

    std::variant<int, std::string> va;
    va.emplace<int>(9);

    ss.str("");
    _out(ss, va);
    CHECK(ss.str() == "9");

    Container c;
    c.v.emplace<int16_t>(347);
    c.t.emplace(std::vector<std::string> {"yo"});

    ss.str("");
    _out(ss, c);
    CHECK(ss.str() == "Container{v=347 t=[\"yo\"]}");
    std::cout << ss.str() << std::endl;
}

TEST_CASE("custom lists", "podgen") {
    using namespace test;

    Lists ls { .timestamp = 99999 };

    ls.list.emplace_back("a");
    ls.list.emplace_back("b");
    ls.list.emplace_back("b");

    ls.nlist.insert(Name { .id = 3, .name = "tre" });
    ls.nlist.insert(Name { .id = 5, .name = "cinque" });
    ls.nlist.insert(Name { .id = 5, .name = "cinque" });
    ls.nlist.insert(Name { .id = 6, .name = "sei" });
    ls.nlist.insert(Name { .id = 8, .name = "otto" });

#if 0
    ls.rlist.emplace_back(4, 5, 5);
    ls.rlist.emplace_back(2, 9, 1);
    ls.rlist.emplace_back(7, 8, 7);
#endif

    ls.sset.insert("z");
    ls.sset.insert("x");
    ls.sset.insert("y");

    ls.map.emplace("u", 4);
    ls.map.emplace("v", 9);
    ls.map.emplace("w", 66);

    ls.listoflists.push_back(std::list<std::string> { "m", "n", "o" });
    ls.listoflists.push_back(std::list<std::string> { "m", "n", "o" });
    ls.listoflists.push_back(std::list<std::string> { "h", "j", "k" });
    ls.listoflists.push_back(std::list<std::string> {});
    ls.listoflists.push_back(std::list<std::string> { "p", "q" });

    std::cout << ls << std::endl;

    ::capnp::MallocMessageBuilder out;
    auto msg_out = out.initRoot<decltype(capnpTypeOf(ls))>();
    podToCapnp(msg_out, ls);

    std::cout << msg_out.toString().flatten().cStr() << std::endl;

    auto lists_in = podFromCapnp(msg_out.asReader());

    std::cout << lists_in << std::endl;
    CHECK(ls == lists_in);

    std::cout << hash(ls) << std::endl;
}

TEST_CASE("inheritance", "podgen") {
    using namespace test;
    Entity entity;
    entity.id = 44444;
    entity.owner = "public";
//    entity.created = 55555;
    entity.createdBy = "dickens";
//    entity.modified = 66666;
    entity.modifiedBy = "no one";
    entity.title = "tale of two cities";
    entity.description = "book";
    entity.info = "contents";
    entity.location = "shelf";

    std::cout << entity << std::endl;

    ::capnp::MallocMessageBuilder out;
    auto msg_out = out.initRoot<decltype(capnpTypeOf(entity))>();
    podToCapnp(msg_out, entity);

    auto entity_in = podFromCapnp(msg_out.asReader());

    CHECK(entity == entity_in);
}

TEST_CASE("generics", "podgen") {
    using namespace test;

    GetInterface iface;

    iface.getUser.getOne.id = 41516;
    iface.getUser.getOne.email = "customer@client.com";
    iface.getUser.getAll.push_back(User { .id = 11111, .email = "secondary@client.com" });
    iface.getUser.getAll.push_back(User { .id = 11112, .email = "analyst@client.com" });

    iface.userError.error.code = 415;
    iface.userResult.msg = "not sure";

    iface.getAccount.getOne.id = 98971;
    iface.getAccount.getOne.name = "client";
    iface.getAccount.getAll.push_back(Account { .id = 98900, .name = "team A" });
    iface.getAccount.getAll.push_back(Account { .id = 98901, .name = "team B" });

    iface.accountError.error.msg = "too many";
    iface.accountResult.msg = "just right";

    std::cout << iface << std::endl;

    std::stringstream ss;
    ss << iface;
    CHECK(ss.str() == "GetInterface{getUser=Get{getOne=User{id=41516 email=\"customer@client.com\"} getAll=[User{id=11111 email=\"secondary@client.com\"},"
                      " User{id=11112 email=\"analyst@client.com\"}]} userResult=Result{msg=\"not sure\"} userError=Error{error=IntError{code=415}}"
                      " getAccount=Get{getOne=Account{id=98971 name=\"client\"} getAll=[Account{id=98900 name=\"team A\"}, Account{id=98901 name=\"team B\"}]}"
                      " accountResult=Result{msg=\"just right\"} accountError=Error{error=TextError{msg=\"too many\"}}}");


    ::capnp::MallocMessageBuilder msg1;
    auto iface_cap = msg1.initRoot<decltype(capnpTypeOf(iface))>();
    podToCapnp(iface_cap, iface);

    std::cout << kj::str(iface_cap.asReader()).cStr() << std::endl;

    auto iface_in = podFromCapnp(iface_cap.asReader());

    CHECK(iface == iface_in);
    
    
    AltInterface alt;
    alt.user.get.getOne.id = 77383;
    alt.user.get.getOne.email = "somebody@there.com";
    alt.user.get.getAll.push_back(User { .id = 89890, .email = "nobody@there.com" });
    alt.user.get.getAll.push_back(User { .id = 89891, .email = "nobody@here.com" });
    
    ::capnp::MallocMessageBuilder msg2;
    auto alt_cap = msg2.initRoot<decltype(capnpTypeOf(alt))>();
    podToCapnp(alt_cap, alt);

    std::cout << kj::str(alt_cap.asReader()).cStr() << std::endl;

    auto alt_in = podFromCapnp(alt_cap.asReader());

    CHECK(alt == alt_in);
}