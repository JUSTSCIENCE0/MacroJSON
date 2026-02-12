// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <macrojson/undef_macros.h>

// struct writer macros
#define MJSON_OBJECT_BEGIN(obj_name, ...) \
namespace macrojson { \
    static inline void write_to_json( \
            const char* name, const obj_name& jval, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& root) { \
        rapidjson::Value jobj(rapidjson::kObjectType);

#define MJSON_FIELD(type, field, ...) \
        write_to_json(#field, jval.field, alloc, jobj);

#define MJSON_OBJECT_END(obj_name) \
        if (name) { \
            write_to_json(name, std::move(jobj), alloc, root); \
        } else { \
            root = std::move(jobj); \
        } \
    } \
}

// enum writer macros
#define MJSON_ENUM_BEGIN(enum_name, ...) \
namespace macrojson { \
    static inline void write_to_json( \
            const char* name, enum_name jval, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& root) { \
        const char* enum_str = nullptr; \
        switch (jval) {

#define MJSON_ENUM_UNIT(name, short_name) \
            case name: enum_str = #short_name; break;

#define MJSON_ENUM_VALUE(name, short_name, value) \
            case name: enum_str = #short_name; break;

#define MJSON_ENUM_ALIAS(name, short_name, value)

#define MJSON_ENUM_END(enum_name) \
            default: \
                assert(!"unknown enum value"); \
                enum_str = "unknown"; break; \
        } \
        assert(enum_str); \
        write_to_json(name, enum_str, alloc, root); \
    } \
}

// polymorphic object macros
#define MJSON_POLYMORPHIC_OBJECT_BEGIN(...) \
namespace macrojson { \
    static inline void write_to_json( \
            const char* name, const MJSON_BASE_OBJECT_NAME* val_ptr, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& root) { \
        assert(val_ptr); \
        rapidjson::Value jobj(rapidjson::kObjectType);

#define MJSON_BASE_OBJECT_BEGIN(types_enum) \
        write_to_json("type", val_ptr->type, alloc, jobj);

#define MJSON_BASE_OBJECT_FIELD(type, field, ...) \
        write_to_json(#field, val_ptr->field, alloc, jobj);

#define MJSON_BASE_OBJECT_END() \
        switch (val_ptr->type) {

#define MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator) \
        case type_enumerator: { \
            auto obj_ptr = static_cast<const obj_name*>(val_ptr);

#define MJSON_DERIVED_OBJECT_FIELD(type, field, ...) \
            write_to_json(#field, obj_ptr->field, alloc, jobj);

#define MJSON_DERIVED_OBJECT_END(obj_name) \
            break; \
        }

#define MJSON_POLYMORPHIC_OBJECT_END() \
        default: \
            assert(!"undefined behavior"); \
            break; \
        } \
        if (name) { \
            write_to_json(name, std::move(jobj), alloc, root); \
        } else { \
            root = std::move(jobj); \
        } \
    } \
}
