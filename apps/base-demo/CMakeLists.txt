# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(base-demo)

macrojson_generate_header(
    example_descr.h
    example.hpp
    TRUE
)

add_executable(${PROJECT_NAME}
    example_descr.h
    generated/example.hpp

    schema_example.cpp
    schema_example.hpp
    write_example.cpp
    write_example.hpp
    read_example.cpp
    read_example.hpp
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/
        ${CMAKE_CURRENT_LIST_DIR}/generated
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MacroJSON
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        JSON_EXAMPLES_DIRECTORY="${CMAKE_CURRENT_LIST_DIR}/json_samples/"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")