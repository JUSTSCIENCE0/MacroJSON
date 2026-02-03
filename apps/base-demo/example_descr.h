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

MJSON_ENUM_BEGIN(SimpleEnum)
    MJSON_ENUM_UNIT (ENUM_VALUE_ONE,   ONE)
    MJSON_ENUM_VALUE(ENUM_VALUE_TWO,   TWO,   2)
    MJSON_ENUM_ALIAS(ENUM_VALUE_THREE, THREE, ENUM_VALUE_TWO)
MJSON_ENUM_END(SimpleEnum)

MJSON_OBJECT_BEGIN(EnumExample)
    MJSON_FIELD(SimpleEnum,              enum_attr)
    MJSON_FIELD(std::vector<SimpleEnum>, enum_arr)
MJSON_OBJECT_END(EnumExample)

MJSON_ENUM_BEGIN(EnumTypesExample)
    MJSON_ENUM_UNIT(E_TYPE_OBJECT_1, object1)
    MJSON_ENUM_UNIT(E_TYPE_OBJECT_2, object2)
MJSON_ENUM_END(EnumTypesExample)

#define MJSON_BASE_OBJECT_NAME BaseExample
MJSON_POLYMORPHIC_OBJECT_BEGIN()
    MJSON_BASE_OBJECT_BEGIN(EnumTypesExample)
        MJSON_BASE_OBJECT_FIELD(int,   i32_attr)
        MJSON_BASE_OBJECT_FIELD(float, flt_attr)
    MJSON_BASE_OBJECT_END()
    MJSON_DERIVED_OBJECT_BEGIN(Object1Example, E_TYPE_OBJECT_1)
        MJSON_DERIVED_OBJECT_FIELD(uint64_t, u64_attr)
        MJSON_DERIVED_OBJECT_FIELD(double,   dbl_attr)
    MJSON_DERIVED_OBJECT_END(Object1Example)
    MJSON_DERIVED_OBJECT_BEGIN(Object2Example, E_TYPE_OBJECT_2)
        MJSON_DERIVED_OBJECT_FIELD(std::string, str_attr)
    MJSON_DERIVED_OBJECT_END(Object2Example)
MJSON_POLYMORPHIC_OBJECT_END()
#undef MJSON_BASE_OBJECT_NAME
