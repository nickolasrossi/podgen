#pragma once

#include "test.convert.hpp"

test::Simple buildSimple() {
    using namespace test;
    Simple simple;

    simple.bool_ = true;
    simple.int8 = -3;
    simple.int16 = -999;
    simple.int32 = -4444444;
    simple.int64 = -77777777777777777L;
    simple.uint8 = 222;
    simple.uint16 = 55555;
    simple.uint32 = 0xdddddddd;
    simple.uint64 = 222222222222222222L;
    simple.float_ = 5.55f;
    simple.double_ = -88.8888;
    simple.string = "whatastring";
    simple.data = ::podgen::Data{std::byte{0xaa}, std::byte{0xbb}, std::byte{0xcc}, std::byte{0xdd}, std::byte{0xee}};
    simple.enumer = test::Choice::B;
    simple.list = std::vector<std::string>{"qq", "rr", "ss", "tt"};
    simple.obj.id = 11111;
    simple.obj.name = "nested";
    simple.objlist.push_back({22222, "nested0"});
    simple.objlist.push_back({33333, "nested1"});
    simple.un = static_cast<int32_t>(222333444);
    simple.vn = std::monostate();
    simple.llist.push_back({44, 33, 22, 11});
    simple.llist.push_back({6, 7, 8});
    simple.llist.push_back({101, 102});
    simple.lnlist.push_back({Name{4, "pre"}, Name{5, "nav"}});
    simple.lnlist.push_back({Name{6, "red"}, Name{7, "wood"}, Name{8, "city"}});
    simple.gg.float_ = 99.9f;
    simple.gg.str = "inner struct";
    simple.opt = "optional";

    return simple;
}
