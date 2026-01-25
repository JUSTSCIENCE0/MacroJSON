// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "rapidjson/filereadstream.h"

#include "example.hpp"
#include "read_example.hpp"

#include <iostream>

static inline bool read_json_from_file(const char* json_file, rapidjson::Document& doc) {
    FILE* fp = nullptr;
    auto err_code = fopen_s(&fp, json_file, "rb");
    if (err_code || !fp) {
        std::cerr << "Could not open file: " << json_file << std::endl;
        std::cerr << "Error code: " << err_code << std::endl;
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
