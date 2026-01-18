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
    struct obj_name {

#define MJSON_FIELD(type, field, ...) \
        type field{};

#define MJSON_ARRAY(type, field, ...) \
        std::vector<type> field{};

#define MJSON_OBJECT_END(obj_name) \
    };

