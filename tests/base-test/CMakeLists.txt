# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(base-test)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MacroJSON
        GTest::gtest
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
