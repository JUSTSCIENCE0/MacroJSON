# Header File Generation with CMake

**MacroJson** provides a CMake generator that automatically generates header files describing data structures based macros.

## Generator

```cmake
macrojson_generate_header(descr_file output_file create_objects)
```
descr_file - the path to the file containing the data structure description, which uses macros from the MacroJson library.\
output_file - the name of the generated header file. The generated file will be created in a `generated` subfolder within your project folder. Add `generated` to your version control system's ignore list.\
create_objects - a flag indicating whether objects of the data structure should be created. If `TRUE`, objects of the data structure will be created; if `FALSE`, only serialization/deserialization functions and schemas will be generated.

## Usage

To use the CMake generator, follow these steps:
0. Add the MacroJson library to your project.
1. In your project's CMakeLists.txt file, add the following code to include the CMake generator:

```cmake
# include header-generator
include(MacroJSON/cmake/HeaderGenerator.cmake)
```
2. In your project's CMakeLists.txt file, call the `macrojson_generate_header()` function to generate a header file. For example:

```cmake
# generate header file
macrojson_generate_header(
    person_descr.h
    person.hpp
    TRUE
)
```

3. Add the generated header file to the list of source files in your project. For example:

```cmake
# add executable
add_executable(my_app main.cpp generated/person.hpp)
```