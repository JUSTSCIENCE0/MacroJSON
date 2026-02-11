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

void level1_schema_example() {
    auto schema = macrojson::get_json_schema_str<Level1Example>(
        "http://example.com/schemas/level1_example.json");
    std::cout << "level1_schema_example" << std::endl;
    std::cout << "Generated JSON Schema:" << std::endl;
    std::cout << schema << std::endl;
    std::cout << std::endl;
}

void level2_schema_example() {
    auto schema = macrojson::get_json_schema_str<Level2Example>(
        "http://example.com/schemas/level2_example.json");
    std::cout << "level2_schema_example" << std::endl;
    std::cout << "Generated JSON Schema:" << std::endl;
    std::cout << schema << std::endl;
    std::cout << std::endl;
}

void optional_schema_example() {
    auto schema = macrojson::get_json_schema_str<OptionalExample>(
        "http://example.com/schemas/optional_example.json");
    std::cout << "optional_schema_example" << std::endl;
    std::cout << "Generated JSON Schema:" << std::endl;
    std::cout << schema << std::endl;
    std::cout << std::endl;
}

void array_schema_example() {
    auto schema = macrojson::get_json_schema_str<ArrayExample>(
        "http://example.com/schemas/array_example.json");
    std::cout << "array_schema_example" << std::endl;
    std::cout << "Generated JSON Schema:" << std::endl;
    std::cout << schema << std::endl;
    std::cout << std::endl;
}
