# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(gen-demo)

macrojson_parse_schema(
    schemas/array_schema_example.json
    array_example.hpp
    TRUE
)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MacroJSON
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")
