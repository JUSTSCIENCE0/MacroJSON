// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <cstdint>

namespace macrojson {
    using namespace rapidjson;

    // Fundamental types writers
    static inline void write_to_json(const char* name, Value&& jval, Document& d) {
        Value jname;
        jname.SetString(name, d.GetAllocator());
        d.AddMember(jname, jval, d.GetAllocator());
    }

    static inline void write_to_json(const char* name, const char* val, Document& d) {
        Value jval;
        jval.SetString(val, d.GetAllocator());
        write_to_json(name, std::move(jval), d);
    }

#define JSON_WRITER(type, setter) \
    static inline void write_to_json(const char* name, type val, Document& d) { \
        Value jval; \
        jval.setter(val); \
        write_to_json(name, std::move(jval), d); \
    }

    JSON_WRITER(int32_t,  SetInt)
    JSON_WRITER(int64_t,  SetInt64)
    JSON_WRITER(uint32_t, SetUint)
    JSON_WRITER(uint64_t, SetUint64)

    JSON_WRITER(float,  SetFloat)
    JSON_WRITER(double, SetDouble)

#undef JSON_WRITER

    // Fundamental types readers


    // Fundamental types schemas

}