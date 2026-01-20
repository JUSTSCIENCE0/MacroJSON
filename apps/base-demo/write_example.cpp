// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "example.hpp"
#include "write_example.hpp"

#include <iostream>

void simple_write_example() {
    SimpleExample example{
        /* i32_attr */ 42,
        /* dbl_attr */ 3.14,
        /* u64_attr */ 1234567890123456789ULL,
        /* str_attr */ "Hello, MacroJSON!"
    };

    std::string json{};
    macrojson::object_to_json_str(example, json);

    std::cout << "simple_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;
    std::cout << std::endl;
}

void level1_write_example() {
    Level1Example example{
        /* i32_attr */ 7,
        /* dbl_attr */ 2.718,
        /* obj_attr */ {
            /* i32_attr */ 100,
            /* dbl_attr */ 1.618,
            /* u64_attr */ 9876543210987654321ULL,
            /* str_attr */ "Nested MacroJSON!"
        }
    };

    std::string json{};
    macrojson::object_to_json_str(example, json);

    std::cout << "level1_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;
    std::cout << std::endl;
}

void level2_write_example() {
    Level2Example example{
        /* u64_attr */ 5555555555555555555ULL,
        /* smp_attr */ {
            /* i32_attr */ 256,
            /* dbl_attr */ 0.5772,
            /* u64_attr */ 112233445566778899ULL,
            /* str_attr */ "Level 2 Simple Example"
        },
        /* lvl_attr */ {
            /* i32_attr */ 128,
            /* dbl_attr */ 1.414,
            /* obj_attr */ {
                /* i32_attr */ 512,
                /* dbl_attr */ 2.236,
                /* u64_attr */ 998877665544332211ULL,
                /* str_attr */ "Level 2 Nested Simple Example"
            }
        }
    };

    std::string json{};
    macrojson::object_to_json_str(example, json);

    std::cout << "level2_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;
    std::cout << std::endl;
}

void array_write_example() {
    ArrayExample example{
        /* i32_arr */ {
            1, 2, 3, 4, 5
        },
        /* dbl_arr */ {
            1.1, 2.2, 3.3, 4.4, 5.5
        },
        /* u64_arr */ {
            10ULL, 20ULL, 30ULL, 40ULL, 50ULL
        },
        /* str_arr */ {
            "one", "two", "three", "four", "five"
        },
        /* smp_arr */ {
            { /* i32_attr */ 11, /* dbl_attr */ 1.11, /* u64_attr */ 111ULL, /* str_attr */ "first" },
            { /* i32_attr */ 22, /* dbl_attr */ 2.22, /* u64_attr */ 222ULL, /* str_attr */ "second" },
            { /* i32_attr */ 33, /* dbl_attr */ 3.33, /* u64_attr */ 333ULL, /* str_attr */ "third" }
        },
        /* lvl1_arr */ {
            { /* i32_attr */ 101, /* dbl_attr */ 10.1, /* obj_attr */ { /* i32_attr */ 201, /* dbl_attr */ 20.2, /* u64_attr */ 202ULL, /* str_attr */ "nested first" } },
            { /* i32_attr */ 102, /* dbl_attr */ 10.2, /* obj_attr */ { /* i32_attr */ 202, /* dbl_attr */ 20.4, /* u64_attr */ 204ULL, /* str_attr */ "nested second" } }
        },
        /* lvl2_arr */ {
            { /* u64_attr */ 1001ULL,
              /* smp_attr */ { /* i32_attr */ 301, /* dbl_attr */ 30.3, /* u64_attr */ 303ULL, /* str_attr */ "level2 first" },
              /* lvl_attr */ { /* i32_attr */ 401, /* dbl_attr */ 40.4, /* obj_attr */ { /* i32_attr */ 501, /* dbl_attr */ 50.5, /* u64_attr */ 505ULL, /* str_attr */ "level2 nested first" } }
            },
            { /* u64_attr */ 1002ULL,
              /* smp_attr */ { /* i32_attr */ 302, /* dbl_attr */ 30.6, /* u64_attr */ 306ULL, /* str_attr */ "level2 second" },
              /* lvl_attr */ { /* i32_attr */ 402, /* dbl_attr */ 40.8, /* obj_attr */ { /* i32_attr */ 502, /* dbl_attr */ 50.9, /* u64_attr */ 509ULL, /* str_attr */ "level2 nested second" } }
            }
        }
    };

    std::string json{};
    macrojson::object_to_json_str(example, json);

    std::cout << "array_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;
    std::cout << std::endl;
}
