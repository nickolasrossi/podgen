#pragma once

#include <functional>
#include "defs.hpp"

#include "include.pod.hpp"
#include <unordered_map>
#include <set>
#include <list>
#include <unordered_set>

namespace test {

inline const std::string whatever = "whatever";

struct Name {
    static inline const std::string TypeName = "Name";

    struct Part {
        static inline const std::string TypeName = "Part";

        struct _field {
            static inline const std::string pa = "pa";
            static inline const std::string pb = "pb";
            static inline const std::string pu = "pu";
        };

        std::string pa;
        std::string pb;
        struct _pug {
            struct _field {
                static inline const std::string pug1 = "pug1";
                static inline const std::string pug2 = "pug2";
            };

            std::string pug1;
            struct _pugu2 {
                struct _field {
                    static inline const std::string pugg = "pugg";
                };

                std::string pugg;

                bool operator==(const _pugu2& o) const {
                    return pugg == o.pugg;
                }

                bool operator!=(const _pugu2& o) const {
                    return !operator==(o);
                }

                struct _Hasher {
                    std::size_t operator()(const _pug::_pugu2& o) const {
                        std::size_t s = 0;
                        s = podgen::hashCombine(s, o.pugg);
                        return s;
                    }

                };
            };
            using _pug2 = std::variant<std::string, _pug::_pugu2>;
            enum _pug2_which {
                pug2_pugu1,
                pug2_pugu2
            };

            _pug2 pug2;
            _pug2_which pug2_which() const { return static_cast<_pug2_which>(pug2.index()); }

            bool operator==(const _pug& o) const {
                return pug1 == o.pug1
                    && pug2 == o.pug2;
            }

            bool operator!=(const _pug& o) const {
                return !operator==(o);
            }

            struct _Hasher {
                std::size_t operator()(const _pug& o) const {
                    std::size_t s = 0;
                    s = podgen::hashCombine(s, o.pug1);
                    s = podgen::hashCombine(s, o.pug2);
                    return s;
                }

            };
        };
        using _pu = std::variant<std::vector<std::string>, std::string, _pug>;
        enum _pu_which {
            pu_pu1,
            pu_pu2,
            pu_pug
        };

        _pu pu;
        _pu_which pu_which() const { return static_cast<_pu_which>(pu.index()); }

        bool operator==(const Part& o) const {
            return pa == o.pa
                && pb == o.pb
                && pu == o.pu;
        }

        bool operator!=(const Part& o) const {
            return !operator==(o);
        }

        struct _Hasher {
            std::size_t operator()(const Part& o) const {
                std::size_t s = 0;
                s = podgen::hashCombine(s, o.pa);
                s = podgen::hashCombine(s, o.pb);
                s = podgen::hashCombine(s, o.pu);
                return s;
            }

        };
    };

    struct _field {
        static inline const std::string id = "id";
        static inline const std::string name = "name";
        static inline const std::string g1 = "g1";
        static inline const std::string u = "u";
    };

    uint16_t id = 0;
    std::string name;
    struct _g1 {
        struct _field {
            static inline const std::string n1 = "n1";
            static inline const std::string g2 = "g2";
        };

        std::string n1;
        struct _g2 {
            struct _field {
                static inline const std::string n2 = "n2";
                static inline const std::string g3 = "g3";
            };

            std::string n2;
            struct _g3 {
                struct _field {
                    static inline const std::string n3 = "n3";
                };

                std::string n3;

                bool operator==(const _g3& o) const {
                    return n3 == o.n3;
                }

                bool operator!=(const _g3& o) const {
                    return !operator==(o);
                }

                struct _Hasher {
                    std::size_t operator()(const _g3& o) const {
                        std::size_t s = 0;
                        s = podgen::hashCombine(s, o.n3);
                        return s;
                    }

                };
            } g3;

            bool operator==(const _g2& o) const {
                return n2 == o.n2
                    && g3 == o.g3;
            }

            bool operator!=(const _g2& o) const {
                return !operator==(o);
            }

            struct _Hasher {
                std::size_t operator()(const _g2& o) const {
                    std::size_t s = 0;
                    s = podgen::hashCombine(s, o.n2);
                    s = podgen::hashCombine(s, o.g3);
                    return s;
                }

            };
        } g2;

        bool operator==(const _g1& o) const {
            return n1 == o.n1
                && g2 == o.g2;
        }

        bool operator!=(const _g1& o) const {
            return !operator==(o);
        }

        struct _Hasher {
            std::size_t operator()(const _g1& o) const {
                std::size_t s = 0;
                s = podgen::hashCombine(s, o.n1);
                s = podgen::hashCombine(s, o.g2);
                return s;
            }

        };
    } g1;
    struct _gu {
        struct _field {
            static inline const std::string gu1 = "gu1";
            static inline const std::string gu2 = "gu2";
            static inline const std::string gu3 = "gu3";
        };

        std::string gu1;
        std::string gu2;
        struct _vc {
            struct _field {
                static inline const std::string vcg1 = "vcg1";
                static inline const std::string vcg2 = "vcg2";
            };

            std::string vcg1;
            std::string vcg2;

            bool operator==(const _vc& o) const {
                return vcg1 == o.vcg1
                    && vcg2 == o.vcg2;
            }

            bool operator!=(const _vc& o) const {
                return !operator==(o);
            }

            struct _Hasher {
                std::size_t operator()(const _gu::_vc& o) const {
                    std::size_t s = 0;
                    s = podgen::hashCombine(s, o.vcg1);
                    s = podgen::hashCombine(s, o.vcg2);
                    return s;
                }

            };
        };
        using _gu3 = std::variant<std::string, std::string, _gu::_vc>;
        enum _gu3_which {
            gu3_va,
            gu3_vb,
            gu3_vc
        };

        _gu3 gu3;
        _gu3_which gu3_which() const { return static_cast<_gu3_which>(gu3.index()); }

        bool operator==(const _gu& o) const {
            return gu1 == o.gu1
                && gu2 == o.gu2
                && gu3 == o.gu3;
        }

        bool operator!=(const _gu& o) const {
            return !operator==(o);
        }

        struct _Hasher {
            std::size_t operator()(const _gu& o) const {
                std::size_t s = 0;
                s = podgen::hashCombine(s, o.gu1);
                s = podgen::hashCombine(s, o.gu2);
                s = podgen::hashCombine(s, o.gu3);
                return s;
            }

        };
    };
    using _u = std::variant<std::string, std::string, _gu>;
    enum _u_which {
        u_au,
        u_bu,
        u_gu
    };

    _u u;
    _u_which u_which() const { return static_cast<_u_which>(u.index()); }

    bool operator==(const Name& o) const {
        return id == o.id
            && name == o.name
            && g1 == o.g1
            && u == o.u;
    }

    bool operator!=(const Name& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Name& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.id);
            s = podgen::hashCombine(s, o.name);
            s = podgen::hashCombine(s, o.g1);
            s = podgen::hashCombine(s, o.u);
            return s;
        }

    };
};

namespace Choice_bf0825cdafa9209c {
enum Choice : uint16_t {
    A,
    C,
    B,
    New,
    CamelCase,
};
}

using Choice = Choice_bf0825cdafa9209c::Choice;

inline const std::string Choice_TypeName = "Choice";
inline const std::vector<std::string> Choice_names {
    "A",
    "C",
    "B",
    "New",
    "CamelCase",
};

struct Simple {
    static inline const std::string TypeName = "Simple";

    static inline const std::string band = "minds";
    static inline const uint16_t answer = 42;
    static inline const Choice choice = Choice::B;

    struct _field {
        static inline const std::string bool_ = "bool";
        static inline const std::string int8 = "int8";
        static inline const std::string int16 = "int16";
        static inline const std::string int32 = "int32";
        static inline const std::string int64 = "int64";
        static inline const std::string uint8 = "uint8";
        static inline const std::string uint16 = "uint16";
        static inline const std::string uint32 = "uint32";
        static inline const std::string uint64 = "uint64";
        static inline const std::string float_ = "float";
        static inline const std::string double_ = "double";
        static inline const std::string string = "string";
        static inline const std::string data = "data";
        static inline const std::string enumer = "enumer";
        static inline const std::string list = "list";
        static inline const std::string obj = "obj";
        static inline const std::string objlist = "objlist";
        static inline const std::string llist = "llist";
        static inline const std::string lnlist = "lnlist";
        static inline const std::string elist = "elist";
        static inline const std::string ilist = "ilist";
        static inline const std::string gg = "gg";
        static inline const std::string opt = "opt";
        static inline const std::string un = "un";
        static inline const std::string vn = "vn";
    };

    bool bool_ = false;
    int8_t int8 = -4;
    int16_t int16 = 0;
    int32_t int32 = 0;
    int64_t int64 = 0LL;
    uint8_t uint8 = 22;
    uint16_t uint16 = 0;
    uint32_t uint32 = 211;
    uint64_t uint64 = 0ULL;
    float float_ = 1.1;
    double double_ = 5.3e-09;
    std::string string;
    podgen::Data data {std::byte{65},std::byte{66},std::byte{67},std::byte{68}};
    Choice enumer = Choice::CamelCase;
    std::vector<std::string> list;
    Name obj;
    std::vector<Name> objlist;
    std::vector<std::vector<int8_t>> llist;
    std::vector<std::vector<Name>> lnlist;
    std::vector<Choice> elist;
    std::vector<uint64_t> ilist;
    struct _gg {
        struct _field {
            static inline const std::string str = "str";
            static inline const std::string float_ = "float";
        };

        std::string str;
        float float_ = 0;

        bool operator==(const _gg& o) const {
            return str == o.str
                && float_ == o.float_;
        }

        bool operator!=(const _gg& o) const {
            return !operator==(o);
        }

        struct _Hasher {
            std::size_t operator()(const _gg& o) const {
                std::size_t s = 0;
                s = podgen::hashCombine(s, o.str);
                s = podgen::hashCombine(s, o.float_);
                return s;
            }

        };
    } gg;
    std::optional<std::string> opt;
    using _un = std::variant<bool, int32_t, uint32_t, std::string, std::vector<int8_t>, Choice, podgen::Data, Name, std::vector<Name>>;
    enum _un_which {
        un_unBool,
        un_unInt,
        un_unUint,
        un_unString,
        un_unList,
        un_unEnum,
        un_unData,
        un_unObj,
        un_unObjlist
    };

    _un un;
    _un_which un_which() const { return static_cast<_un_which>(un.index()); }
    using _vn = std::variant<int32_t, std::monostate, bool>;
    enum _vn_which {
        vn_vInt,
        vn_voyd,
        vn_unBool
    };

    _vn vn;
    _vn_which vn_which() const { return static_cast<_vn_which>(vn.index()); }

    bool operator==(const Simple& o) const {
        return bool_ == o.bool_
            && int8 == o.int8
            && int16 == o.int16
            && int32 == o.int32
            && int64 == o.int64
            && uint8 == o.uint8
            && uint16 == o.uint16
            && uint32 == o.uint32
            && uint64 == o.uint64
            && float_ == o.float_
            && double_ == o.double_
            && string == o.string
            && data == o.data
            && enumer == o.enumer
            && list == o.list
            && obj == o.obj
            && objlist == o.objlist
            && llist == o.llist
            && lnlist == o.lnlist
            && elist == o.elist
            && ilist == o.ilist
            && gg == o.gg
            && opt == o.opt
            && un == o.un
            && vn == o.vn;
    }

    bool operator!=(const Simple& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Simple& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.bool_);
            s = podgen::hashCombine(s, o.int8);
            s = podgen::hashCombine(s, o.int16);
            s = podgen::hashCombine(s, o.int32);
            s = podgen::hashCombine(s, o.int64);
            s = podgen::hashCombine(s, o.uint8);
            s = podgen::hashCombine(s, o.uint16);
            s = podgen::hashCombine(s, o.uint32);
            s = podgen::hashCombine(s, o.uint64);
            s = podgen::hashCombine(s, o.float_);
            s = podgen::hashCombine(s, o.double_);
            s = podgen::hashCombine(s, o.string);
            s = podgen::hashCombine(s, o.data);
            s = podgen::hashCombine(s, o.enumer);
            s = podgen::hashCombine(s, o.list);
            s = podgen::hashCombine(s, o.obj);
            s = podgen::hashCombine(s, o.objlist);
            s = podgen::hashCombine(s, o.llist);
            s = podgen::hashCombine(s, o.lnlist);
            s = podgen::hashCombine(s, o.elist);
            s = podgen::hashCombine(s, o.ilist);
            s = podgen::hashCombine(s, o.gg);
            s = podgen::hashCombine(s, o.opt);
            s = podgen::hashCombine(s, o.un);
            s = podgen::hashCombine(s, o.vn);
            return s;
        }

    };
};

using Variant = std::variant<std::string, int16_t, podgen::Data, Choice>;
enum Variant_which {
    Variant_string,
    Variant_int16,
    Variant_data,
    Variant_choice
};

using MaybeText = std::optional<std::vector<std::string>>;

struct Container {
    static inline const std::string TypeName = "Container";

    struct _field {
        static inline const std::string v = "v";
        static inline const std::string t = "t";
    };

    Variant v;
    MaybeText t;

    bool operator==(const Container& o) const {
        return v == o.v
            && t == o.t;
    }

    bool operator!=(const Container& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Container& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.v);
            s = podgen::hashCombine(s, o.t);
            return s;
        }

    };
};

struct Lists {
    static inline const std::string TypeName = "Lists";

    struct _field {
        static inline const std::string timestamp = "timestamp";
        static inline const std::string list = "list";
        static inline const std::string nlist = "nlist";
        static inline const std::string rlist = "rlist";
        static inline const std::string sset = "sset";
        static inline const std::string map = "map";
        static inline const std::string listoflists = "listoflists";
        static inline const std::string empty = "empty";
    };

    uint64_t timestamp = 0ULL;
    std::vector<std::string> list;
    std::unordered_set<Name, Name::_Hasher> nlist;
    std::list<Name> rlist;
    std::set<std::string> sset;
    std::unordered_map<std::string, uint64_t> map;
    std::list<std::list<std::string>> listoflists;
    std::vector<std::string> empty;

    bool operator==(const Lists& o) const {
        return timestamp == o.timestamp
            && list == o.list
            && nlist == o.nlist
            && rlist == o.rlist
            && sset == o.sset
            && map == o.map
            && listoflists == o.listoflists
            && empty == o.empty;
    }

    bool operator!=(const Lists& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Lists& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.timestamp);
            s = podgen::hashCombine(s, o.list);
            s = podgen::hashCombine(s, o.nlist);
            s = podgen::hashCombine(s, o.rlist);
            s = podgen::hashCombine(s, o.sset);
            s = podgen::hashCombine(s, o.map);
            s = podgen::hashCombine(s, o.listoflists);
            s = podgen::hashCombine(s, o.empty);
            return s;
        }

    };
};

struct Base {
    static inline const std::string TypeName = "Base";

    struct _field {
        static inline const std::string id = "id";
        static inline const std::string owner = "owner";
        static inline const std::string created = "created";
        static inline const std::string createdBy = "createdBy";
        static inline const std::string modified = "modified";
        static inline const std::string modifiedBy = "modifiedBy";
    };

    uint64_t id = 0ULL;
    std::string owner;
    uint64_t created = 0ULL;
    std::string createdBy;
    uint64_t modified = 0ULL;
    std::string modifiedBy;

    bool operator==(const Base& o) const {
        return id == o.id
            && owner == o.owner
            && created == o.created
            && createdBy == o.createdBy
            && modified == o.modified
            && modifiedBy == o.modifiedBy;
    }

    bool operator!=(const Base& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Base& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.id);
            s = podgen::hashCombine(s, o.owner);
            s = podgen::hashCombine(s, o.created);
            s = podgen::hashCombine(s, o.createdBy);
            s = podgen::hashCombine(s, o.modified);
            s = podgen::hashCombine(s, o.modifiedBy);
            return s;
        }

    };
};

struct Titled {
    static inline const std::string TypeName = "Titled";

    struct _field {
        static inline const std::string title = "title";
        static inline const std::string description = "description";
    };

    std::string title;
    std::string description;

    bool operator==(const Titled& o) const {
        return title == o.title
            && description == o.description;
    }

    bool operator!=(const Titled& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Titled& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.title);
            s = podgen::hashCombine(s, o.description);
            return s;
        }

    };
};

struct Entity : Titled, Base, ::inc::Info {
    static inline const std::string TypeName = "Entity";

    struct _field {
        static inline const std::string location = "location";
    };

    std::string location;

    bool operator==(const Entity& o) const {
        return id == o.id
            && owner == o.owner
            && created == o.created
            && createdBy == o.createdBy
            && modified == o.modified
            && modifiedBy == o.modifiedBy
            && title == o.title
            && description == o.description
            && info == o.info
            && location == o.location;
    }

    bool operator!=(const Entity& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Entity& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.id);
            s = podgen::hashCombine(s, o.owner);
            s = podgen::hashCombine(s, o.created);
            s = podgen::hashCombine(s, o.createdBy);
            s = podgen::hashCombine(s, o.modified);
            s = podgen::hashCombine(s, o.modifiedBy);
            s = podgen::hashCombine(s, o.title);
            s = podgen::hashCombine(s, o.description);
            s = podgen::hashCombine(s, o.info);
            s = podgen::hashCombine(s, o.location);
            return s;
        }

    };
};

struct Empty {
    static inline const std::string TypeName = "Empty";

    struct _field {
    };


    bool operator==(const Empty& o) const {
        return true;
    }

    bool operator!=(const Empty& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Empty& o) const {
            std::size_t s = 0;
            return s;
        }

    };
};

struct Sparse {
    static inline const std::string TypeName = "Sparse";

    struct _field {
        static inline const std::string id = "id";
        static inline const std::string name = "name";
    };

    uint64_t id = 0ULL;
    std::optional<std::string> name;

    bool operator==(const Sparse& o) const {
        return id == o.id
            && name == o.name;
    }

    bool operator!=(const Sparse& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Sparse& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.id);
            s = podgen::hashCombine(s, o.name);
            return s;
        }

    };
};

template <typename T, typename U>
struct Get {
    static inline const std::string TypeName = "Get";

    struct Result {
        static inline const std::string TypeName = "Result";

        struct _field {
            static inline const std::string msg = "msg";
        };

        std::string msg;

        bool operator==(const Result& o) const {
            return msg == o.msg;
        }

        bool operator!=(const Result& o) const {
            return !operator==(o);
        }

        struct _Hasher {
            std::size_t operator()(const Result& o) const {
                std::size_t s = 0;
                s = podgen::hashCombine(s, o.msg);
                return s;
            }

        };
    };

    template <typename E>
    struct Error {
        static inline const std::string TypeName = "Error";

        struct _field {
            static inline const std::string error = "error";
        };

        E error;

        bool operator==(const Error& o) const {
            return error == o.error;
        }

        bool operator!=(const Error& o) const {
            return !operator==(o);
        }

        struct _Hasher {
            std::size_t operator()(const Error<E>& o) const {
                std::size_t s = 0;
                s = podgen::hashCombine(s, o.error);
                return s;
            }

        };
    };

    struct _field {
        static inline const std::string getOne = "getOne";
        static inline const std::string getAll = "getAll";
    };

    T getOne;
    U getAll;

    bool operator==(const Get& o) const {
        return getOne == o.getOne
            && getAll == o.getAll;
    }

    bool operator!=(const Get& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Get<T, U>& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.getOne);
            s = podgen::hashCombine(s, o.getAll);
            return s;
        }

    };
};

struct User {
    static inline const std::string TypeName = "User";

    struct _field {
        static inline const std::string id = "id";
        static inline const std::string email = "email";
    };

    uint64_t id = 0ULL;
    std::string email;

    bool operator==(const User& o) const {
        return id == o.id
            && email == o.email;
    }

    bool operator!=(const User& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const User& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.id);
            s = podgen::hashCombine(s, o.email);
            return s;
        }

    };
};

struct Account {
    static inline const std::string TypeName = "Account";

    struct _field {
        static inline const std::string id = "id";
        static inline const std::string name = "name";
    };

    uint64_t id = 0ULL;
    std::string name;

    bool operator==(const Account& o) const {
        return id == o.id
            && name == o.name;
    }

    bool operator!=(const Account& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const Account& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.id);
            s = podgen::hashCombine(s, o.name);
            return s;
        }

    };
};

struct TextError {
    static inline const std::string TypeName = "TextError";

    struct _field {
        static inline const std::string msg = "msg";
    };

    std::string msg;

    bool operator==(const TextError& o) const {
        return msg == o.msg;
    }

    bool operator!=(const TextError& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const TextError& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.msg);
            return s;
        }

    };
};

struct IntError {
    static inline const std::string TypeName = "IntError";

    struct _field {
        static inline const std::string code = "code";
    };

    uint16_t code = 0;

    bool operator==(const IntError& o) const {
        return code == o.code;
    }

    bool operator!=(const IntError& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const IntError& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.code);
            return s;
        }

    };
};

struct GetInterface {
    static inline const std::string TypeName = "GetInterface";

    struct _field {
        static inline const std::string getUser = "getUser";
        static inline const std::string userResult = "userResult";
        static inline const std::string userError = "userError";
        static inline const std::string getAccount = "getAccount";
        static inline const std::string accountResult = "accountResult";
        static inline const std::string accountError = "accountError";
    };

    Get<User, std::vector<User>> getUser;
    Get<User, std::vector<User>>::Result userResult;
    Get<User, std::vector<User>>::Error<IntError> userError;
    Get<Account, std::vector<Account>> getAccount;
    Get<Account, std::vector<Account>>::Result accountResult;
    Get<Account, std::vector<Account>>::Error<TextError> accountError;

    bool operator==(const GetInterface& o) const {
        return getUser == o.getUser
            && userResult == o.userResult
            && userError == o.userError
            && getAccount == o.getAccount
            && accountResult == o.accountResult
            && accountError == o.accountError;
    }

    bool operator!=(const GetInterface& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const GetInterface& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.getUser);
            s = podgen::hashCombine(s, o.userResult);
            s = podgen::hashCombine(s, o.userError);
            s = podgen::hashCombine(s, o.getAccount);
            s = podgen::hashCombine(s, o.accountResult);
            s = podgen::hashCombine(s, o.accountError);
            return s;
        }

    };
};

template <typename V, typename W>
struct AltGet {
    static inline const std::string TypeName = "AltGet";

    struct _field {
        static inline const std::string get = "get";
    };

    Get<V, W> get;

    bool operator==(const AltGet& o) const {
        return get == o.get;
    }

    bool operator!=(const AltGet& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const AltGet<V, W>& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.get);
            return s;
        }

    };
};

struct AltInterface {
    static inline const std::string TypeName = "AltInterface";

    struct _field {
        static inline const std::string user = "user";
    };

    AltGet<User, std::vector<User>> user;

    bool operator==(const AltInterface& o) const {
        return user == o.user;
    }

    bool operator!=(const AltInterface& o) const {
        return !operator==(o);
    }

    struct _Hasher {
        std::size_t operator()(const AltInterface& o) const {
            std::size_t s = 0;
            s = podgen::hashCombine(s, o.user);
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

inline void _out(std::ostream& os, const Name::Part::_pug::_pugu2& o) {
    os << "pugu2{";
    os << "pugg=";
    _out(os, o.pugg);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::Part::_pug::_pugu2& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::Part::_pug& o) {
    os << "pug{";
    os << "pug1=";
    _out(os, o.pug1);
    os << ' ';
    os << "pug2=";
    test::_out(os, o.pug2);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::Part::_pug& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::Part& o) {
    os << "Part{";
    os << "pa=";
    _out(os, o.pa);
    os << ' ';
    os << "pb=";
    _out(os, o.pb);
    os << ' ';
    os << "pu=";
    test::_out(os, o.pu);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::Part& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::_g1::_g2::_g3& o) {
    os << "g3{";
    os << "n3=";
    _out(os, o.n3);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::_g1::_g2::_g3& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::_g1::_g2& o) {
    os << "g2{";
    os << "n2=";
    _out(os, o.n2);
    os << ' ';
    os << "g3=";
    _out(os, o.g3);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::_g1::_g2& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::_g1& o) {
    os << "g1{";
    os << "n1=";
    _out(os, o.n1);
    os << ' ';
    os << "g2=";
    _out(os, o.g2);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::_g1& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::_gu::_vc& o) {
    os << "vc{";
    os << "vcg1=";
    _out(os, o.vcg1);
    os << ' ';
    os << "vcg2=";
    _out(os, o.vcg2);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::_gu::_vc& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name::_gu& o) {
    os << "gu{";
    os << "gu1=";
    _out(os, o.gu1);
    os << ' ';
    os << "gu2=";
    _out(os, o.gu2);
    os << ' ';
    os << "gu3=";
    test::_out(os, o.gu3);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name::_gu& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Name& o) {
    os << "Name{";
    os << "id=";
    _out(os, o.id);
    os << ' ';
    os << "name=";
    _out(os, o.name);
    os << ' ';
    os << "g1=";
    _out(os, o.g1);
    os << ' ';
    os << "u=";
    test::_out(os, o.u);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Name& o) {
    _out(os, o);
    return os;
}

namespace Choice_bf0825cdafa9209c {
inline void _out(std::ostream& os, const Choice& o) {
    os << podgen::enumToName(o, Choice_names);
}

inline std::ostream& operator<<(std::ostream& os, const Choice& o) {
    _out(os, o);
    return os;
}

}
inline void _out(std::ostream& os, const Simple::_gg& o) {
    os << "gg{";
    os << "str=";
    _out(os, o.str);
    os << ' ';
    os << "float_=";
    _out(os, o.float_);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Simple::_gg& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Simple& o) {
    os << "Simple{";
    os << "bool_=";
    _out(os, o.bool_);
    os << ' ';
    os << "int8=";
    _out(os, o.int8);
    os << ' ';
    os << "int16=";
    _out(os, o.int16);
    os << ' ';
    os << "int32=";
    _out(os, o.int32);
    os << ' ';
    os << "int64=";
    _out(os, o.int64);
    os << ' ';
    os << "uint8=";
    _out(os, o.uint8);
    os << ' ';
    os << "uint16=";
    _out(os, o.uint16);
    os << ' ';
    os << "uint32=";
    _out(os, o.uint32);
    os << ' ';
    os << "uint64=";
    _out(os, o.uint64);
    os << ' ';
    os << "float_=";
    _out(os, o.float_);
    os << ' ';
    os << "double_=";
    _out(os, o.double_);
    os << ' ';
    os << "string=";
    _out(os, o.string);
    os << ' ';
    os << "data=";
    _out(os, o.data);
    os << ' ';
    os << "enumer=";
    _out(os, o.enumer);
    os << ' ';
    os << "list=";
    test::_out(os, o.list);
    os << ' ';
    os << "obj=";
    _out(os, o.obj);
    os << ' ';
    os << "objlist=";
    test::_out(os, o.objlist);
    os << ' ';
    os << "llist=";
    test::_out(os, o.llist);
    os << ' ';
    os << "lnlist=";
    test::_out(os, o.lnlist);
    os << ' ';
    os << "elist=";
    test::_out(os, o.elist);
    os << ' ';
    os << "ilist=";
    test::_out(os, o.ilist);
    os << ' ';
    os << "gg=";
    _out(os, o.gg);
    os << ' ';
    os << "opt=";
    test::_out(os, o.opt);
    os << ' ';
    os << "un=";
    test::_out(os, o.un);
    os << ' ';
    os << "vn=";
    test::_out(os, o.vn);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Simple& o) {
    _out(os, o);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Variant& o) {
test::    _out(os, o);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const MaybeText& o) {
test::    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Container& o) {
    os << "Container{";
    os << "v=";
    test::_out(os, o.v);
    os << ' ';
    os << "t=";
    test::_out(os, o.t);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Container& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Lists& o) {
    os << "Lists{";
    os << "timestamp=";
    _out(os, o.timestamp);
    os << ' ';
    os << "list=";
    test::_out(os, o.list);
    os << ' ';
    os << "nlist=";
    test::_out(os, o.nlist);
    os << ' ';
    os << "rlist=";
    test::_out(os, o.rlist);
    os << ' ';
    os << "sset=";
    test::_out(os, o.sset);
    os << ' ';
    os << "map=";
    test::_out(os, o.map);
    os << ' ';
    os << "listoflists=";
    test::_out(os, o.listoflists);
    os << ' ';
    os << "empty=";
    test::_out(os, o.empty);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Lists& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Base& o) {
    os << "Base{";
    os << "id=";
    _out(os, o.id);
    os << ' ';
    os << "owner=";
    _out(os, o.owner);
    os << ' ';
    os << "created=";
    _out(os, o.created);
    os << ' ';
    os << "createdBy=";
    _out(os, o.createdBy);
    os << ' ';
    os << "modified=";
    _out(os, o.modified);
    os << ' ';
    os << "modifiedBy=";
    _out(os, o.modifiedBy);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Base& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Titled& o) {
    os << "Titled{";
    os << "title=";
    _out(os, o.title);
    os << ' ';
    os << "description=";
    _out(os, o.description);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Titled& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Entity& o) {
    os << "Entity{";
    os << "id=";
    _out(os, o.id);
    os << ' ';
    os << "owner=";
    _out(os, o.owner);
    os << ' ';
    os << "created=";
    _out(os, o.created);
    os << ' ';
    os << "createdBy=";
    _out(os, o.createdBy);
    os << ' ';
    os << "modified=";
    _out(os, o.modified);
    os << ' ';
    os << "modifiedBy=";
    _out(os, o.modifiedBy);
    os << ' ';
    os << "title=";
    _out(os, o.title);
    os << ' ';
    os << "description=";
    _out(os, o.description);
    os << ' ';
    os << "info=";
    _out(os, o.info);
    os << ' ';
    os << "location=";
    _out(os, o.location);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Entity& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Empty& o) {
    os << "Empty{";
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Empty& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Sparse& o) {
    os << "Sparse{";
    os << "id=";
    _out(os, o.id);
    os << ' ';
    os << "name=";
    test::_out(os, o.name);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Sparse& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const User& o) {
    os << "User{";
    os << "id=";
    _out(os, o.id);
    os << ' ';
    os << "email=";
    _out(os, o.email);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const User& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Account& o) {
    os << "Account{";
    os << "id=";
    _out(os, o.id);
    os << ' ';
    os << "name=";
    _out(os, o.name);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Account& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const TextError& o) {
    os << "TextError{";
    os << "msg=";
    _out(os, o.msg);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const TextError& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const IntError& o) {
    os << "IntError{";
    os << "code=";
    _out(os, o.code);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const IntError& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Get<User, std::vector<User>>& o) {
    os << "Get{";
    os << "getOne=";
    _out(os, o.getOne);
    os << ' ';
    os << "getAll=";
    test::_out(os, o.getAll);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Get<User, std::vector<User>>& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Get<User, std::vector<User>>::Result& o) {
    os << "Result{";
    os << "msg=";
    _out(os, o.msg);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Get<User, std::vector<User>>::Result& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Get<User, std::vector<User>>::Error<IntError>& o) {
    os << "Error{";
    os << "error=";
    _out(os, o.error);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Get<User, std::vector<User>>::Error<IntError>& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Get<Account, std::vector<Account>>& o) {
    os << "Get{";
    os << "getOne=";
    _out(os, o.getOne);
    os << ' ';
    os << "getAll=";
    test::_out(os, o.getAll);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Get<Account, std::vector<Account>>& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Get<Account, std::vector<Account>>::Result& o) {
    os << "Result{";
    os << "msg=";
    _out(os, o.msg);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Get<Account, std::vector<Account>>::Result& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const Get<Account, std::vector<Account>>::Error<TextError>& o) {
    os << "Error{";
    os << "error=";
    _out(os, o.error);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Get<Account, std::vector<Account>>::Error<TextError>& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const GetInterface& o) {
    os << "GetInterface{";
    os << "getUser=";
    _out(os, o.getUser);
    os << ' ';
    os << "userResult=";
    _out(os, o.userResult);
    os << ' ';
    os << "userError=";
    _out(os, o.userError);
    os << ' ';
    os << "getAccount=";
    _out(os, o.getAccount);
    os << ' ';
    os << "accountResult=";
    _out(os, o.accountResult);
    os << ' ';
    os << "accountError=";
    _out(os, o.accountError);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const GetInterface& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const AltGet<User, std::vector<User>>& o) {
    os << "AltGet{";
    os << "get=";
    _out(os, o.get);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const AltGet<User, std::vector<User>>& o) {
    _out(os, o);
    return os;
}

inline void _out(std::ostream& os, const AltInterface& o) {
    os << "AltInterface{";
    os << "user=";
    _out(os, o.user);
    os << '}';
}

inline std::ostream& operator<<(std::ostream& os, const AltInterface& o) {
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
            test::_out_adl(os, o.first);
            os << " -> ";
            test::_out_adl(os, o.second);
            os << '}';
        } else {
            test::_out_adl(os, o);
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
    test::_out_adl(os, t.first);
    os << ", ";
    test::_out_adl(os, t.second);
    os << '}';
}

/// output any variant
template <typename... Args>
void _out(std::ostream& os, const std::variant<Args...>& v) {
    std::visit([&os](const auto& e) -> void { test::_out_adl(os, e); }, v);
}

template <typename T>
void _out(std::ostream& os, const std::optional<T>& o) {
    if (o) {
        test::_out_adl(os, *o);
    } else {
        os << "{}";
    }
}

template <typename T>
void _out_adl(std::ostream& os, const T& o) {
    if constexpr (podgen::is_ambiguous_adl<T>::value) {
        test::_out(os, o);
    } else {
        _out(os, o);
    }
}

}

namespace std {

template <> struct hash<::test::Name::Part::_pug::_pugu2> {
    std::size_t operator()(const ::test::Name::Part::_pug::_pugu2& o) const {
        return ::test::Name::Part::_pug::_pugu2::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::Part::_pug> {
    std::size_t operator()(const ::test::Name::Part::_pug& o) const {
        return ::test::Name::Part::_pug::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::Part> {
    std::size_t operator()(const ::test::Name::Part& o) const {
        return ::test::Name::Part::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::_g1::_g2::_g3> {
    std::size_t operator()(const ::test::Name::_g1::_g2::_g3& o) const {
        return ::test::Name::_g1::_g2::_g3::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::_g1::_g2> {
    std::size_t operator()(const ::test::Name::_g1::_g2& o) const {
        return ::test::Name::_g1::_g2::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::_g1> {
    std::size_t operator()(const ::test::Name::_g1& o) const {
        return ::test::Name::_g1::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::_gu::_vc> {
    std::size_t operator()(const ::test::Name::_gu::_vc& o) const {
        return ::test::Name::_gu::_vc::_Hasher()(o);
    }
};

template <> struct hash<::test::Name::_gu> {
    std::size_t operator()(const ::test::Name::_gu& o) const {
        return ::test::Name::_gu::_Hasher()(o);
    }
};

template <> struct hash<::test::Name> {
    std::size_t operator()(const ::test::Name& o) const {
        return ::test::Name::_Hasher()(o);
    }
};

template <> struct hash<::test::Simple::_gg> {
    std::size_t operator()(const ::test::Simple::_gg& o) const {
        return ::test::Simple::_gg::_Hasher()(o);
    }
};

template <> struct hash<::test::Simple> {
    std::size_t operator()(const ::test::Simple& o) const {
        return ::test::Simple::_Hasher()(o);
    }
};

template <> struct hash<::test::Container> {
    std::size_t operator()(const ::test::Container& o) const {
        return ::test::Container::_Hasher()(o);
    }
};

template <> struct hash<::test::Lists> {
    std::size_t operator()(const ::test::Lists& o) const {
        return ::test::Lists::_Hasher()(o);
    }
};

template <> struct hash<::test::Base> {
    std::size_t operator()(const ::test::Base& o) const {
        return ::test::Base::_Hasher()(o);
    }
};

template <> struct hash<::test::Titled> {
    std::size_t operator()(const ::test::Titled& o) const {
        return ::test::Titled::_Hasher()(o);
    }
};

template <> struct hash<::test::Entity> {
    std::size_t operator()(const ::test::Entity& o) const {
        return ::test::Entity::_Hasher()(o);
    }
};

template <> struct hash<::test::Empty> {
    std::size_t operator()(const ::test::Empty& o) const {
        return ::test::Empty::_Hasher()(o);
    }
};

template <> struct hash<::test::Sparse> {
    std::size_t operator()(const ::test::Sparse& o) const {
        return ::test::Sparse::_Hasher()(o);
    }
};

template <> struct hash<::test::User> {
    std::size_t operator()(const ::test::User& o) const {
        return ::test::User::_Hasher()(o);
    }
};

template <> struct hash<::test::Account> {
    std::size_t operator()(const ::test::Account& o) const {
        return ::test::Account::_Hasher()(o);
    }
};

template <> struct hash<::test::TextError> {
    std::size_t operator()(const ::test::TextError& o) const {
        return ::test::TextError::_Hasher()(o);
    }
};

template <> struct hash<::test::IntError> {
    std::size_t operator()(const ::test::IntError& o) const {
        return ::test::IntError::_Hasher()(o);
    }
};

template <> struct hash<::test::Get<::test::User, std::vector<::test::User>>> {
    std::size_t operator()(const ::test::Get<::test::User, std::vector<::test::User>>& o) const {
        return ::test::Get<::test::User, std::vector<::test::User>>::_Hasher()(o);
    }
};

template <> struct hash<::test::Get<::test::User, std::vector<::test::User>>::Result> {
    std::size_t operator()(const ::test::Get<::test::User, std::vector<::test::User>>::Result& o) const {
        return ::test::Get<::test::User, std::vector<::test::User>>::Result::_Hasher()(o);
    }
};

template <> struct hash<::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>> {
    std::size_t operator()(const ::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>& o) const {
        return ::test::Get<::test::User, std::vector<::test::User>>::Error<::test::IntError>::_Hasher()(o);
    }
};

template <> struct hash<::test::Get<::test::Account, std::vector<::test::Account>>> {
    std::size_t operator()(const ::test::Get<::test::Account, std::vector<::test::Account>>& o) const {
        return ::test::Get<::test::Account, std::vector<::test::Account>>::_Hasher()(o);
    }
};

template <> struct hash<::test::Get<::test::Account, std::vector<::test::Account>>::Result> {
    std::size_t operator()(const ::test::Get<::test::Account, std::vector<::test::Account>>::Result& o) const {
        return ::test::Get<::test::Account, std::vector<::test::Account>>::Result::_Hasher()(o);
    }
};

template <> struct hash<::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>> {
    std::size_t operator()(const ::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>& o) const {
        return ::test::Get<::test::Account, std::vector<::test::Account>>::Error<::test::TextError>::_Hasher()(o);
    }
};

template <> struct hash<::test::GetInterface> {
    std::size_t operator()(const ::test::GetInterface& o) const {
        return ::test::GetInterface::_Hasher()(o);
    }
};

template <> struct hash<::test::AltGet<::test::User, std::vector<::test::User>>> {
    std::size_t operator()(const ::test::AltGet<::test::User, std::vector<::test::User>>& o) const {
        return ::test::AltGet<::test::User, std::vector<::test::User>>::_Hasher()(o);
    }
};

template <> struct hash<::test::AltInterface> {
    std::size_t operator()(const ::test::AltInterface& o) const {
        return ::test::AltInterface::_Hasher()(o);
    }
};

}
