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

#ifdef MJSON_OBJECT_END
#  undef MJSON_OBJECT_END
#endif

#define MJSON_OBJECT_BEGIN(obj_name) \
namespace macrojson { \
    static inline void write_to_json(const char*, const obj_name& jval, rapidjson::Document& d) {

#define MJSON_FIELD(type, field, ...) \
        write_to_json(#field, jval.field, d);

#define MJSON_OBJECT_END(obj_name) \
    } \
}
