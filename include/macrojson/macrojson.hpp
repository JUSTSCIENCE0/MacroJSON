// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <cstdint>
#include <string>
#include <vector>

#define MJSON_CHECK_ERROR(code) \
    do { \
        macrojson::MJsonErrorCode err = code; \
        if (err != macrojson::MJsonErrorCode::E_MJSON_OK) { \
            return err; \
        } \
    } while (0)

namespace macrojson {
    using namespace rapidjson;

    // error codes
    enum MJsonErrorCode {
        E_MJSON_OK = 0,
        E_MJSON_NOT_EXISTS,
        E_MJSON_TYPE_MISMATCH
    };

    // Fundamental types writers
    static inline void write_to_json(
            const char* name, Value&& jval, Document::AllocatorType& alloc, Value& root) {
        if (name == nullptr) {
            root = std::move(jval);
        }
        else {
            Value jname;
            jname.SetString(name, alloc);
            root.AddMember(jname, jval, alloc);
        }
    }

    static inline void write_to_json(
            const char* name, const std::string& val, Document::AllocatorType& alloc, Value& root) {
        Value jval;
        jval.SetString(val.c_str(), alloc);
        write_to_json(name, std::move(jval), alloc, root);
    }

    template<typename T>
    void write_to_json(
            const char* name, std::vector<T> val, Document::AllocatorType& alloc, Value& root) {
        Value jarr(kArrayType);
        for (const auto& item : val) {
            Value jval;
            write_to_json(nullptr, item, alloc, jval);
            jarr.PushBack(jval, alloc);
        }
        write_to_json(name, std::move(jarr), alloc, root);
    }

#define JSON_WRITER(type, setter) \
    static inline void write_to_json( \
            const char* name, type val, Document::AllocatorType& alloc, Value& root) { \
        Value jval; \
        jval.setter(val); \
        write_to_json(name, std::move(jval), alloc, root); \
    }

    JSON_WRITER(int32_t,  SetInt)
    JSON_WRITER(int64_t,  SetInt64)
    JSON_WRITER(uint32_t, SetUint)
    JSON_WRITER(uint64_t, SetUint64)

    JSON_WRITER(float,  SetFloat)
    JSON_WRITER(double, SetDouble)

#undef JSON_WRITER

    // Fundamental types readers
    template<typename T>
    MJsonErrorCode read_from_json(const char* name, const Value& root, std::vector<T>& val) {
        val.clear();

        if (name && !root.HasMember(name)) {
            return E_MJSON_NOT_EXISTS;
        }
        const Value& jval = name ? root[name] : root;
        if (!jval.IsArray()) {
            return E_MJSON_TYPE_MISMATCH;
        }

        for (const auto& item : jval.GetArray()) {
            T elem{};
            MJSON_CHECK_ERROR(read_from_json(nullptr, item, elem));
            val.emplace_back(std::move(elem));
        }

        return E_MJSON_OK;
    }

#define JSON_READER(type, checker, getter) \
    static inline MJsonErrorCode read_from_json( \
            const char* name, const Value& root, type& val) { \
        if (name && !root.HasMember(name)) { \
            return E_MJSON_NOT_EXISTS; \
        } \
        const Value& jval = name ? root[name] : root; \
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