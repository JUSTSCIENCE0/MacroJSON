# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

function(macrojson_generate_header
    descr_file
    output_file
    create_objects
)
    if (create_objects)
        set(MJSON_DEFINE_OBJECT 1)
    else()
        set(MJSON_DEFINE_OBJECT 0)
    endif()

    set(MJSON_HEADER_TEMPLATE
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Templates/MacroJsonHeader.template)
    set(MJSON_OUTPUT_FILE
        ${CMAKE_CURRENT_LIST_DIR}/generated/${output_file})
    set(MJSON_DESCR_FILE ${descr_file})
    configure_file(
        ${MJSON_HEADER_TEMPLATE}
        ${MJSON_OUTPUT_FILE}
        @ONLY
    )
endfunction()

function(macrojson_parse_schema
    schema_file
    output_file
    create_objects
)
    if(NOT DEFINED PYTHON_EXECUTABLE OR PYTHON_EXECUTABLE STREQUAL "")
        find_package(Python3 REQUIRED COMPONENTS Interpreter)

        if(Python3_FOUND)
            message(STATUS "Python3 Interpreter: ${Python3_EXECUTABLE}")
        else()
            message(FATAL_ERROR "Python3 not found")
        endif()
    endif()

    set(MJSON_GENERATOR_SCRIPT "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../gen/generator_from_schema.py")
    set(MJSON_DESCR_OUTPUT_FILE "${CMAKE_CURRENT_LIST_DIR}/generated/descr_${output_file}")

    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${MJSON_GENERATOR_SCRIPT} ${schema_file} ${MJSON_DESCR_OUTPUT_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        RESULT_VARIABLE MJSON_GENERATION_RESULT
    )

    if(MJSON_GENERATION_RESULT)
        message(FATAL_ERROR "Generator script failed with error code: ${MJSON_GENERATION_RESULT}")
    endif()

    macrojson_generate_header(
        descr_${output_file}
        ${output_file}
        ${create_objects}
    )
endfunction(macrojson_parse_schema)