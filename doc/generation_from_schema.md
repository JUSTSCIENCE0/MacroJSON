# Code Generation from JSON Schema (Beta)

    Attention! The functionality of generating code from schema is under development and does not support all features of JSON Schema.

If you already have a description of the data types you are working with in the form of a JSON Schema, then **MacroJson** allows you to automatically generate C++ structures and functions for (de)serialization based on this schema.

## Generator

```cmake
macrojson_parse_schema(schema_file, output_file, create_objects)
```
schema_file - path to the JSON Schema file that needs to be processed. \
output_file - the name of the generated header file. The generated file will be created in a `generated` subfolder within your project folder. Add `generated` to your version control system's ignore list.\
create_objects - a flag indicating whether objects of the data structure should be created. If `TRUE`, objects of the data structure will be created; if `FALSE`, only serialization/deserialization functions and schemas will be generated.

Additionally, `macrojson_parse_schema` generates additional helper header files in the `generated` folder.

To use the function, you need to have Python 3.x interpreter installed on your system.

## Usage

To use the CMake generator, follow these steps:
0. Add the MacroJson library to your project.
1. In your project's CMakeLists.txt file, add the following code to include the CMake generator:

```cmake
# include header-generator
include(MacroJSON/cmake/HeaderGenerator.cmake)
```
2. In your project's CMakeLists.txt file, call the `macrojson_parse_schema()` function to generate a header file. For example:

```cmake
macrojson_parse_schema(
    person_schema.json
    person.hpp
    TRUE
)
```

3. Add the generated header file to the list of source files in your project. For example:

```cmake
# add executable
add_executable(my_app main.cpp generated/person.hpp)
```


Is code the best documentation? Check out [demo example](../apps/gen-demo)