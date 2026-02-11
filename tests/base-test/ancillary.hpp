// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#pragma once

#include <gtest/gtest.h>

#include "test_samples.hpp"

static void check_object(const SimpleExample& example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsObject());
    ASSERT_TRUE(obj.HasMember("i32_attr"));
    ASSERT_TRUE(obj["i32_attr"].IsInt());
    EXPECT_EQ(example.i32_attr, obj["i32_attr"].GetInt());
    ASSERT_TRUE(obj.HasMember("dbl_attr"));
    ASSERT_TRUE(obj["dbl_attr"].IsDouble());
    EXPECT_DOUBLE_EQ(example.dbl_attr, obj["dbl_attr"].GetDouble());
    ASSERT_TRUE(obj.HasMember("u64_attr"));
    ASSERT_TRUE(obj["u64_attr"].IsUint64());
    EXPECT_EQ(example.u64_attr, obj["u64_attr"].GetUint64());
    ASSERT_TRUE(obj.HasMember("str_attr"));
    ASSERT_TRUE(obj["str_attr"].IsString());
    EXPECT_EQ(example.str_attr, obj["str_attr"].GetString());
}

static void check_object(const Level1Example& example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsObject());
    ASSERT_TRUE(obj.HasMember("i32_attr"));
    ASSERT_TRUE(obj["i32_attr"].IsInt());
    EXPECT_EQ(example.i32_attr, obj["i32_attr"].GetInt());
    ASSERT_TRUE(obj.HasMember("dbl_attr"));
    ASSERT_TRUE(obj["dbl_attr"].IsDouble());
    EXPECT_DOUBLE_EQ(example.dbl_attr, obj["dbl_attr"].GetDouble());
    ASSERT_TRUE(obj.HasMember("obj_attr"));
    check_object(example.obj_attr, obj["obj_attr"]);
}

static void check_object(const Level2Example& example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsObject());
    ASSERT_TRUE(obj.HasMember("u64_attr"));
    ASSERT_TRUE(obj["u64_attr"].IsUint64());
    EXPECT_EQ(example.u64_attr, obj["u64_attr"].GetUint64());
    ASSERT_TRUE(obj.HasMember("smp_attr"));
    check_object(example.smp_attr, obj["smp_attr"]);
    ASSERT_TRUE(obj.HasMember("lvl_attr"));
    check_object(example.lvl_attr, obj["lvl_attr"]);
}

static void check_object(const ArrayExample& example, const rapidjson::Value& obj) {
   ASSERT_TRUE(obj.IsObject());

#define CHECK_ARRAY(field, type_check, get_method) \
   ASSERT_TRUE(obj.HasMember(#field)); \
   const auto& field = obj[#field]; \
   ASSERT_TRUE(field.IsArray()); \
   ASSERT_EQ(example.field.size(), field.Size()); \
   for (rapidjson::SizeType i = 0; i < field.Size(); ++i) { \
       ASSERT_TRUE(field[i].type_check()); \
       EXPECT_EQ(example.field[i], field[i].get_method()); \
   }

   CHECK_ARRAY(i32_arr, IsInt, GetInt)
   CHECK_ARRAY(dbl_arr, IsDouble, GetDouble)
   CHECK_ARRAY(u64_arr, IsUint64, GetUint64)
   CHECK_ARRAY(str_arr, IsString, GetString)

#undef CHECK_ARRAY

#define CHECK_ARRAY(field) \
   ASSERT_TRUE(obj.HasMember(#field)); \
   const auto& field = obj[#field]; \
   ASSERT_TRUE(field.IsArray()); \
   ASSERT_EQ(example.field.size(), field.Size()); \
   for (rapidjson::SizeType i = 0; i < field.Size(); ++i) { \
       check_object(example.field[i], field[i]); \
   }

   CHECK_ARRAY(smp_arr)
   CHECK_ARRAY(lvl1_arr)
   CHECK_ARRAY(lvl2_arr)

#undef CHECK_ARRAY
}

static void check_object(const OptionalExample& example, const rapidjson::Value& obj) {
   ASSERT_TRUE(obj.IsObject());

#define CHECK_OPTIONAL(field, type_check, get_method) \
   if (example.field.has_value()) { \
       ASSERT_TRUE(obj.HasMember(#field)); \
       const auto& field = obj[#field]; \
       ASSERT_TRUE(field.type_check()); \
       EXPECT_EQ(example.field.value(), field.get_method()); \
   } else { \
       ASSERT_FALSE(obj.HasMember(#field)); \
   }

   CHECK_OPTIONAL(i32_opt, IsInt, GetInt)
   CHECK_OPTIONAL(dbl_opt, IsDouble, GetDouble)
   CHECK_OPTIONAL(u64_opt, IsUint64, GetUint64)
   CHECK_OPTIONAL(str_opt, IsString, GetString)
#undef CHECK_OPTIONAL

#define CHECK_OPTIONAL(field) \
   if (example.field.has_value()) { \
       ASSERT_TRUE(obj.HasMember(#field)); \
       const auto& field = obj[#field]; \
       ASSERT_TRUE(field.IsObject()); \
       check_object(example.field.value(), field); \
   } else { \
       ASSERT_TRUE(!obj.HasMember(#field) || obj[#field].IsNull()); \
   }
   CHECK_OPTIONAL(smp_opt)
   CHECK_OPTIONAL(lvl1_opt)
   CHECK_OPTIONAL(lvl2_opt)

#undef CHECK_OPTIONAL
}

static void check_object(SimpleEnum example, const rapidjson::Value& val) {
   ASSERT_TRUE(val.IsString());

   auto check_aliased_value = [](const char* str) {
       if (!std::strcmp(str, "TWO") ||
           !std::strcmp(str, "THREE"))
           return true;
       return false;
   };

   switch (example)
   {
   case ENUM_VALUE_ONE:
       ASSERT_STREQ("ONE", val.GetString());
       break;
   case ENUM_VALUE_TWO: {
       ASSERT_PRED1(check_aliased_value, val.GetString());
       break;
   }
   default:
       FAIL() << "Unknown enum value";
       break;
   }
}

static void check_object(const EnumExample& example, const rapidjson::Value& obj) {
   ASSERT_TRUE(obj.IsObject());
   ASSERT_TRUE(obj.HasMember("enum_attr"));
   check_object(example.enum_attr, obj["enum_attr"]);
   ASSERT_TRUE(obj.HasMember("enum_arr"));
   const auto& enum_arr = obj["enum_arr"];
   ASSERT_TRUE(enum_arr.IsArray());
   ASSERT_EQ(example.enum_arr.size(), enum_arr.Size());
   for (rapidjson::SizeType i = 0; i < enum_arr.Size(); ++i) {
       check_object(example.enum_arr[i], enum_arr[i]);
   }
}

static void check_object(EnumTypesExample example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsString());

    switch (example) {
    case E_TYPE_OBJECT_1:
        ASSERT_STREQ("object1", obj.GetString());
        break;
    case E_TYPE_OBJECT_2:
        ASSERT_STREQ("object2", obj.GetString());
        break;
    default:
        FAIL() << "Unknown enum value";
        break;
    }
}

static void check_object(const BaseExample& example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsObject());
    ASSERT_TRUE(obj.HasMember("type"));
    check_object(example.type, obj["type"]);
    ASSERT_TRUE(obj.HasMember("i32_attr"));
    ASSERT_TRUE(obj["i32_attr"].IsInt());
    EXPECT_EQ(example.i32_attr, obj["i32_attr"].GetInt());
    ASSERT_TRUE(obj.HasMember("flt_attr"));
    ASSERT_TRUE(obj["flt_attr"].IsFloat());
    EXPECT_FLOAT_EQ(example.flt_attr, obj["flt_attr"].GetFloat());
}

static void check_object(const Object1Example& example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsObject());
    check_object(static_cast<const BaseExample&>(example), obj);
    ASSERT_EQ(example.type, E_TYPE_OBJECT_1);
    ASSERT_TRUE(obj.HasMember("u64_attr"));
    ASSERT_TRUE(obj["u64_attr"].IsUint64());
    EXPECT_EQ(example.u64_attr, obj["u64_attr"].GetUint64());
    ASSERT_TRUE(obj.HasMember("dbl_attr"));
    ASSERT_TRUE(obj["dbl_attr"].IsDouble());
    EXPECT_DOUBLE_EQ(example.dbl_attr, obj["dbl_attr"].GetDouble());
}

static void check_object(const Object2Example& example, const rapidjson::Value& obj) {
    ASSERT_TRUE(obj.IsObject());
    check_object(static_cast<const BaseExample&>(example), obj);
    ASSERT_EQ(example.type, E_TYPE_OBJECT_2);
    ASSERT_TRUE(obj.HasMember("str_attr"));
    ASSERT_TRUE(obj["str_attr"].IsString());
    EXPECT_EQ(example.str_attr, obj["str_attr"].GetString());
}
