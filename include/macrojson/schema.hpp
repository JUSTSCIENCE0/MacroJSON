// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <macrojson/undef_macros.h>

// struct generation macros
#define MJSON_OBJECT_BEGIN(obj_name, def_title, def_descr) \
namespace macrojson { \
    template<> \
    inline void generate_schema<obj_name>( \
            const char* name, const char* title, const char* description, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& schema) { \
        if (!title) \
            title = def_title; \
        if (!description) \
            description = def_descr; \
        generate_schema_base(name, title, description, "object", alloc, schema); \
        rapidjson::Value& jobj = name ? schema[name] : schema; \
        jobj.AddMember("properties", rapidjson::Value(rapidjson::kObjectType), alloc); \
        rapidjson::Value& jprops = jobj["properties"]; \
        jobj.AddMember("required", rapidjson::Value(rapidjson::kArrayType), alloc); \
        rapidjson::Value& jreq = jobj["required"];

#define MJSON_FIELD(type, field, title, description, /* validation */...) \
        if constexpr (!is_std_optional_v<type> && !is_std_vector_v<type>) \
            jreq.PushBack(#field, alloc); \
        generate_schema<type>(#field, title, description, alloc, jprops, ##__VA_ARGS__); \

#define MJSON_OBJECT_END(obj_name) \
    } \
}

// enum schema macros
#define MJSON_ENUM_BEGIN(enum_name, def_title, def_descr) \
namespace macrojson { \
    template<> \
    inline void generate_schema<enum_name>( \
            const char* name, const char* title, const char* description, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& schema) { \
        if (!title) \
            title = def_title; \
        if (!description) \
            description = def_descr; \
        generate_schema_base(name, title, description, "string", alloc, schema); \
        rapidjson::Value& jobj = name ? schema[name] : schema; \
        jobj.AddMember("enum", rapidjson::Value(rapidjson::kArrayType), alloc); \
        rapidjson::Value& jenums = jobj["enum"]; \

#define MJSON_ENUM_UNIT(name, short_name) \
        jenums.PushBack(#short_name, alloc);

#define MJSON_ENUM_VALUE(name, short_name, value) \
        jenums.PushBack(#short_name, alloc);

#define MJSON_ENUM_ALIAS(name, short_name, value) \
        jenums.PushBack(#short_name, alloc);

#define MJSON_ENUM_END(enum_name) \
    } \
}

// polymorphic object schema macros
#define MJSON_POLYMORPHIC_OBJECT_BEGIN(def_title, def_descr) \
namespace macrojson { \
    template<> \
    inline void generate_schema<MJSON_BASE_OBJECT_NAME>( \
            const char* name, const char* title, const char* description, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& schema) { \
        if (!title) \
            title = def_title; \
        if (!description) \
            description = def_descr; \
        generate_schema_base(name, title, description, "object", alloc, schema); \
        rapidjson::Value& jobj = name ? schema[name] : schema; \

#define MJSON_BASE_OBJECT_BEGIN(types_enum) \
        jobj.AddMember("properties", rapidjson::Value(rapidjson::kObjectType), alloc); \
        rapidjson::Value& jprops = jobj["properties"]; \
        jobj.AddMember("required", rapidjson::Value(rapidjson::kArrayType), alloc); \
        rapidjson::Value& jreq = jobj["required"];

#define MJSON_BASE_OBJECT_FIELD(type, field, title, description, /* validation */...) \
        if constexpr (!is_std_optional_v<type> && !is_std_vector_v<type>) \
            jreq.PushBack(#field, alloc); \
        generate_schema<type>(#field, title, description, alloc, jprops, ##__VA_ARGS__); \

#define MJSON_BASE_OBJECT_END() \
        jobj.AddMember("oneOf", rapidjson::Value(rapidjson::kArrayType), alloc); \
        rapidjson::Value& joneof = jobj["oneOf"]; \

#define MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator) \
        { \
            auto jderived = rapidjson::Value(rapidjson::kObjectType); \
            jderived.AddMember("properties", rapidjson::Value(rapidjson::kObjectType), alloc); \
            rapidjson::Value& j_der_props = jderived["properties"]; \
            jderived.AddMember("required", rapidjson::Value(rapidjson::kArrayType), alloc); \
            rapidjson::Value& j_der_req = jderived["required"]; \
            j_der_props.AddMember("type", rapidjson::Value(rapidjson::kObjectType), alloc); \
            write_to_json("const", type_enumerator, alloc, j_der_props["type"]); \
            j_der_req.PushBack("type", alloc);

#define MJSON_DERIVED_OBJECT_FIELD(type, field, title, description, /* validation */...) \
            if constexpr (!is_std_optional_v<type> && !is_std_vector_v<type>) \
                j_der_req.PushBack(#field, alloc); \
            generate_schema<type>(#field, title, description, alloc, j_der_props, ##__VA_ARGS__); \

#define MJSON_DERIVED_OBJECT_END(obj_name) \
            joneof.PushBack(jderived, alloc); \
        }

#define MJSON_POLYMORPHIC_OBJECT_END() \
    } \
}


