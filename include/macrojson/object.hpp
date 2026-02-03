// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <macrojson/undef_macros.h>

// struct generation macros
#define MJSON_OBJECT_BEGIN(obj_name) \
    struct obj_name {

#define MJSON_FIELD(type, field, ...) \
        type field{};

#define MJSON_OBJECT_END(obj_name) \
    };

// enum generation macros
#define MJSON_ENUM_BEGIN(enum_name) \
    enum enum_name {

#define MJSON_ENUM_UNIT(name, short_name) \
        name,

#define MJSON_ENUM_VALUE(name, short_name, value) \
        name = value,

#define MJSON_ENUM_ALIAS(name, short_name, value) \
        name = value,

#define MJSON_ENUM_END(enum_name) \
    };

// polymorphic object macros
#define MJSON_POLYMORPHIC_OBJECT_BEGIN()

#define MJSON_BASE_OBJECT_BEGIN(types_enum) \
    struct MJSON_BASE_OBJECT_NAME { \
        types_enum type{};

#define MJSON_BASE_OBJECT_FIELD(type, field, ...) \
        type field{};

#define MJSON_BASE_OBJECT_END() \
    };

#define MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator) \
    struct obj_name : public MJSON_BASE_OBJECT_NAME { \
        obj_name() { type = type_enumerator; }

#define MJSON_DERIVED_OBJECT_FIELD(type, field, ...) \
        type field{};

#define MJSON_DERIVED_OBJECT_END(obj_name) \
    };

#define MJSON_POLYMORPHIC_OBJECT_END()

