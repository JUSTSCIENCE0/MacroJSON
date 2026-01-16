# Copyright (c) 2026, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

function(macrojson_generate_header
    descr_file
    output_file
)
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