// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "polymorphic_simple_example.hpp"

#include <iostream>

void polymorphic_simple_demo() {
    std::unique_ptr<PolymorphicObjectExampleBase> example1 =
        std::make_unique<PolymorphicObjectExampleObject1>();
    auto e1_ptr = static_cast<PolymorphicObjectExampleObject1*>(example1.get());
    e1_ptr->i32_attr = 123;
    e1_ptr->flt_attr = 4.56f;
    e1_ptr->u64_attr = 7890;
    e1_ptr->dbl_attr = 7.89;
    std::string json1{};
    macrojson::object_to_json_str(example1, json1);
    std::cout << "polymorphic_simple_demo" << std::endl;
    std::cout << "Serialized JSON 1:" << std::endl;
    std::cout << json1 << std::endl;
    std::cout << std::endl;

    std::unique_ptr<PolymorphicObjectExampleBase> example1_output{};
    std::string error_descr{};
    auto code = macrojson::json_str_to_object(json1, example1_output, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading PolymorphicObjectExampleObject1 from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }

    std::unique_ptr<PolymorphicObjectExampleBase> example2 =
        std::make_unique<PolymorphicObjectExampleObject2>();
    auto e2_ptr = static_cast<PolymorphicObjectExampleObject2*>(example2.get());
    e2_ptr->i32_attr = 123;
    e2_ptr->flt_attr = 4.56f;
    e2_ptr->bool_attr = true;
    e2_ptr->str_attr = "example 2";
    std::string json2{};
    macrojson::object_to_json_str(example2, json2);
    std::cout << "Serialized JSON 2:" << std::endl;
    std::cout << json2 << std::endl;
    std::cout << std::endl;

    std::unique_ptr<PolymorphicObjectExampleBase> example2_output{};
    code = macrojson::json_str_to_object(json2, example2_output, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading PolymorphicObjectExampleObject2 from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }
}