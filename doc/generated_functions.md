# Generated Functions

## Error Codes

The enumeration `MJsonErrorCode` contains the following error codes:
- `E_MJSON_OK` - operation completed successfully;
- `E_MJSON_NOT_EXISTS` - specified file does not exist;
- `E_MJSON_TYPE_MISMATCH` - data type does not match expected;
- `E_MJSON_FILE_ERROR` - error occurred while working with a file;
- `E_MJSON_PARSE_FAILED` - failed to parse JSON string or document;
- `E_MJSON_UNKNOWN_ENUM` - enumeration value is unrecognized;
- `E_MJSON_UNSUPPORTED_OBJECT_TYPE` - object type is not supported for serialization/deserialization;
- `E_MJSON_SCHEMA_VALIDATION_FAILED` - JSON document did not pass validation against the schema. In this case, `error_descr` will contain a detailed description of validation errors.

## Object Type
An object type is a data type described using macros from the MacroJson library. This can be a structure, enumeration, array, or polymorphic type. Hereafter, T represents the object type.

## Serialization

Functions for serializing objects to JSON format.

```cpp
template <typename T>
void object_to_json_str(const T& object, std::string& out);
```
object - the object that needs to be serialized.\
out - the string where the serialization result will be written.

```cpp
template <typename T>
MJsonErrorCode object_to_json_file(const T& object, const std::filesystem::path& json_file)
```
object - the object that needs to be serialized.\
json_file - the path to the file where the serialization result will be written. If the file already exists, it will be overwritten.

## Deserialization

```cpp
template <typename T>
MJsonErrorCode json_doc_to_object(const rapidjson::Document& doc, T& object, std::string& error_descr)
```
doc - the JSON document that needs to be deserialized.\
object - the object where the deserialization result will be written.\
error_descr - the string where the description of the error will be written in case of unsuccessful deserialization. In case of successful deserialization, it will be empty

```cpp
template <typename T>
MJsonErrorCode json_str_to_object(const std::string& json, T& object, std::string& error_descr)
```
json - the JSON string that needs to be deserialized.\
object - the object where the deserialization result will be written.\
error_descr - the string where the description of the error will be written in case of unsuccessful deserialization. In case of successful deserialization, it will be empty.

```cpp
template <typename T>
MJsonErrorCode json_file_to_object(const std::filesystem::path& json_file, T& object, std::string& error_descr)
```
json_file - the path to the file from which the JSON needs to be deserialized.\
object - the object where the deserialization result will be written.\
error_descr - the string where the description of the error will be written in case of unsuccessful deserialization. In case of successful deserialization, it will be empty

## JSON Schema

```cpp
template <typename T>
rapidjson::Document get_json_schema_doc(const char* id = nullptr, const char* title = nullptr, const char* description = nullptr)
```
id - the identifier of the schema, "$id" field. If not specified, the schema will be without "$id".\
title - "title" field of the schema. If not specified, it will use the title specified for object T by default.\
description - "description" field of the schema. If not specified, it will use the description specified for object T by default.

Returns a JSON document representing the JSON schema for type T.

```cpp
template <typename T>
std::string get_json_schema_str(const char* id = nullptr, const char* title = nullptr, const char* description = nullptr)
```
id - the identifier of the schema, "$id" field. If not specified, the schema will be without "$id".\
title - "title" field of the schema. If not specified, it will use the title specified for object T by default.\
description - "description" field of the schema. If not specified, it will use the description specified for object T by default.

Returns a string JSON representing the JSON schema for type T.

## Validation
```cpp
template <typename T>
MJsonErrorCode validate_json_doc(const rapidjson::Document& doc, std::string& error_descr)
```
doc - JSON document to validate.\
error_descr - string where the error description will be written in case of validation failure. Will be empty in case of successful validation.

```cpp
template <typename T>
static inline MJsonErrorCode validate_json_str(const std::string& json, std::string& error_descr)
```
json - JSON string to validate.\
error_descr - string where the error description will be written in case of validation failure. Will be empty in case of successful validation.

