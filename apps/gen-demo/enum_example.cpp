// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "enum_example.hpp"

#include <iostream>

void enum_demo() {
    EnumExample example{
       /* enum_attr */ SimpleEnum::E_SIMPLEENUM_ONE,
       /* enum_arr */ {
           SimpleEnum::E_SIMPLEENUM_ONE,
           SimpleEnum::E_SIMPLEENUM_TWO,
           SimpleEnum::E_SIMPLEENUM_THREE
       }
    };
    std::string json{};
    macrojson::object_to_json_str(example, json);
    std::cout << "enum_demo" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;

    EnumExample example_output{};
    std::string error_descr{};
    auto code = macrojson::json_str_to_object(json, example_output, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
       std::cerr << "Error reading EnumExample from JSON." << std::endl;
       if (!error_descr.empty()) {
           std::cerr << error_descr << std::endl;
       }
       return;
    }
    std::cout << "Deserialized JSON (part):" << std::endl;
    std::cout << "enum_attr = " << static_cast<int>(example.enum_attr) << std::endl;
    std::cout << "enum_arr size = " << example.enum_arr.size() << std::endl;
    std::cout << std::endl;
}