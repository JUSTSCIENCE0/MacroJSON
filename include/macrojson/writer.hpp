// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <macrojson/undef_macros.h>

#define MJSON_OBJECT_BEGIN(obj_name) \
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
