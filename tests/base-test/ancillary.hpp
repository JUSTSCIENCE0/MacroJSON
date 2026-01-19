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
