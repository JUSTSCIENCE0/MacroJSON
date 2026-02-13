// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

MJSON_OBJECT_BEGIN(
        SimpleExample,
        "Simple Example",
        "A simple example object with primitive types and string")
    MJSON_FIELD(int,         i32_attr,
        "I32 Attribute",
        "32-bit integer attribute",
        macrojson::Range<int>{})
    MJSON_FIELD(double,      dbl_attr,
        "Double Attribute",
        "Double-precision floating-point attribute")
    MJSON_FIELD(uint64_t,    u64_attr,
        "U64 Attribute",
        "64-bit unsigned integer attribute")
    MJSON_FIELD(std::string, str_attr,
        "String Attribute", nullptr,
        macrojson::StringLength{ 1, 100 })
MJSON_OBJECT_END(SimpleExample)

MJSON_OBJECT_BEGIN(Level1Example,
        "Level 1 Example",
        "An example object that contains another object as an attribute")
    MJSON_FIELD(int,           i32_attr,
        "I32 Attribute",
        "32-bit integer attribute")
    MJSON_FIELD(double,        dbl_attr,
        "Double Attribute",
        "Double-precision floating-point attribute")
    MJSON_FIELD(SimpleExample, obj_attr, nullptr, nullptr)
MJSON_OBJECT_END(Level1Example)

MJSON_OBJECT_BEGIN(Level2Example,
        "Level 2 Example",
        "An example object that contains two levels of nested objects as attributes")
    MJSON_FIELD(uint64_t,      u64_attr,
        "U64 Attribute",
        "64-bit unsigned integer attribute")
    MJSON_FIELD(SimpleExample, smp_attr,
        "Object Attribute",
        "An attribute that is an object of type SimpleExample")
    MJSON_FIELD(Level1Example, lvl_attr, nullptr, nullptr)
MJSON_OBJECT_END(Level2Example)

MJSON_OBJECT_BEGIN(OptionalExample,
        "Optional Example",
        "An example object that contains optional attributes of various types")
   MJSON_FIELD(std::optional<int>,           i32_opt,
        "Optional I32 Attribute",
        "An optional 32-bit integer attribute",
        macrojson::Range<int>{})
   MJSON_FIELD(std::optional<double>,        dbl_opt,
        "Optional Double Attribute",
        "An optional double-precision floating-point attribute")
   MJSON_FIELD(std::optional<uint64_t>,      u64_opt,
        "Optional U64 Attribute",
        "An optional 64-bit unsigned integer attribute")
   MJSON_FIELD(std::optional<std::string>,   str_opt,
        "Optional String Attribute",
        "An optional string attribute")
   MJSON_FIELD(std::optional<SimpleExample>, smp_opt,
        "Optional Object Attribute",
        "An optional object of type SimpleExample")
   MJSON_FIELD(std::optional<Level1Example>, lvl1_opt,
        "Optional Level1 Object Attribute",
        "An optional object of type Level1Example")
   MJSON_FIELD(std::optional<Level2Example>, lvl2_opt,
        "Optional Level2 Object Attribute",
        "An optional object of type Level2Example")
MJSON_OBJECT_END(OptionalExample)

MJSON_OBJECT_BEGIN(ArrayExample,
        "Array Example",
        "An example object that contains arrays of various types as attributes")
   MJSON_FIELD(std::vector<int>,           i32_arr,
        "I32 Array Attribute",
        "An array of 32-bit integer attributes",
        macrojson::Range<int>{})
   MJSON_FIELD(std::vector<double>,        dbl_arr,
        "Double Array Attribute",
        "An array of double-precision floating-point attributes",
        macrojson::ArrayParams{ 1, 100, true },
        macrojson::Range<double>{ -1000.0, true, 1000.0, true })
   MJSON_FIELD(std::vector<uint64_t>,      u64_arr,
        "U64 Array Attribute",
        "An array of 64-bit unsigned integer attributes")
   MJSON_FIELD(std::vector<std::string>,   str_arr,
        "String Array Attribute",
        "An array of string attributes")
   MJSON_FIELD(std::vector<SimpleExample>, smp_arr,
        "Object Array Attribute",
        "An array of objects of type SimpleExample")
   MJSON_FIELD(std::vector<Level1Example>, lvl1_arr,
        "Level1 Object Array Attribute",
        "An array of objects of type Level1Example")
   MJSON_FIELD(std::vector<Level2Example>, lvl2_arr,
        "Level2 Object Array Attribute",
        "An array of objects of type Level2Example")
MJSON_OBJECT_END(ArrayExample)

MJSON_ENUM_BEGIN(SimpleEnum,
        "Simple Enum",
        "A simple example enum with unit, value, and alias enumerators")
    MJSON_ENUM_UNIT (ENUM_VALUE_ONE,   ONE)
    MJSON_ENUM_VALUE(ENUM_VALUE_TWO,   TWO,   2)
    MJSON_ENUM_ALIAS(ENUM_VALUE_THREE, THREE, ENUM_VALUE_TWO)
MJSON_ENUM_END(SimpleEnum)

MJSON_OBJECT_BEGIN(EnumExample,
        "Enum Example",
        "An example object that contains an enum attribute")
    MJSON_FIELD(SimpleEnum,              enum_attr, nullptr, nullptr)
    MJSON_FIELD(std::vector<SimpleEnum>, enum_arr, 
        "Enum Array Attribute",
        "An array of enum attributes")
MJSON_OBJECT_END(EnumExample)

MJSON_ENUM_BEGIN(EnumTypesExample,
        "Enum Types Example",
        "An example enum that is used as a type discriminator for polymorphic objects")
    MJSON_ENUM_UNIT(E_TYPE_OBJECT_1, object1)
    MJSON_ENUM_UNIT(E_TYPE_OBJECT_2, object2)
MJSON_ENUM_END(EnumTypesExample)

#define MJSON_BASE_OBJECT_NAME BaseExample
MJSON_POLYMORPHIC_OBJECT_BEGIN(
        "Polymorphic object example",
        "An example of a polymorphic object with a type discriminator enum "
        "and two derived object types")
    MJSON_BASE_OBJECT_BEGIN(EnumTypesExample)
      MJSON_BASE_OBJECT_FIELD(int,   i32_attr,
        "I32 Attribute",
        "32-bit integer attribute")
      MJSON_BASE_OBJECT_FIELD(float, flt_attr,
        "Float Attribute",
        "Single-precision floating-point attribute")
    MJSON_BASE_OBJECT_END()
    MJSON_DERIVED_OBJECT_BEGIN(Object1Example, E_TYPE_OBJECT_1)
      MJSON_DERIVED_OBJECT_FIELD(uint64_t, u64_attr,
        "U64 Attribute",
        "64-bit unsigned integer attribute")
      MJSON_DERIVED_OBJECT_FIELD(double,   dbl_attr,
        "Double Attribute",
        "Double-precision floating-point attribute")
    MJSON_DERIVED_OBJECT_END(Object1Example)
    MJSON_DERIVED_OBJECT_BEGIN(Object2Example, E_TYPE_OBJECT_2)
      MJSON_DERIVED_OBJECT_FIELD(std::string, str_attr,
        "String Attribute",
        "A string attribute")
    MJSON_DERIVED_OBJECT_END(Object2Example)
MJSON_POLYMORPHIC_OBJECT_END()
#undef MJSON_BASE_OBJECT_NAME

MJSON_OBJECT_BEGIN(PolymorphicExample,
        "Polymorphic Example",
        "An example object that contains polymorphic objects as attributes")
   MJSON_FIELD(std::unique_ptr<BaseExample>, plm_attr,
        "Polymorphic Attribute",
        "A polymorphic object attribute")
   MJSON_FIELD(std::vector<
       std::unique_ptr<BaseExample>>, plm_arr,
        "Polymorphic Array Attribute",
        "An array of polymorphic object attributes")
MJSON_OBJECT_END(PolymorphicExample)
