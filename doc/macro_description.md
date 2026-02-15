# Macros

## Object
### MJSON_OBJECT_BEGIN
Declares the beginning of an object description. Must be called before declaring fields of the object.
Arguments:
- name - the name of the object. Acts as an identifier for the structure that will be generated. It should be unique within the entire description and comply with C/C++ naming conventions for identifiers.
- def_title - default title of the object. Acts as a string literal that will be used as the `title` field in JSON schema if another value is not specified when describing the field. Must be a string literal. Specify `nullptr` if you do not want to use a default title.
- def_description - default description of the object. Acts as a string literal that will be used as the `description` field in JSON schema if another value is not specified when describing the field. Must be a string literal. Specify `nullptr` if you do not want to use a default description.

### MJSON_FIELD
Declares an object field.
Arguments:
- type - the type of the field. Must be one of the supported data types (see [Supported Data Types](supported_types.md)) or the name of another object described above in the code.
- name - the name of the field. Acts as an identifier that will be used as the field name in the generated structure and in JSON schema. It must be unique within the object and comply with C/C++ naming conventions for identifiers.
- title - the title of the field. Acts as a string literal that will be used as the `title` field in JSON schema. Must be a string literal. Specify `nullptr` if you do not want to use a title for this field or use the default object title.
- description - the description of the field. Acts as a string literal that will be used as the `description` field in JSON schema. Must be a string literal. Specify `nullptr` if you do not want to use a description for this field or use the default object description.
- validation_params - validation parameters of the field. Acts as a variadic list of arguments that may contain zero or more validation parameters applicable to this field. Each validation parameter must be one of the supported types of validation parameters and be consistent with the type of the field (see [Validation Parameters](validation_params.md)).

### MJSON_OBJECT_END
Declares the end of an object description. Must be called after declaring all fields of the object.
Arguments:
- name - the name of the object. Must match the name specified in **MJSON_OBJECT_BEGIN** for this object. Requirements for the argument are similar to those for the name argument in **MJSON_OBJECT_BEGIN**.

## Перечисление

### MJSON_ENUM_BEGIN
Declares the beginning of an enumeration description. Must be called before declaring values of the enumeration.
Arguments:
- name - the name of the enumeration. Acts as an identifier for the enumeration type that will be generated. It must be unique within the entire description and comply with C/C++ naming conventions for identifiers.
- def_title - default title of the enumeration. Acts as a string literal that will be used as the `title` field in JSON schema if another value is not specified when describing the field. Must be a string literal. Specify `nullptr` if you do not want to use a default title.
- def_description - default description of the enumeration. Acts as a string literal that will be used as the `description` field in JSON schema if another value is not specified when describing the field. Must be a string literal. Specify `nullptr` if you do not want to use a default description.

### MJSON_ENUM_UNIT
Declares an enumeration element with automatically calculated value.
Arguments:
- name - the name of the enumeration element. Acts as an identifier that will be used as the element name in the generated enumeration. Must be unique within the enumeration and comply with C/C++ naming conventions for identifiers.
- short_name - the short name of the enumeration element. Acts as a string value that will be used as the value of the element in JSON.

### MJSON_ENUM_VALUE
Declares an enumeration element with explicitly specified value.
Arguments:
- name - the name of the enumeration element. Acts as an identifier that will be used as the element name in the generated enumeration. Must be unique within the enumeration and comply with C/C++ naming conventions for identifiers.
- short_name - the short name of the enumeration element. Acts as a string value that will be used as the value of the element in JSON.
- value - the value of the enumeration element. Acts as an integer literal that will be used as the value of the element in the generated enumeration. Must be unique within the enumeration.

### MJSON_ENUM_ALIAS
Declares an alias for an enumeration element.
Arguments:
- name - the name of the alias. Acts as an identifier that will be used as the alias name in the generated enumeration. It must be unique within the enumeration and comply with C/C++ naming conventions for identifiers.
- short_name - the short name of the enumeration element. Acts as a string value that will be used as the value of the element in JSON.
- value - the name or value of the enumeration element to which the alias refers. If an element name is specified, it must be an identifier that was declared earlier in this same enumeration. If a value is specified, it must be an integer literal that corresponds to the value of one of the elements declared earlier in this same enumeration.

### MJSON_ENUM_END
Declares the end of an enumeration description. Must be called after declaring all elements of the enumeration.
Arguments:
- name - the name of the enumeration. It must match the name specified in **MJSON_ENUM_BEGIN** for this enumeration. Requirements for the argument are similar to those for the name argument in **MJSON_ENUM_BEGIN**.

## Полиморфный объект

It is recommended to read about the concept of polymorphic objects and their support in **MacroJson** in the [Advanced Features](advanced_features.md) section.

To describe a polymorphic object, you need to first define (`#define`) the `MJSON_BASE_OBJECT_NAME` macro before calling **MJSON_POLYMORPHIC_OBJECT_BEGIN**, containing the name of the base object type. After completing the description of the polymorphic object and after calling **MJSON_POLYMORPHIC_OBJECT_END**, the `MJSON_BASE_OBJECT_NAME` macro must be undefined.

### MJSON_POLYMORPHIC_OBJECT_BEGIN
Declares the beginning of a polymorphic object description. Must be called before defining the base and all derived objects.
Arguments:
- def_title - default title of the object. Acts as a string literal that will be used as the `title` field in JSON schema if another value is not specified when describing the field. Must be a string literal. Specify `nullptr` if you do not want to use a default title.
- def_description - default description of the object. Acts as a string literal that will be used as the `description` field in JSON schema if another value is not specified when describing the field. Must be a string literal. Specify `nullptr` if you do not want to use a default description.

### MJSON_BASE_OBJECT_BEGIN
Declares the beginning of a base object description for a polymorphic object. Must be called after **MJSON_POLYMORPHIC_OBJECT_BEGIN** and before defining derived objects.
Arguments:
- types_enum - the name of an enumeration previously declared, whose value will determine the specific type of the derived object. Requirements for this argument are similar to those for the `name` argument in **MJSON_ENUM_BEGIN**.

### MJSON_BASE_OBJECT_FIELD
Declares a field of the base object for a polymorphic object.
Arguments are similar to those of the **MJSON_FIELD** macro.

### MJSON_BASE_OBJECT_END
Declares the end of a base object description for a polymorphic object. Must be called after defining all fields of the base object and before defining derived objects.
No arguments are required.

### MJSON_DERIVED_OBJECT_BEGIN
Declares the beginning of a derived object description for a polymorphic object. Must be called after declaring the base object and before declaring fields of the derived object. Can be called multiple times to describe several derived objects.
Arguments:
- name - the name of the derived object. Acts as an identifier for the type structure that will be generated for this derived object. Must be unique within the entire description and comply with C/C++ naming conventions for identifiers.
- type_enumerator - the value of the enumeration declared earlier and used in the base object, which corresponds to this derived object. Must be an identifier that was declared earlier in this same enumeration.

### MJSON_DERIVED_OBJECT_FIELD
Declares a field of the derived object type for a polymorphic object.
Arguments are similar to those of the **MJSON_FIELD** macro.

### MJSON_DERIVED_OBJECT_END
Declares the end of a derived object description for a polymorphic object. Must be called after declaring all fields of the derived object and before declaring the next derived object or ending the description of the polymorphic object.
Arguments:
- name - the name of the derived object. Must match the name specified in **MJSON_DERIVED_OBJECT_BEGIN** for this derived object. Requirements for the argument are similar to those for the name argument in **MJSON_DERIVED_OBJECT_BEGIN**.

### MJSON_POLYMORPHIC_OBJECT_END
Declares the end of a polymorphic object description. Must be called after declaring all derived objects.
No arguments are required.
