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

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::cout << "simple_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << buffer.GetString() << std::endl;
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

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::cout << "level1_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << buffer.GetString() << std::endl;
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

    rapidjson::Document doc;
    doc.SetObject();

    macrojson::write_to_json(nullptr, example, doc.GetAllocator(), doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::cout << "level2_write_example" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << buffer.GetString() << std::endl;
    std::cout << std::endl;
}
