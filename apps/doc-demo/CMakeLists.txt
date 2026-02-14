# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(doc-demo)

macrojson_generate_header(
    person_descr.h
    person.hpp
    TRUE
)

add_executable(${PROJECT_NAME}
    person_descr.h
    generated/person.hpp

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

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "apps")
