// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#ifdef WIN32
# define _CRT_SECURE_NO_WARNINGS
#endif

#include "rapidjson/filereadstream.h"

#include "example.hpp"
#include "read_example.hpp"

#include <iostream>

static inline bool read_json_from_file(const char* json_file, rapidjson::Document& doc) {
    FILE* fp = fopen(json_file, "rb");
    if (!fp) {
        std::cerr << "Could not open file: " << json_file << std::endl;
        return false;
    }
    char readBuffer[1024];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    doc.ParseStream(is);
    fclose(fp);
    if (doc.HasParseError()) {
        std::cerr << "Error parsing JSON file: " << json_file << std::endl;
        return false;
    }
    return true;
}

void simple_read_example() {
    SimpleExample example{};
    std::string error_descr{};
    auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "simple_example.json", example, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading SimpleExample from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }

    std::cout << "simple_read_example" << std::endl;
    std::cout << "Deserialized JSON (part):" << std::endl;
    std::cout << "i32_attr = " << example.i32_attr << std::endl;
    std::cout << "str_attr = " << example.str_attr << std::endl;
    std::cout << std::endl;
}

void level1_read_example() {
    Level1Example example{};
    std::string error_descr{};
    auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "level1_example.json", example, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading SimpleExample from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }

    std::cout << "level1_read_example" << std::endl;
    std::cout << "Deserialized JSON (part):" << std::endl;
    std::cout << "i32_attr = " << example.i32_attr << std::endl;
    std::cout << "obj_attr.str_attr = " << example.obj_attr.str_attr << std::endl;
    std::cout << std::endl;
}

void level2_read_example() {
    Level2Example example{};
    std::string error_descr{};
    auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "level2_example.json", example, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading SimpleExample from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }

    std::cout << "level2_read_example" << std::endl;
    std::cout << "Deserialized JSON (part):" << std::endl;
    std::cout << "u64_attr = " << example.u64_attr << std::endl;
    std::cout << "smp_attr.str_attr = " << example.smp_attr.str_attr << std::endl;
    std::cout << "lvl_attr.i32_attr = " << example.lvl_attr.i32_attr << std::endl;
    std::cout << "lvl_attr.obj_attr.str_attr = " <<
        example.lvl_attr.obj_attr.str_attr << std::endl;
    std::cout << std::endl;
}

void array_read_example() {
    ArrayExample example{};
    std::string error_descr{};
    auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "array_example.json", example, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
       std::cerr << "Error reading SimpleExample from JSON." << std::endl;
       if (!error_descr.empty()) {
           std::cerr << error_descr << std::endl;
       }
       return;
    }

    std::cout << "array_read_example" << std::endl;
}

void enum_read_example() {
    EnumExample example{};
    std::string error_descr{};
    auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "enum_example.json", example, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
       std::cerr << "Error reading EnumExample from JSON." << std::endl;
       if (!error_descr.empty()) {
           std::cerr << error_descr << std::endl;
       }
       return;
    }
    std::cout << "enum_read_example" << std::endl;
    std::cout << "Deserialized JSON (part):" << std::endl;
    std::cout << "enum_attr = " << static_cast<int>(example.enum_attr) << std::endl;
    std::cout << "enum_arr size = " << example.enum_arr.size() << std::endl;
    std::cout << std::endl;
}

void print(const Object1Example& o1) {
    std::cout << "Deserialized Object1Example JSON:" << std::endl;
    std::cout << "\ti32_attr = " << o1.i32_attr << std::endl;
    std::cout << "\tflt_attr = " << o1.flt_attr << std::endl;
    std::cout << "\tu64_attr = " << o1.u64_attr << std::endl;
    std::cout << "\tdbl_attr = " << o1.dbl_attr << std::endl;
}

void print(const Object2Example& o2) {
    std::cout << "Deserialized Object2Example JSON:" << std::endl;
    std::cout << "\ti32_attr = " << o2.i32_attr << std::endl;
    std::cout << "\tflt_attr = " << o2.flt_attr << std::endl;
    std::cout << "\tstr_attr = " << o2.str_attr << std::endl;
}

void polymorphic_simple_read_example() {
    std::cout << "polymorphic_simple_read_example" << std::endl;

    std::unique_ptr<BaseExample> example1{};
    std::string error_descr{};
    auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "object1_example.json", example1, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading Object1Example from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }
    print(static_cast<Object1Example&>(*example1));

    std::shared_ptr<BaseExample> example2{};
    code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "object2_example.json", example2, error_descr);
    if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
        std::cerr << "Error reading Object2Example from JSON." << std::endl;
        if (!error_descr.empty()) {
            std::cerr << error_descr << std::endl;
        }
        return;
    }
    print(static_cast<Object2Example&>(*example2));
    std::cout << std::endl;
}

void print(const BaseExample& base) {
    switch (base.type) {
    case EnumTypesExample::E_TYPE_OBJECT_1: {
        auto& obj1 = static_cast<const Object1Example&>(base);
        print(obj1);
        break;
    }
    case EnumTypesExample::E_TYPE_OBJECT_2: {
        auto& obj2 = static_cast<const Object2Example&>(base);
        print(obj2);
        break;
    }
    default:
        std::cout << "Unknown plm_attr type." << std::endl;
        break;
    }
}

void polymorphic_complex_read_example() {
    //PolymorphicExample example{};
    //std::string error_descr{};
    //auto code = macrojson::json_file_to_object(JSON_EXAMPLES_DIRECTORY "polymorphic_example.json", example, error_descr);
    //if (code != macrojson::MJsonErrorCode::E_MJSON_OK) {
    //    std::cerr << "Error reading PolymorphicExample from JSON." << std::endl;
    //    if (!error_descr.empty()) {
    //        std::cerr << error_descr << std::endl;
    //    }
    //    return;
    //}

    //std::cout << "polymorphic_complex_read_example" << std::endl;
    //std::cout << "Deserialized JSON:" << std::endl;
    //if (example.plm_attr) {
    //    std::cout << "plm_attr: " << std::endl;
    //    print(*example.plm_attr);
    //}

    //std::cout << "plm_arr size = " << example.plm_arr.size() << std::endl;
    //std::cout << "plm_arr: " << std::endl;
    //for (const auto& item : example.plm_arr) {
    //    print(*item);
    //}
    //std::cout << std::endl;
}
