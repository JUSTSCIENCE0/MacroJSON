// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "example.hpp"

#include <iostream>

int main() {
    SimpleExample example{
        /* i32_attr */ 42,
        /* dbl_attr */ 3.14,
        /* u64_attr */ 1234567890123456789ULL,
        /* str_attr */ "Hello, MacroJSON!"
    };

    rapidjson::Document doc;
    doc.SetObject();

    write_to_json(nullptr, example, doc);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << buffer.GetString() << std::endl;

    return 0;
}
