// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "ancillary.hpp"

TEST(WriterTests, SimpleExampleTest) {
    SimpleExample example{
        /* i32_attr */ 42,
        /* dbl_attr */ 3.14,
        /* u64_attr */ 1234567890123456789ULL,
        /* str_attr */ "Hello, MacroJSON!"
    };

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    check_object(example, doc);
}

TEST(WriterTests, Level1ExampleTest) {
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

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    check_object(example, doc);
}

TEST(WriterTests, Level2ExampleTest) {
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

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    check_object(example, doc);
}

TEST(WriterTests, ArrayExampleTest) {
    ArrayExample example{
       /* i32_arr */ {1, 2, 3, 4, 5},
       /* dbl_arr */ {1.1, 2.2, 3.3},
       /* u64_arr */ {10ULL, 20ULL, 30ULL},
       /* str_arr */ {"one", "two", "three"},
       /* smp_arr */ {
           {10, 0.1, 100ULL, "first"},
           {20, 0.2, 200ULL, "second"}
       },
       /* lvl1_arr */ {
           {1, 1.0, {1000, 0.01, 10000ULL, "lvl1_first"}},
           {2, 2.0, {2000, 0.02, 20000ULL, "lvl1_second"}}
       },
       /* lvl2_arr */ {
           {3000ULL,
            {3, 3.0, 30000ULL, "lvl2_smp_first"},
            {4, 4.0, {4000, 0.04, 40000ULL, "lvl2_lvl_first"}}},
           {6000ULL,
            {5, 5.0, 50000ULL, "lvl2_smp_second"},
            {6, 6.0, {6000, 0.06, 60000ULL, "lvl2_lvl_second"}}}
       }
    };

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    check_object(example, doc);
}

TEST(WriterTests, OptionalExampleTest) {
    OptionalExample example{
       /* i32_opt */ 42,
       /* dbl_opt */ std::nullopt,
       /* u64_opt */ 1234567890ULL,
       /* str_opt */ "Optional String",
       /* smp_opt */ SimpleExample{1, 1.1, 111ULL, "Smp Optional"},
       /* lvl1_opt */ std::nullopt,
       /* lvl2_opt */ Level2Example{
           9999999999ULL,
           {2, 2.2, 222ULL, "Lvl2 Smp Optional"},
           {3, 3.3, {333, 3.33, 3333ULL, "Lvl2 Lvl Optional"}}
       }
    };

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    check_object(example, doc);
}

TEST(WriterTests, EnumExampleTest) {
    EnumExample example{
       /* enum_attr */ ENUM_VALUE_TWO,
       /* enum_arr */ {ENUM_VALUE_ONE, ENUM_VALUE_TWO, ENUM_VALUE_THREE}
    };
    rapidjson::Document doc;
    doc.SetObject();
    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);
    check_object(example, doc);
}

TEST(WriterTests, PolymorphicExampleTest) {
    std::unique_ptr<BaseExample> example1 =
        std::make_unique<Object1Example>();
    auto& obj1 = static_cast<Object1Example&>(*example1);
    obj1.i32_attr = 10;
    obj1.flt_attr = 1.5f;
    obj1.u64_attr = 100ULL;
    obj1.dbl_attr = 2.5;
    rapidjson::Document doc1;
    doc1.SetObject();
    macrojson::write_to_json(nullptr, example1, doc1.GetAllocator(), doc1);
    check_object(obj1, doc1);

    std::shared_ptr<BaseExample> example2 =
        std::make_shared<Object2Example>();
    auto& obj2 = static_cast<Object2Example&>(*example2);
    obj2.i32_attr = 20;
    obj2.flt_attr = 2.5f;
    obj2.bool_attr = false;
    obj2.str_attr = "Polymorphic Object 2";
    rapidjson::Document doc2;
    doc2.SetObject();
    macrojson::write_to_json(nullptr, example2, doc2.GetAllocator(), doc2);
    check_object(obj2, doc2);
}