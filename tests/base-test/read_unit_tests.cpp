// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "ancillary.hpp"

TEST(ReaderTests, SimpleExampleTest) {
    const char* json_str = R"({
        "i32_attr": 42,
        "dbl_attr": 3.14,
        "u64_attr": 1234567890123456789,
        "str_attr": "Hello, MacroJSON!"
    })";

    rapidjson::Document doc;
    doc.Parse(json_str);

    SimpleExample example;
    macrojson::read_from_json(nullptr, doc, example);

    check_object(example, doc);
}

TEST(ReaderTests, Level1ExampleTest) {
    const char* json_str = R"({
        "i32_attr": 7,
        "dbl_attr": 2.718,
        "obj_attr": {
            "i32_attr": 100,
            "dbl_attr": 1.618,
            "u64_attr": 9876543210987654321,
            "str_attr": "Nested MacroJSON!"
        }
    })";

    rapidjson::Document doc;
    doc.Parse(json_str);

    Level1Example example;
    macrojson::read_from_json(nullptr, doc, example);

    check_object(example, doc);
}

TEST(ReaderTests, Level2ExampleTest) {
    const char* json_str = R"({
        "u64_attr": 5555555555555555555,
        "smp_attr": {
            "i32_attr": 256,
            "dbl_attr": 0.5772,
            "u64_attr": 112233445566778899,
            "str_attr": "Level 2 Simple Example"
        },
        "lvl_attr": {
            "i32_attr": 128,
            "dbl_attr": 1.414,
            "obj_attr": {
                "i32_attr": 512,
                "dbl_attr": 2.236,
                "u64_attr": 998877665544332211,
                "str_attr": "Level 2 Nested Simple Example"
            }
        }
    })";

    rapidjson::Document doc;
    doc.Parse(json_str);

    Level2Example example;
    macrojson::read_from_json(nullptr, doc, example);

    check_object(example, doc);
}

TEST(ReaderTests, ArrayExampleTest) {
    const char* json_str = R"({
        "i32_arr": [1, 2, 3, 4, 5],
        "dbl_arr": [1.1, 2.2, 3.3],
        "u64_arr": [10000000000, 20000000000],
        "str_arr": ["one", "two", "three"],
        "smp_arr": [
            {
                "i32_attr": 10,
                "dbl_attr": 0.1,
                "u64_attr": 11111111111,
                "str_attr": "First"
            },
            {
                "i32_attr": 20,
                "dbl_attr": 0.2,
                "u64_attr": 22222222222,
                "str_attr": "Second"
            }
        ],
        "lvl1_arr": [
            {
                "i32_attr": 30,
                "dbl_attr": 0.3,
                "obj_attr": {
                    "i32_attr": 300,
                    "dbl_attr": 3.0,
                    "u64_attr": 33333333333,
                    "str_attr": "Nested First"
                }
            }
        ],
        "lvl2_arr": []
    })";

    rapidjson::Document doc;
    doc.Parse(json_str);

    ArrayExample example;
    macrojson::read_from_json(nullptr, doc, example);

    check_object(example, doc);
}
