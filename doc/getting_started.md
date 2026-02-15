# Simple example to get started

Let's say we have JSON objects that we want to read and write in C++:
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

Using **MacroJson**, we can describe the data structure, immediately document it, and define validation rules:
```cpp
// define the nested employee data structure
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

// define the main data structure for a person
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

Using CMake function `macrojson_generate_header`, we generate a header file based on the data structure description. The header file contains the definition of the structure, JSON schema generator and functions for serialization and deserialization. \
After generation, we can use these functions to work with JSON data:

```cpp
#include "person.hpp"

// getting JSON schema
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
// checking JSON data for compliance with the schema
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
// deserialization of JSON string into data structure
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
// serialization of data structure to JSON string
std::string person_json = "";
macrojson::object_to_json_str(person_out, person_json);
std::cout << "Person JSON:\n" << person_json << std::endl;
```

Is code the best documentation? Check out [demo example](../apps/doc-demo)
