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

MJSON_ENUM_BEGIN(SimpleEnum)
    MJSON_ENUM_UNIT (ENUM_VALUE_ONE,   ONE)
    MJSON_ENUM_VALUE(ENUM_VALUE_TWO,   TWO, 2)
    MJSON_ENUM_ALIAS(ENUM_VALUE_THREE, THREE, ENUM_VALUE_TWO)
MJSON_ENUM_END(SimpleEnum)

MJSON_OBJECT_BEGIN(EnumExample)
    MJSON_FIELD(SimpleEnum,              enum_attr)
    MJSON_FIELD(std::vector<SimpleEnum>, enum_arr)
MJSON_OBJECT_END(EnumExample)

/*
polymorphic object draft

#define MJSON_BASE_OBJECT_NAME ...
MJSON_POLYMORPHIC_OBJECT_BEGIN()
    MJSON_BASE_OBJECT_BEGIN(types_enum)
        MJSON_POLYMORPHIC_FIELD(...)
        ...
    MJSON_BASE_OBJECT_END()
    MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator)
        MJSON_POLYMORPHIC_FIELD(...)
        MJSON_POLYMORPHIC_FIELD(...)
        ...
    MJSON_DERIVED_OBJECT_END(obj_name)
    MJSON_DERIVED_OBJECT_BEGIN(obj_name, type_enumerator)
        MJSON_POLYMORPHIC_FIELD(...)
        MJSON_POLYMORPHIC_FIELD(...)
        ...
    MJSON_DERIVED_OBJECT_END(obj_name)
    ...
MJSON_POLYMORPHIC_OBJECT_END()
#undef MJSON_BASE_OBJECT_NAME

*/
