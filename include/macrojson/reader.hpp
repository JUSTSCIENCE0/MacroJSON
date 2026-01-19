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
    static inline MJsonErrorCode read_from_json( \
            const char* name, const rapidjson::Value& root, obj_name& val) { \
        if (name && !root.HasMember(name)) { \
            return MJsonErrorCode::E_MJSON_NOT_EXISTS; \
        } \
        const rapidjson::Value& obj = name ? root[name] : \
                                  root; \
        if (!obj.IsObject()) { \
            return MJsonErrorCode::E_MJSON_TYPE_MISMATCH; \
        } \

#define MJSON_FIELD(type, field, ...) \
        MJSON_CHECK_ERROR(read_from_json(#field, obj, val.field));

#define MJSON_OBJECT_END(obj_name) \
        return MJsonErrorCode::E_MJSON_OK; \
    } \
}
