# MacroJson

**MacroJson** is a header-only library for reading, writing, and documenting JSON objects based on declarative description of data structure. \
The library allows automatically generating the following:
- C++ structure code
- JSON schema for documentation and data validation
- Functions for serializing and deserializing JSON objects

RapidJSON library is used as a backend for working with JSON data.

## What problem does the library solve
When working with JSON data in C++, it often requires writing duplicate code with identical semantics. This leads to duplicated logic, desynchronization between data structures, implementation of (de)serializers and documentation. **MacroJson** allows describing the data structure **once** and automatically generating all other code. This eliminates duplication, ensures consistency between all components and simplifies code maintenance.

## Documentation and usage examples
- [Getting Started](doc/getting_started.md)
- [CMake Generator](doc/cmake_generator.md)
- [Generated Functions](doc/generated_functions.md)
- [Macro Description](doc/macro_description.md)
- [Advanced Features](doc/advanced_features.md)
- [Supported Data Types](doc/supported_types.md)
- [Validation Parameters](doc/validation_params.md)
- [Code Generation from JSON Schema (Beta)](doc/generation_from_schema.md)
- [Usage Example](apps/doc-demo) and [full functionality demonstration](apps/base-demo)

## Third-party dependencies

- [RapidJSON](https://github.com/Tencent/rapidjson.git) - backend for working with JSON data. Fast and lightweight JSON parser, generator and validator for C++.

[Russian version](doc/ru/readme.md)