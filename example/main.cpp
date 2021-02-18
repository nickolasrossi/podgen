#include <test.pod.hpp>
#include <test.convert.hpp>
#include <capnp/message.h>
#include <capnp/serialize.h>

int main() {
    test::Lists lists;
    lists.timestamp = 512;
    lists.list.emplace_back("value 1");
    lists.list.emplace_back("value 2");
    lists.sset.emplace("2. second");
    lists.sset.emplace("1. first");
    lists.listoflists.assign({std::list<std::string>({{"hi"}})});

    capnp::MallocMessageBuilder builder;
    auto capnpLists = builder.initRoot<test::capnp::Lists>();
    podToCapnp(capnpLists, lists);

    if (!capnpLists.getSset()[0].asString().startsWith("1.")) {
        return 1;
    }
    capnp::writeMessageToFd(fileno(stdout), builder);
}