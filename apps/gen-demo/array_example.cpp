// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "array_example.hpp"

#include <iostream>

void array_demo() {
    ArrayExample example{
       /* i32_arr */ {
           1, 2, 3, 4, 5
       },
       /* dbl_arr */ {
           1.1, 2.2, 3.3, 4.4, 5.5
       },
       /* u64_arr */ {
           10, 20, 30, 40, 50
       },
       /* str_arr */ {
           "one", "two", "three", "four", "five"
       },
       /* smp_arr */ {
           { /* i32_attr */ 11, /* dbl_attr */ 1.11, /* u64_attr */ 111, /* str_attr */ "first" },
           { /* i32_attr */ 22, /* dbl_attr */ 2.22, /* u64_attr */ 222, /* str_attr */ "second" },
           { /* i32_attr */ 33, /* dbl_attr */ 3.33, /* u64_attr */ 333, /* str_attr */ "third" }
       },
       /* lvl1_arr */ {
           { /* i32_attr */ 101, /* dbl_attr */ 10.1, /* obj_attr */ { /* i32_attr */ 201, /* dbl_attr */ 20.2, /* u64_attr */ 203, /* str_attr */ "nested first" } },
           { /* i32_attr */ 102, /* dbl_attr */ 10.2, /* obj_attr */ { /* i32_attr */ 202, /* dbl_attr */ 20.4, /* u64_attr */ 204, /* str_attr */ "nested second" } }
       },
       /* lvl2_arr */ {
           { /* u64_attr */ 1001,
             /* smp_attr */ { /* i32_attr */ 301, /* dbl_attr */ 30.3, /* u64_attr */ 303, /* str_attr */ "level2 first" },
             /* lvl_attr */ { /* i32_attr */ 401, /* dbl_attr */ 40.4, /* obj_attr */ { /* i32_attr */ 501, /* dbl_attr */ 50.5, /* u64_attr */ 505, /* str_attr */ "level2 nested first" } }
           },
           { /* u64_attr */ 1002,
             /* smp_attr */ { /* i32_attr */ 302, /* dbl_attr */ 30.6, /* u64_attr */ 306, /* str_attr */ "level2 second" },
             /* lvl_attr */ { /* i32_attr */ 402, /* dbl_attr */ 40.8, /* obj_attr */ { /* i32_attr */ 502, /* dbl_attr */ 50.9, /* u64_attr */ 509, /* str_attr */ "level2 nested second" } }
           }
        }
    };

    std::string json{};
    macrojson::object_to_json_str(example, json);

    std::cout << "array_demo" << std::endl;
    std::cout << "Serialized JSON:" << std::endl;
    std::cout << json << std::endl;
    std::cout << std::endl;

    ArrayExample example_out{};
    std::string error_descr{};
    auto code = macrojson::json_str_to_object(json, example_out, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
       std::cerr << "Error reading ArrayExample from JSON." << std::endl;
       if (!error_descr.empty()) {
           std::cerr << error_descr << std::endl;
       }
       return;
    }
}