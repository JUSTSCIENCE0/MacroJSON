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

MJSON_OBJECT_BEGIN(ArrayExample)
    MJSON_FIELD(std::vector<int>,           i32_arr)
    MJSON_FIELD(std::vector<double>,        dbl_arr)
    MJSON_FIELD(std::vector<uint64_t>,      u64_arr)
    MJSON_FIELD(std::vector<std::string>,   str_arr)
    MJSON_FIELD(std::vector<SimpleExample>, smp_arr)
    MJSON_FIELD(std::vector<Level1Example>, lvl1_arr)
    MJSON_FIELD(std::vector<Level2Example>, lvl2_arr)
MJSON_OBJECT_END(ArrayExample)

MJSON_OBJECT_BEGIN(OptionalExample)
    MJSON_FIELD(std::optional<int>,           i32_opt)
    MJSON_FIELD(std::optional<double>,        dbl_opt)
    MJSON_FIELD(std::optional<uint64_t>,      u64_opt)
    MJSON_FIELD(std::optional<std::string>,   str_opt)
    MJSON_FIELD(std::optional<SimpleExample>, smp_opt)
    MJSON_FIELD(std::optional<Level1Example>, lvl1_opt)
    MJSON_FIELD(std::optional<Level2Example>, lvl2_opt)
MJSON_OBJECT_END(OptionalExample)
