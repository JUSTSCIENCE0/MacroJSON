// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <cstdint>
#include <string>

namespace macrojson {
    using namespace rapidjson;

    // error codes
    enum MJsonErrorCode {
        E_MJSON_OK = 0,
        E_MJSON_NOT_EXISTS,
        E_MJSON_TYPE_MISMATCH
    };

    // Fundamental types writers
    static inline void write_to_json(const char* name, Value&& jval, Document& d) {
        Value jname;
        jname.SetString(name, d.GetAllocator());
        d.AddMember(jname, jval, d.GetAllocator());
    }

    static inline void write_to_json(const char* name, const std::string& val, Document& d) {
        Value jval;
        jval.SetString(val.c_str(), d.GetAllocator());
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
#define JSON_READER(type, checker, getter) \
    static inline MJsonErrorCode read_from_json(const Value& obj, const char* name, type& val) { \
        if (!obj.HasMember(name)) { \
            return E_MJSON_NOT_EXISTS; \
        } \
        const Value& jval = obj[name]; \
        if (!jval.checker()) { \
            return E_MJSON_TYPE_MISMATCH; \
        } \
        val = jval.getter(); \
        return E_MJSON_OK; \
    }

    JSON_READER(std::string, IsString, GetString)
    JSON_READER(int32_t,     IsInt,    GetInt)
    JSON_READER(int64_t,     IsInt64,  GetInt64)
    JSON_READER(uint32_t,    IsUint,   GetUint)
    JSON_READER(uint64_t,    IsUint64, GetUint64)
    JSON_READER(float,       IsFloat,  GetFloat)
    JSON_READER(double,      IsDouble, GetDouble)

#undef JSON_READER

    // Fundamental types validators
    // TODO

    // Fundamental types schemas
    // TODO
}