// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <type_traits>
#include <limits>

#include <fstream>
#include <filesystem>
#include <cassert>

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
        E_MJSON_TYPE_MISMATCH,
        E_MJSON_FILE_ERROR,
        E_MJSON_PARSE_FAILED,
        E_MJSON_UNKNOWN_ENUM,
        E_MJSON_UNSUPPORTED_OBJECT_TYPE
    };

    // helpers
    // trait to detect std::optional<T>
    template<typename T> struct is_std_optional : std::false_type {};
    template<typename U> struct is_std_optional<std::optional<U>> : std::true_type {};
    template<typename T> inline constexpr bool is_std_optional_v = is_std_optional<T>::value;
    // trait to detect std::vector<T>
    template<typename T> struct is_std_vector : std::false_type {};
    template<typename U> struct is_std_vector<std::vector<U>> : std::true_type {};
    template<typename T> inline constexpr bool is_std_vector_v = is_std_vector<T>::value;
    // trait to detect std::unique_ptr<T>
    template<typename T> struct is_std_unique_ptr : std::false_type {};
    template<typename U> struct is_std_unique_ptr<std::unique_ptr<U>> : std::true_type {};
    template<typename T> inline constexpr bool is_std_unique_ptr_v = is_std_unique_ptr<T>::value;
    // trait to detect std::shared_ptr<T>
    template<typename T> struct is_std_shared_ptr : std::false_type {};
    template<typename U> struct is_std_shared_ptr<std::shared_ptr<U>> : std::true_type {};
    template<typename T> inline constexpr bool is_std_shared_ptr_v = is_std_shared_ptr<T>::value;

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

    template<typename T>
    void write_to_json(
            const char* name, const std::optional<T>& val, Document::AllocatorType& alloc, Value& root);

    template<typename T>
    void write_to_json(
            const char* name, const std::vector<T>& val, Document::AllocatorType& alloc, Value& root);

    template<typename T>
    void write_to_json(
            const char* name, const std::unique_ptr<T>& val, Document::AllocatorType& alloc, Value& root);

    template<typename T>
    void write_to_json(
            const char* name, const std::shared_ptr<T>& val, Document::AllocatorType& alloc, Value& root);

    // Fundamental types readers
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

    template<typename T>
    MJsonErrorCode read_from_json(const char* name, const Value& root, std::vector<T>& val);

    template<typename T>
    MJsonErrorCode read_from_json(const char* name, const Value& root, std::optional<T>& val);

    // Fundamental types validators
    // validation descriptions
    struct StringRegex {
        std::string pattern = "";
    };
    struct StringLength {
        int minLength = -1;
        int maxLength = -1;
    };
    template<typename NUM,
             std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
    struct MultipleOf {
        NUM multipleOf{};
    };
    template<typename NUM,
             std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
    struct Range {
        NUM minimum = std::numeric_limits<NUM>::min();
        bool isExclusiveMinimum = false;
        NUM maximum = std::numeric_limits<NUM>::max();
        bool isExclusiveMaximum = false;
    };
    struct ArrayParams {
        int minItems = 0;
        int maxItems = -1;
        bool uniqueItems = false;
    };

    // Fundamental types schemas validation fields
    template<typename T>
    void add_validation_fields(Document::AllocatorType&, Value&) {}

    template<typename T, typename... Validators>
    void add_validation_fields(
            Document::AllocatorType& alloc, Value& schema, Validators... validators);

    template<
        typename T,
        typename... Validators,
        std::enable_if_t<std::is_same_v<T, std::string>, bool> = true>
    void add_validation_fields(
            Document::AllocatorType& alloc, Value& obj, const StringRegex& regex, Validators... validators) {
        Value jregexp;
        jregexp.SetString(regex.pattern.c_str(), alloc);
        obj.AddMember("pattern", jregexp, alloc);

        add_validation_fields<T>(alloc, obj, validators...);
    }

    template<
        typename T,
        typename... Validators,
        std::enable_if_t<std::is_same_v<T, std::string>, bool> = true>
    void add_validation_fields(
            Document::AllocatorType& alloc, Value& obj, const StringLength& regex, Validators... validators) {
        obj.AddMember("minLength", regex.minLength, alloc);
        obj.AddMember("maxLength", regex.maxLength, alloc);

        add_validation_fields<T>(alloc, obj, validators...);
    }

    template<
        typename NUM,
        typename... Validators,
        std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
    void add_validation_fields(
            Document::AllocatorType& alloc, Value& obj, const MultipleOf<NUM>& multipleOf, Validators... validators) {
        obj.AddMember("multipleOf", multipleOf.multipleOf, alloc);

        add_validation_fields<NUM>(alloc, obj, validators...);
    }

    template<
        typename NUM,
        typename... Validators,
        std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
    void add_validation_fields(
            Document::AllocatorType& alloc, Value& obj, const Range<NUM>& range, Validators... validators) {
        if (range.isExclusiveMinimum) {
            obj.AddMember("exclusiveMinimum", range.minimum, alloc);
        }
        else {
            obj.AddMember("minimum", range.minimum, alloc);
        }
        if (range.isExclusiveMaximum) {
            obj.AddMember("exclusiveMaximum", range.maximum, alloc);
        }
        else {
            obj.AddMember("maximum", range.maximum, alloc);
        }

        add_validation_fields<NUM>(alloc, obj, validators...);
    }

    // Fundamental types schemas
    template<typename T, typename... Validators>
    void generate_schema(
            const char* name, const char* title, const char* description,
            Document::AllocatorType& alloc, Value& schema,
            Validators... validators);

    static inline void generate_schema_base(
            const char* name, const char* title, const char* description, const char* type,
            Document::AllocatorType& alloc, Value& schema) {
        if (name) {
            Value jname;
            jname.SetString(name, alloc);
            schema.AddMember(jname, Value().SetObject(), alloc);
        }

        Value& jobj = name ? schema[name] : schema;

        if (title) {
            Value jtitle;
            jtitle.SetString(title, alloc);
            jobj.AddMember("title", jtitle, alloc);
        }

        if (description) {
            Value jdesc;
            jdesc.SetString(description, alloc);
            jobj.AddMember("description", jdesc, alloc);
        }

        if (type) {
            Value jtype;
            jtype.SetString(type, alloc);
            jobj.AddMember("type", jtype, alloc);
        }
    }

    template<
        typename NUM,
        std::enable_if_t<std::is_arithmetic_v<NUM>, bool> = true>
    inline void generate_schema(
            const char* name, const char* title, const char* description,
            Document::AllocatorType& alloc, Value& schema) {
        if constexpr (std::is_floating_point_v<NUM>) {
            generate_schema_base(name, title, description, "number", alloc, schema);
        }
        else if constexpr (std::is_integral_v<NUM>) {
            generate_schema_base(name, title, description, "integer", alloc, schema);
        }
        else {
            static_assert(false, "unsupported type");
        }
    }
}