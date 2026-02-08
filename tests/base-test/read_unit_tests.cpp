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
    ASSERT_EQ(
        macrojson::read_from_json(nullptr, doc, example),
        macrojson::MJsonErrorCode::E_MJSON_OK
    );

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
    ASSERT_EQ(
        macrojson::read_from_json(nullptr, doc, example),
        macrojson::MJsonErrorCode::E_MJSON_OK
    );

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
    ASSERT_EQ(
        macrojson::read_from_json(nullptr, doc, example),
        macrojson::MJsonErrorCode::E_MJSON_OK
    );

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

    //ArrayExample example;
    //ASSERT_EQ(
    //    macrojson::read_from_json(nullptr, doc, example),
    //    macrojson::MJsonErrorCode::E_MJSON_OK
    //);

    //check_object(example, doc);
}

TEST(ReaderTests, OptionalExampleTest) {
    const char* json_str = R"({
        "i32_opt": 99,
        "u64_opt": 99999999999,
        "str_opt": "Optional String",
        "smp_opt": {
            "i32_attr": 123,
            "dbl_attr": 4.56,
            "u64_attr": 65432109876,
            "str_attr": "Optional SimpleExample"
        },
        "lvl1_opt": null,
        "lvl2_opt": {
            "u64_attr": 88888888888,
            "smp_attr": {
                "i32_attr": 321,
                "dbl_attr": 6.54,
                "u64_attr": 87654321099,
                "str_attr": "Level2 Optional SimpleExample"
            },
            "lvl_attr": {
                "i32_attr": 111,
                "dbl_attr": 1.11,
                "obj_attr": {
                    "i32_attr": 222,
                    "dbl_attr": 2.22,
                    "u64_attr": 33333333333,
                    "str_attr": "Level2 Optional Nested SimpleExample"
                }
            }
        }
    })";

    rapidjson::Document doc;
    doc.Parse(json_str);

    //OptionalExample example;
    //ASSERT_EQ(
    //    macrojson::read_from_json(nullptr, doc, example),
    //    macrojson::MJsonErrorCode::E_MJSON_OK
    //);

    //check_object(example, doc);
}

TEST(ReaderTests, EnumExampleTest) {
    const char* json_str = R"({
        "enum_attr": "TWO",
        "enum_arr": ["ONE", "TWO", "THREE", "TWO"]
    })";
    rapidjson::Document doc;
    doc.Parse(json_str);
    //EnumExample example;
    //ASSERT_EQ(
    //    macrojson::read_from_json(nullptr, doc, example),
    //    macrojson::MJsonErrorCode::E_MJSON_OK
    //);
    //check_object(example, doc);
}

TEST(ReaderTests, PolymorphicExampleTest) {
    const char* json_str1 = R"({
        "type": "object1",
        "i32_attr": 123,
        "flt_attr": 4.559999942779541,
        "u64_attr": 7890123456789012345,
        "dbl_attr": 7.89
    })";
    rapidjson::Document doc1;
    doc1.Parse(json_str1);
    std::unique_ptr<BaseExample> example1{};
    ASSERT_EQ(
        macrojson::read_from_json(nullptr, doc1, example1),
        macrojson::MJsonErrorCode::E_MJSON_OK
    );
    check_object(static_cast<Object1Example&>(*example1), doc1);

    const char* json_str2 = R"({
        "type": "object2",
        "i32_attr": 123,
        "flt_attr": 4.559999942779541,
        "str_attr": "example 2"
    })";
    rapidjson::Document doc2;
    doc2.Parse(json_str2);
    std::shared_ptr<BaseExample> example2{};
    ASSERT_EQ(
        macrojson::read_from_json(nullptr, doc2, example2),
        macrojson::MJsonErrorCode::E_MJSON_OK
    );
    check_object(static_cast<Object2Example&>(*example2), doc2);
}
