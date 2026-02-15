// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include <gtest/gtest.h>

#include "test_samples.hpp"

void check_str_param(const rapidjson::Value& schema, const char* param_name, const char* expected_value) {
    ASSERT_TRUE(schema.HasMember(param_name));
    ASSERT_TRUE(schema[param_name].IsString());
    ASSERT_STREQ(expected_value, schema[param_name].GetString());
}

void check_int_param(const rapidjson::Value& schema, const char* param_name, int expected_value) {
    ASSERT_TRUE(schema.HasMember(param_name));
    ASSERT_TRUE(schema[param_name].IsInt());
    ASSERT_EQ(expected_value, schema[param_name].GetInt());
}

void check_uint64_param(const rapidjson::Value& schema, const char* param_name, uint64_t expected_value) {
    ASSERT_TRUE(schema.HasMember(param_name));
    ASSERT_TRUE(schema[param_name].IsUint64());
    ASSERT_EQ(expected_value, schema[param_name].GetUint64());
}

void check_double_param(const rapidjson::Value& schema, const char* param_name, double expected_value) {
    ASSERT_TRUE(schema.HasMember(param_name));
    ASSERT_TRUE(schema[param_name].IsDouble());
    ASSERT_DOUBLE_EQ(expected_value, schema[param_name].GetDouble());
}

void check_bool_param(const rapidjson::Value& schema, const char* param_name, bool expected_value) {
    ASSERT_TRUE(schema.HasMember(param_name));
    ASSERT_TRUE(schema[param_name].IsBool());
    ASSERT_EQ(expected_value, schema[param_name].GetBool());
}

void check_title(const rapidjson::Value& schema, const char* expected_title) {
    check_str_param(schema, "title", expected_title);
}

void check_description(const rapidjson::Value& schema, const char* expected_description) {
    check_str_param(schema, "description", expected_description);
}

void check_type(const rapidjson::Value& schema, const char* expected_type) {
    check_str_param(schema, "type", expected_type);
}

const rapidjson::Value& get_properties(const rapidjson::Value& schema) {
    EXPECT_TRUE(schema.HasMember("properties"));
    EXPECT_TRUE(schema["properties"].IsObject());
    return schema["properties"];
}

const rapidjson::Value& get_field(const rapidjson::Value& properties, const char* field_name) {
    EXPECT_TRUE(properties.HasMember(field_name));
    EXPECT_TRUE(properties[field_name].IsObject());
    return properties[field_name];
}

const rapidjson::Value& get_alternatives(const rapidjson::Value& schema) {
    EXPECT_TRUE(schema.HasMember("oneOf"));
    EXPECT_TRUE(schema["oneOf"].IsArray());
    return schema["oneOf"];
}

void check_required_fields(const rapidjson::Value& schema, const std::vector<std::string>& expected_fields) {
    ASSERT_TRUE(schema.HasMember("required"));
    ASSERT_TRUE(schema["required"].IsArray());
    const auto& required = schema["required"];
    ASSERT_EQ(required.Size(), expected_fields.size());
    for (rapidjson::SizeType i = 0; i < required.Size(); ++i) {
        ASSERT_TRUE(required[i].IsString());
        ASSERT_STREQ(expected_fields[i].c_str(), required[i].GetString());
    }
}

void check_enum_field(const rapidjson::Value& field_schema, const std::vector<std::string>& expected_enum_values) {
    ASSERT_TRUE(field_schema.HasMember("enum"));
    ASSERT_TRUE(field_schema["enum"].IsArray());
    const auto& enum_values = field_schema["enum"];
    ASSERT_EQ(enum_values.Size(), expected_enum_values.size());
    for (rapidjson::SizeType i = 0; i < enum_values.Size(); ++i) {
        ASSERT_TRUE(enum_values[i].IsString());
        ASSERT_STREQ(expected_enum_values[i].c_str(), enum_values[i].GetString());
    }
}

void check_simple_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Simple Example");
    check_description(schema, "A simple example object with primitive types and string");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& i32_attr = get_field(properties, "i32_attr");
    check_title(i32_attr, "I32 Attribute");
    check_description(i32_attr, "32-bit integer attribute");
    check_type(i32_attr, "integer");
    check_int_param(i32_attr, "minimum", std::numeric_limits<int>::min());
    check_int_param(i32_attr, "maximum", std::numeric_limits<int>::max());

    const auto& dbl_attr = get_field(properties, "dbl_attr");
    check_title(dbl_attr, "Double Attribute");
    check_description(dbl_attr, "Double-precision floating-point attribute");
    check_type(dbl_attr, "number");

    const auto& u64_attr = get_field(properties, "u64_attr");
    check_title(u64_attr, "U64 Attribute");
    check_description(u64_attr, "64-bit unsigned integer attribute");
    check_type(u64_attr, "integer");

    const auto& str_attr = get_field(properties, "str_attr");
    check_title(str_attr, "String Attribute");
    check_type(str_attr, "string");
    check_int_param(str_attr, "minLength", 1);
    check_int_param(str_attr, "maxLength", 100);

    check_required_fields(schema, {"i32_attr", "dbl_attr", "u64_attr", "str_attr"});
}

void check_level1_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Level 1 Example");
    check_description(schema, "An example object that contains another object as an attribute");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& i32_attr = get_field(properties, "i32_attr");
    check_title(i32_attr, "I32 Attribute");
    check_description(i32_attr, "32-bit integer attribute");
    check_type(i32_attr, "integer");
    check_int_param(i32_attr, "exclusiveMinimum", -1000);
    check_int_param(i32_attr, "exclusiveMaximum", 1000);
    check_int_param(i32_attr, "multipleOf", 10);

    const auto& dbl_attr = get_field(properties, "dbl_attr");
    check_title(dbl_attr, "Double Attribute");
    check_description(dbl_attr, "Double-precision floating-point attribute");
    check_type(dbl_attr, "number");

    const auto& obj_attr = get_field(properties, "obj_attr");
    check_simple_example_schema(obj_attr);

    check_required_fields(schema, {"i32_attr", "dbl_attr", "obj_attr"});
}

void check_level2_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Level 2 Example");
    check_description(schema, "An example object that contains two levels of nested objects as attributes");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& u64_attr = get_field(properties, "u64_attr");
    check_title(u64_attr, "U64 Attribute");
    check_description(u64_attr, "64-bit unsigned integer attribute");
    check_type(u64_attr, "integer");

    const auto& smp_attr = get_field(properties, "smp_attr");
    check_simple_example_schema(smp_attr);

    const auto& lvl_attr = get_field(properties, "lvl_attr");
    check_level1_example_schema(lvl_attr);

    check_required_fields(schema, {"u64_attr", "smp_attr", "lvl_attr"});
}

void check_array_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Array Example");
    check_description(schema, "An example object that contains arrays of various types as attributes");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& i32_arr = get_field(properties, "i32_arr");
    check_title(i32_arr, "I32 Array Attribute");
    check_description(i32_arr, "An array of 32-bit integer attributes");
    check_type(i32_arr, "array");

    const auto& i32_items = get_field(i32_arr, "items");
    check_type(i32_items, "integer");
    check_int_param(i32_items, "minimum", std::numeric_limits<int>::min());
    check_int_param(i32_items, "maximum", std::numeric_limits<int>::max());

    const auto& dbl_arr = get_field(properties, "dbl_arr");
    check_title(dbl_arr, "Double Array Attribute");
    check_description(dbl_arr, "An array of double-precision floating-point attributes");
    check_type(dbl_arr, "array");
    check_int_param(dbl_arr, "minItems", 1);
    check_int_param(dbl_arr, "maxItems", 100);
    check_bool_param(dbl_arr, "uniqueItems", true);

    const auto& dbl_items = get_field(dbl_arr, "items");
    check_type(dbl_items, "number");
    check_double_param(dbl_items, "exclusiveMinimum", -1000.0);
    check_double_param(dbl_items, "exclusiveMaximum", 1000.0);
    check_double_param(dbl_items, "multipleOf", 0.1);

    // skip uint64_t, string, SimpleExample, Level1Example array attributes for brevity

    const auto& lvl2_arr = get_field(properties, "lvl2_arr");
    check_title(lvl2_arr, "Level2 Object Array Attribute");
    check_description(lvl2_arr, "An array of objects of type Level2Example");
    check_type(lvl2_arr, "array");

    const auto& lvl2_items = get_field(lvl2_arr, "items");
    check_level2_example_schema(lvl2_items);

    check_required_fields(schema, {});
}

void check_optional_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Optional Example");
    check_description(schema, "An example object that contains optional attributes of various types");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& i32_opt = get_field(properties, "i32_opt");
    check_title(i32_opt, "Optional I32 Attribute");
    check_description(i32_opt, "An optional 32-bit integer attribute");
    check_type(i32_opt, "integer");

    const auto& dbl_opt = get_field(properties, "dbl_opt");
    check_title(dbl_opt, "Optional Double Attribute");
    check_description(dbl_opt, "An optional double-precision floating-point attribute");
    check_type(dbl_opt, "number");

    const auto& u64_opt = get_field(properties, "u64_opt");
    check_title(u64_opt, "Optional U64 Attribute");
    check_description(u64_opt, "An optional 64-bit unsigned integer attribute");
    check_type(u64_opt, "integer");
    check_uint64_param(u64_opt, "minimum", std::numeric_limits<uint64_t>::min());
    check_uint64_param(u64_opt, "maximum", std::numeric_limits<uint64_t>::max());
    check_uint64_param(u64_opt, "multipleOf", 2);

    const auto& str_opt = get_field(properties, "str_opt");
    check_title(str_opt, "Optional String Attribute");
    check_description(str_opt, "An optional string attribute");
    check_type(str_opt, "string");

    const auto& lvl2_opt = get_field(properties, "lvl2_opt");
    check_level2_example_schema(lvl2_opt);

    check_required_fields(schema, {});
}

void check_enum_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Enum Example");
    check_description(schema, "An example object that contains an enum attribute and an array of enums as attributes");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& enum_attr = get_field(properties, "enum_attr");
    check_title(enum_attr, "Enum Attribute");
    check_description(enum_attr, "An attribute of type SimpleEnum");
    check_type(enum_attr, "string");
    check_enum_field(enum_attr, {"ONE", "TWO", "THREE"});

    const auto& enum_arr = get_field(properties, "enum_arr");
    check_title(enum_arr, "Enum Array Attribute");
    check_description(enum_arr, "An array of enum attributes");
    check_type(enum_arr, "array");

    const auto& enum_items = get_field(enum_arr, "items");
    check_title(enum_items, "Simple Enum");
    check_description(enum_items, "A simple example enum with unit, value, and alias enumerators");
    check_type(enum_items, "string");
    check_enum_field(enum_items, {"ONE", "TWO", "THREE"});

    check_required_fields(schema, {"enum_attr"});
}

void check_polymorphic_example_schema(const rapidjson::Value& schema) {
    ASSERT_TRUE(schema.IsObject());

    check_title(schema, "Polymorphic Object Example");
    check_description(schema, "An example of a polymorphic object with a type discriminator enum and two derived object types");
    check_type(schema, "object");

    const auto& properties = get_properties(schema);

    const auto& i32_attr = get_field(properties, "i32_attr");
    check_title(i32_attr, "I32 Attribute");
    check_description(i32_attr, "32-bit integer attribute");
    check_type(i32_attr, "integer");

    const auto& flt_attr = get_field(properties, "flt_attr");
    check_title(flt_attr, "Float Attribute");
    check_description(flt_attr, "Single-precision floating-point attribute");
    check_type(flt_attr, "number");

    check_required_fields(schema, {"i32_attr", "flt_attr"});

    const auto& alternatives = get_alternatives(schema);
    ASSERT_EQ(alternatives.Size(), 2);

    const auto& alt1 = alternatives[0];
    const auto& alt1_properties = get_properties(alt1);

    const auto& alt1_type = get_field(alt1_properties, "type");
    check_str_param(alt1_type, "const", "object1");

    const auto& u64_attr = get_field(alt1_properties, "u64_attr");
    check_title(u64_attr, "U64 Attribute");
    check_description(u64_attr, "64-bit unsigned integer attribute");
    check_type(u64_attr, "integer");

    const auto& dbl_attr = get_field(alt1_properties, "dbl_attr");
    check_title(dbl_attr, "Double Attribute");
    check_description(dbl_attr, "Double-precision floating-point attribute");
    check_type(dbl_attr, "number");

    check_required_fields(alt1, {"type", "u64_attr", "dbl_attr"});

    const auto& alt2 = alternatives[1];
    const auto& alt2_properties = get_properties(alt2);

    const auto& alt2_type = get_field(alt2_properties, "type");
    check_str_param(alt2_type, "const", "object2");

    const auto& bool_attr = get_field(alt2_properties, "bool_attr");
    check_title(bool_attr, "Boolean Attribute");
    check_description(bool_attr, "A boolean attribute");
    check_type(bool_attr, "boolean");

    const auto& str_attr = get_field(alt2_properties, "str_attr");
    check_title(str_attr, "String Attribute");
    check_description(str_attr, "A string attribute");
    check_type(str_attr, "string");

    check_required_fields(alt2, {"type", "bool_attr", "str_attr"});
}

TEST(SchemaTests, SimpleExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<SimpleExample>();
    check_simple_example_schema(schema_doc);
}

TEST(SchemaTests, Level1ExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<Level1Example>();
    check_level1_example_schema(schema_doc);
}

TEST(SchemaTests, Level2ExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<Level2Example>();
    check_level2_example_schema(schema_doc);
}

TEST(SchemaTests, ArrayExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<ArrayExample>();
    check_array_example_schema(schema_doc);
}

TEST(SchemaTests, OptionalExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<OptionalExample>();
    check_optional_example_schema(schema_doc);
}

TEST(SchemaTests, EnumExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<EnumExample>();
    check_enum_example_schema(schema_doc);
}

TEST(SchemaTests, PolymorphicExampleSchema) {
    auto schema_doc = macrojson::get_json_schema_doc<BaseExample>();
    check_polymorphic_example_schema(schema_doc);
}
