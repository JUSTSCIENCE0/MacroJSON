// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "polymorphic_complex_example.hpp"

#include <iostream>

void polymorphic_complex_demo() {
    PolymorphicExample example{};
    example.plm_attr = std::make_unique<PolymorphicAttributeObject1>();
    auto e1_ptr = static_cast<PolymorphicAttributeObject1*>(example.plm_attr.get());
    e1_ptr->i32_attr = 321;
    e1_ptr->flt_attr = 6.54f;
    e1_ptr->u64_attr = 543210;
    e1_ptr->dbl_attr = 9.87;

    example.plm_arr.push_back(std::make_unique<PolymorphicObjectExampleObject1>());
    auto e2_ptr = static_cast<PolymorphicObjectExampleObject1*>(example.plm_arr.back().get());
    e2_ptr->i32_attr = 111;
    e2_ptr->flt_attr = 1.11f;
    e2_ptr->u64_attr = 111111;
    e2_ptr->dbl_attr = 1.11;

    example.plm_arr.push_back(std::make_unique<PolymorphicObjectExampleObject2>());
    auto e3_ptr = static_cast<PolymorphicObjectExampleObject2*>(example.plm_arr.back().get());
    e3_ptr->i32_attr = 222;
    e3_ptr->flt_attr = 2.22f;
    e3_ptr->bool_attr = false;
    e3_ptr->str_attr = "example in array";

    std::string json{};
    macrojson::object_to_json_str(example, json);
    std::cout << "polymorphic_complex_demo" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;
    std::cout << std::endl;

    PolymorphicExample example_output{};
    std::string error_descr{};
    auto code = macrojson::json_str_to_object(json, example_output, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
       std::cerr << "Error reading PolymorphicExample from JSON." << std::endl;
       if (!error_descr.empty()) {
           std::cerr << error_descr << std::endl;
       }
       return;
    }
}