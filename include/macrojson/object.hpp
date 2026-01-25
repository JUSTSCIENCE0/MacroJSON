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

