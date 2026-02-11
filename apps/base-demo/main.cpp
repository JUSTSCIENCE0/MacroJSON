// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "schema_example.hpp"
#include "write_example.hpp"
#include "read_example.hpp"

int main() {
    simple_schema_example();
    level1_schema_example();
    level2_schema_example();
    optional_schema_example();
    array_schema_example();
    enum_schema_example();

    simple_write_example();
    level1_write_example();
    level2_write_example();
    array_write_example();
    enum_write_example();
    polymorphic_simple_write_example();
    polymorphic_complex_write_example();

    simple_read_example();
    level1_read_example();
    level2_read_example();
    array_read_example();
    enum_read_example();
    polymorphic_simple_read_example();
    polymorphic_complex_read_example();
    return 0;
}
