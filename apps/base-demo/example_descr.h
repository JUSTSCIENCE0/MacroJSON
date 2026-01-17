// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

MJSON_OBJECT_BEGIN(SimpleExample)
    MJSON_FIELD(int,         i32_attr)
    MJSON_FIELD(double,      dbl_attr)
    MJSON_FIELD(uint64_t,    u64_attr)
    MJSON_FIELD(std::string, str_attr)
MJSON_OBJECT_END(SimpleExample)

MJSON_OBJECT_BEGIN(Level1Example)
    MJSON_FIELD(int,           i32_attr)
    MJSON_FIELD(double,        dbl_attr)
    MJSON_FIELD(SimpleExample, obj_attr)
MJSON_OBJECT_END(Level1Example)

MJSON_OBJECT_BEGIN(Level2Example)
    MJSON_FIELD(uint64_t,      u64_attr)
    MJSON_FIELD(SimpleExample, smp_attr)
    MJSON_FIELD(Level1Example, lvl_attr)
MJSON_OBJECT_END(Level2Example)
