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
        generate_schema_base(name, title, description, "object", alloc, schema); \
        rapidjson::Value& jobj = name ? schema[name] : schema; \
        rapidjson::Value jprops(rapidjson::kObjectType);

#define MJSON_FIELD(type, field, title, description, /* validation */...)

#define MJSON_OBJECT_END(obj_name) \
        jobj.AddMember("properties", std::move(jprops), alloc); \
    } \
}

// enum schema macros
#define MJSON_ENUM_BEGIN(enum_name)

#define MJSON_ENUM_UNIT(name, short_name)

#define MJSON_ENUM_VALUE(name, short_name, value)

#define MJSON_ENUM_ALIAS(name, short_name, value)

#define MJSON_ENUM_END(enum_name)

// polymorphic object schema macros
#define MJSON_POLYMORPHIC_OBJECT_BEGIN()

#define MJSON_BASE_OBJECT_BEGIN(types_enum)

#define MJSON_BASE_OBJECT_FIELD(type, field, ...)

#define MJSON_BASE_OBJECT_END()

#define MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator)

#define MJSON_DERIVED_OBJECT_FIELD(type, field, ...)

#define MJSON_DERIVED_OBJECT_END(obj_name)

#define MJSON_POLYMORPHIC_OBJECT_END()


