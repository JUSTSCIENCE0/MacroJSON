// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "example.hpp"
#include "schema_example.hpp"

#include <iostream>

void simple_schema_example() {
    auto schema = macrojson::get_json_schema_str<SimpleExample>(
        "http://example.com/schemas/simple_example.json");
    std::cout << "simple_schema_example" << std::endl;
    std::cout << "Generated JSON Schema:" << std::endl;
    std::cout << schema << std::endl;
    std::cout << std::endl;
}

