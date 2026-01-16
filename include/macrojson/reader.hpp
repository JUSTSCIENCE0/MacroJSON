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
    static inline macrojson::MJsonErrorCode read_from_json(const rapidjson::Value& root, const char* name, obj_name& val) { \
        if (name && !root.HasMember(name)) { \
            return macrojson::MJsonErrorCode::E_MJSON_NOT_EXISTS; \
        } \
        const rapidjson::Value& obj = name ? root[name] : \
                                  root; \
        if (!obj.IsObject()) { \
            return macrojson::MJsonErrorCode::E_MJSON_TYPE_MISMATCH; \
        } \

#define MJSON_FIELD(type, field, ...) \
        MJSON_CHECK_ERROR(macrojson::read_from_json(obj, #field, val.field));

#define MJSON_OBJECT_END(obj_name) \
        return macrojson::MJsonErrorCode::E_MJSON_OK; \
    }
