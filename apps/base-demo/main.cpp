// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

#include "write_example.hpp"
#include "read_example.hpp"

int main() {
    simple_write_example();
    level1_write_example();
    level2_write_example();
    array_write_example();
    enum_write_example();
    polymorphic_write_example();

    simple_read_example();
    level1_read_example();
    level2_read_example();
    array_read_example();
    enum_read_example();
    polymorphic_read_example();
    return 0;
}
