# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(base-test)

macrojson_generate_header(
    test_descr.h
    test_samples.hpp
)

add_executable(${PROJECT_NAME}
    test_descr.h
    generated/test_samples.hpp

    ancillary.hpp
    write_unit_tests.cpp
    read_unit_tests.cpp
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
        GTest::gtest
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
