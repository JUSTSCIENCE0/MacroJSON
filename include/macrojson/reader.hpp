// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <macrojson/undef_macros.h>

// struct reader macros
#define MJSON_OBJECT_BEGIN(obj_name, ...) \
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

// enum reader macros
#define MJSON_ENUM_BEGIN(enum_name, ...) \
namespace macrojson { \
    static inline MJsonErrorCode read_from_json( \
            const char* name, const rapidjson::Value& root, enum_name& val) { \
        if (name && !root.HasMember(name)) { \
            return MJsonErrorCode::E_MJSON_NOT_EXISTS; \
        } \
        const rapidjson::Value& jval = name ? root[name] : root; \
        if (!jval.IsString()) { \
            return MJsonErrorCode::E_MJSON_TYPE_MISMATCH; \
        } \
        const char* enum_str = jval.GetString();

#define MJSON_ENUM_UNIT(name, short_name) \
        if (std::strcmp(enum_str, #short_name) == 0) { \
            val = name; \
            return MJsonErrorCode::E_MJSON_OK; \
        }

#define MJSON_ENUM_VALUE(name, short_name, value) \
            MJSON_ENUM_UNIT(name, short_name)

#define MJSON_ENUM_ALIAS(name, short_name, value) \
            MJSON_ENUM_UNIT(name, short_name)

#define MJSON_ENUM_END(enum_name) \
        assert(!"unknown enum value"); \
        return MJsonErrorCode::E_MJSON_UNKNOWN_ENUM; \
    } \
}

// polymorphic object macros
#define MJSON_POLYMORPHIC_OBJECT_BEGIN(...) \
namespace macrojson { \
    static inline MJsonErrorCode read_from_json( \
            const char* name, const rapidjson::Value& root, std::unique_ptr<MJSON_BASE_OBJECT_NAME>& val) { \
        val.reset(); \
        if (name && !root.HasMember(name)) { \
            return MJsonErrorCode::E_MJSON_NOT_EXISTS; \
        } \
        const rapidjson::Value& obj = name ? root[name] : root; \
        if (!obj.IsObject()) { \
            return MJsonErrorCode::E_MJSON_TYPE_MISMATCH; \
        } \

#define MJSON_BASE_OBJECT_BEGIN(types_enum) \
        MJSON_BASE_OBJECT_NAME base_obj{}; \
        MJSON_CHECK_ERROR(read_from_json("type", obj, base_obj.type));

#define MJSON_BASE_OBJECT_FIELD(type, field, ...) \
        MJSON_CHECK_ERROR(read_from_json(#field, obj, base_obj.field));

#define MJSON_BASE_OBJECT_END() \
        switch (base_obj.type) {

#define MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator) \
        case type_enumerator: { \
            val = std::make_unique<obj_name>(); \
            auto obj_ptr = static_cast<obj_name*>(val.get());

#define MJSON_DERIVED_OBJECT_FIELD(type, field, ...) \
            MJSON_CHECK_ERROR(read_from_json(#field, obj, obj_ptr->field));

#define MJSON_DERIVED_OBJECT_END(obj_name) \
            break; \
        }

#define MJSON_POLYMORPHIC_OBJECT_END() \
        default: \
            return MJsonErrorCode::E_MJSON_UNSUPPORTED_OBJECT_TYPE;\
        } \
        *val = base_obj; \
        return MJsonErrorCode::E_MJSON_OK; \
    } \
}
