# Генерируемые функции

## Коды ошибок

Перечисление `MJsonErrorCode` содержит следующие коды ошибок:
- `E_MJSON_OK` - операция выполнена успешно;
- `E_MJSON_NOT_EXISTS` - указанный файл не существует;
- `E_MJSON_TYPE_MISMATCH` - тип данных не соответствует ожидаемому;
- `E_MJSON_FILE_ERROR` - ошибка при работе с файлом;
- `E_MJSON_PARSE_FAILED` - не удалось распарсить JSON-строку или документ;
- `E_MJSON_UNKNOWN_ENUM` - значение перечисления не распознано;
- `E_MJSON_UNSUPPORTED_OBJECT_TYPE` - тип объекта не поддерживается для сериализации/десериализации;
- `E_MJSON_SCHEMA_VALIDATION_FAILED` - JSON документ не прошел валидацию по схеме. В этом случае `error_descr` будет содержать подробное описание ошибок валидации.

## Тип объекта
Тип объекта - это тип данных, который описан с помощью макросов библиотеки MacroJson. Это может быть структура, перечисление, массив или полиморфный тип. Далее, Т - это тип объекта.

## Сериализация

Функции для сериализации объектов в JSON-формат.

```cpp
template <typename T>
void object_to_json_str(const T& object, std::string& out);
```
object - объект, который нужно сериализовать.\
out - строка, в которую будет записан результат сериализации.

```cpp
template <typename T>
MJsonErrorCode object_to_json_file(const T& object, const std::filesystem::path& json_file)
```
object - объект, который нужно сериализовать.\
json_file - путь к файлу, в который будет записан результат сериализации. Если файл уже существует, он будет перезаписан.

## Десериализация

```cpp
template <typename T>
MJsonErrorCode json_doc_to_object(const rapidjson::Document& doc, T& object, std::string& error_descr)
```
doc - JSON документ, который нужно десериализовать.\
object - объект, в который будет записан результат десериализации.\
error_descr - строка, в которую будет записано описание ошибки в случае неудачной десериализации. В случае успешной десериализации будет пустой.

```cpp
template <typename T>
MJsonErrorCode json_str_to_object(const std::string& json, T& object, std::string& error_descr)
```
json - JSON строка, которую нужно десериализовать.\
object - объект, в который будет записан результат десериализации.\
error_descr - строка, в которую будет записано описание ошибки в случае неудачной десериализации. В случае успешной десериализации будет пустой.

```cpp
template <typename T>
MJsonErrorCode json_file_to_object(const std::filesystem::path& json_file, T& object, std::string& error_descr)
```
json_file - путь к файлу, из которого нужно десериализовать JSON.\
object - объект, в который будет записан результат десериализации.\
error_descr - строка, в которую будет записано описание ошибки в случае неудачной десериализации. В случае успешной десериализации будет пустой.

## JSON Schema

```cpp
template <typename T>
rapidjson::Document get_json_schema_doc(const char* id = nullptr, const char* title = nullptr, const char* description = nullptr)
```
id - идентификатор схемы "$id". Если не указан, то схема будет без "$id".\
title - заголовок схемы "title". Если не указан, то будет использовать title указанный для объекта Т по умолчанию.\
description - описание схемы "description". Если не указано, то будет использовать description указанный для объекта Т по умолчанию.

Возвращает JSON документ, представляющий JSON схему для типа T.

```cpp
template <typename T>
std::string get_json_schema_str(const char* id = nullptr, const char* title = nullptr, const char* description = nullptr)
```
id - идентификатор схемы "$id". Если не указан, то схема будет без "$id".\
title - заголовок схемы "title". Если не указан, то будет использовать title указанный для объекта Т по умолчанию.\
description - описание схемы "description". Если не указано, то будет использовать description указанный для объекта Т по умолчанию.

Возвращает строку JSON, представляющий JSON схему для типа T.

## Валидация
```cpp
template <typename T>
MJsonErrorCode validate_json_doc(const rapidjson::Document& doc, std::string& error_descr)
```
doc - JSON документ, который нужно валидировать.\
error_descr - строка, в которую будет записано описание ошибки в случае неудачной валидации. В случае успешной валидации будет пустой.

```cpp
template <typename T>
static inline MJsonErrorCode validate_json_str(const std::string& json, std::string& error_descr)
```
json - JSON строка, которую нужно валидировать.\
error_descr - строка, в которую будет записано описание ошибки в случае неудачной валидации. В случае успешной валидации будет пустой.

