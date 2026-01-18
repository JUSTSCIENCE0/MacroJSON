// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#ifdef MJSON_OBJECT_BEGIN
#  undef MJSON_OBJECT_BEGIN
#endif

#ifdef MJSON_FIELD
#  undef MJSON_FIELD
#endif

#ifdef MJSON_ARRAY
#  undef MJSON_ARRAY
#endif

#ifdef MJSON_OBJECT_END
#  undef MJSON_OBJECT_END
#endif

#define MJSON_OBJECT_BEGIN(obj_name) \
namespace macrojson { \
    static inline void write_to_json( \
            const char* name, const obj_name& jval, \
            rapidjson::Document::AllocatorType& alloc, rapidjson::Value& root) { \
        rapidjson::Value jobj(rapidjson::kObjectType);

#define MJSON_FIELD(type, field, ...) \
        write_to_json(#field, jval.field, alloc, jobj);

#define MJSON_ARRAY(type, field, ...) \
        write_to_json(#field, jval.field, alloc, jobj);

#define MJSON_OBJECT_END(obj_name) \
        if (name) { \
            write_to_json(name, std::move(jobj), alloc, root); \
        } else { \
            root = std::move(jobj); \
        } \
    } \
}
