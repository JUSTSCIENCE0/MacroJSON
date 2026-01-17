// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "example.hpp"

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
