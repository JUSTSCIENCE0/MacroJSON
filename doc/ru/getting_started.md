# Простой пример для начала

Пусть у нас есть JSON объекты, которые мы хотим читать и записывать в C++:

```json
{
  "name": "Mike Smith",
  "age": 21,
  "email": "mike.smith@example.com",
  "employee": {
    "id": 67890,
    "department": "Marketing",
    "position": "Manager"
  }
}

{
  "name": "John Doe",
  "age": 30,
  "email": "john.doe@example.com",
  "employee": {
    "id": 12345,
    "department": "Engineering"
  },
  "skills": ["C++", "JSON", "MacroJson"]
}
```

С помощью **MacroJson** мы можем описать структуру данных, сразу же документировать ее и определить правила валидации:

```c
// Определяем вложенную структуру данных сотрудника
MJSON_OBJECT_BEGIN(Employee,
        "Employee Info", nullptr)
    MJSON_FIELD(int, id,
        "Employee ID",
        "Unique identifier for the employee")
    MJSON_FIELD(std::string, department,
        "Department",
        "Department where the employee works")
    MJSON_FIELD(std::optional<std::string>, position,
        "Position",
        "Job position of the employee")
MJSON_OBJECT_END(Employee)

// Определяем основную структуру данных человека
MJSON_OBJECT_BEGIN(Person,
        "Person Info",
        "Information about a person, including their name, age, email, and employment details")
    MJSON_FIELD(std::string, name,
        "Name",
        "Full name of the person")
    MJSON_FIELD(int, age,
        "Age",
        "Age of the person in years",
        macrojson::Range<int>{18, /*inclusive*/ false, 100, /*exclusive*/ true})
    MJSON_FIELD(std::string, email,
        "Email",
        "Email address of the person",
        macrojson::StringRegex{ R"(^\S+@\S+\.\S+$)" },
        macrojson::StringLength{ 5, 100 })
    MJSON_FIELD(Employee, employee,
        "Employee Information",
        "Information about the person's employment")
    MJSON_FIELD(std::vector<std::string>, skills,
        "Skills",
        "List of skills the person has",
        macrojson::ArrayParams{ 0, 10, true },
        macrojson::StringLength{ 1, 50 })
MJSON_OBJECT_END(Person)
```

C помощью CMake функции `macrojson_generate_header` мы генерируем на основе описания структуры данных заголовочный файл, содержащий определение структуры, генератор JSON схемы и функции для сериализации и десериализации. \
После генерации мы можем использовать эти функции для работы с JSON данными:

```cpp
#include "person.hpp"

// получение JSON схемы
auto person_schema = macrojson::get_json_schema_str<Person>();
std::cout << "Person JSON Schema:\n" << person_schema << std::endl;
```
<details>
<summary>Вывод</summary>

```json
{
    "$schema": "https://json-schema.org/draft/2020-12/schema",
    "title": "Person Info",
    "description": "Information about a person, including their name, age, email, and employment details",
    "type": "object",
    "properties": {
        "name": {
            "title": "Name",
            "description": "Full name of the person",
            "type": "string"
        },
        "age": {
            "title": "Age",
            "description": "Age of the person in years",
            "type": "integer",
            "minimum": 18,
            "exclusiveMaximum": 100
        },
        "email": {
            "title": "Email",
            "description": "Email address of the person",
            "type": "string",
            "pattern": "^\\S+@\\S+\\.\\S+$",
            "minLength": 5,
            "maxLength": 100
        },
        "employee": {
            "title": "Employee Information",
            "description": "Information about the person's employment",
            "type": "object",
            "properties": {
                "id": {
                    "title": "Employee ID",
                    "description": "Unique identifier for the employee",
                    "type": "integer"
                },
                "department": {
                    "title": "Department",
                    "description": "Department where the employee works",
                    "type": "string"
                },
                "position": {
                    "title": "Position",
                    "description": "Job position of the employee",
                    "type": "string"
                }
            },
            "required": [
                "id",
                "department"
            ]
        },
        "skills": {
            "title": "Skills",
            "description": "List of skills the person has",
            "type": "array",
            "maxItems": 10,
            "uniqueItems": true,
            "items": {
                "type": "string",
                "minLength": 1,
                "maxLength": 50
            }
        }
    },
    "required": [
        "name",
        "age",
        "email",
        "employee"
    ]
}
```

</details>

```cpp
// проверка JSON данных на соответствие схеме
const char* person_json_str = ...;

std::string error_descr{};
macrojson::MJsonErrorCode err_code = macrojson::validate_json_str<Person>(person_json_str, error_descr);
if (err_code != macrojson::E_MJSON_OK) {
    std::cerr << "JSON does not conform to schema: " << error_descr << std::endl;
    return 1;
} else {
    std::cout << "JSON is valid according to schema." << std::endl;
}
```

```cpp
// десериализация JSON строки в структуру данных
const char* person_json_str = ...;

Person person_in{};
error_descr = "";
err_code = macrojson::json_str_to_object(person_json_str, person_in, error_descr);
if (err_code != macrojson::E_MJSON_OK) {
    std::cerr << "Error parsing JSON: " << error_descr << std::endl;
    return 1;
}
```

```cpp
// сериализация структуры данных в JSON строку
std::string person_json = "";
macrojson::object_to_json_str(person_out, person_json);
std::cout << "Person JSON:\n" << person_json << std::endl;
```

Код - лучшая документация? Смотри [демо-пример](../../apps/doc-demo)
