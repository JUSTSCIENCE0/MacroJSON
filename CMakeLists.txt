# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

project(MacroJSON LANGUAGES CXX)

if (NOT PROJECT_IS_TOP_LEVEL)
    message(STATUS "MacroJSON: applications and tests are disabled in submodule")
    set(MJSON_APPS OFF)
    set(MJSON_TEST OFF)
endif()

option(MJSON_APPS        "Build example applications" ON)
option(MJSON_TEST        "Build tests" ON)

if (PROJECT_IS_TOP_LEVEL)
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        add_compile_options(
            /W4 /WX               # warnings options
            /DUNICODE /D_UNICODE  # use unicode
            /MP                   # multiple processes build
            /Zc:preprocessor      # __VA_ARG__ support
        )
    elseif ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        add_compile_options(
            # warnings options
            -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual
            -Wdisabled-optimization -Wformat=2 -Winit-self 
            -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual
            -Wredundant-decls -Wshadow -Wsign-promo 
            -Wundef -Wno-variadic-macros
            -Wno-parentheses -fdiagnostics-show-option
            -Werror
        )
    endif() # GCC & Clang

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

include(FetchContent)

# RapidJSON
if (NOT TARGET RapidJSON)
    FetchContent_Declare(
        RapidJSON
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG        24b5e7a8b27f42fa16b96fc70aade9106cf7102f # latest master 05.02.2025
    )
    set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(RapidJSON)
    set_property(TARGET travis_doc PROPERTY FOLDER "externals/RapidJSON")
endif()

# googletest
if (MJSON_TEST)
    if (NOT TARGET gtest)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # v1.17.0
        )
        FetchContent_MakeAvailable(googletest)
        set_property(TARGET gmock gmock_main gtest gtest_main PROPERTY FOLDER "externals")
    endif()
endif()

add_library(MacroJSON INTERFACE)

target_sources(MacroJSON
    PRIVATE
        include/macrojson/macrojson.hpp
        include/macrojson/object.hpp
        include/macrojson/reader.hpp
        include/macrojson/schema.hpp
        include/macrojson/writer.hpp
        include/macrojson/undef_macros.h
)

target_include_directories(MacroJSON
    INTERFACE
        include/
        ${RapidJSON_SOURCE_DIR}/include/
)

target_link_libraries(MacroJSON
    INTERFACE
        RapidJSON
)

# include header-generator
include(cmake/HeaderGenerator.cmake)

if (MJSON_APPS)
    add_subdirectory(apps)
endif(MJSON_APPS)

if (MJSON_TEST)
    add_subdirectory(tests)
endif(MJSON_TEST)
