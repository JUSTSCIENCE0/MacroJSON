// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <macrojson/undef_macros.h>

#define MJSON_OBJECT_BEGIN(obj_name) \
    struct obj_name {

#define MJSON_FIELD(type, field, ...) \
        type field{};

#define MJSON_OBJECT_END(obj_name) \
    };

