
# print a cmake variables to the command line
function(print var)
    message("${var} = ${${var}}")
endfunction()

# use clang-format to format code
function(clang_format target directory)
    message(STATUS "Formatting files for ${target}")

    find_program(CLANG-FORMAT_PATH clang-format REQUIRED)
    message(STATUS "Found clang-format!")

    set(EXPRESSION h hpp hh c cc cxx cpp)
    message(STATUS "Searching for ${EXPRESSION} files in ${directory}...")

    list(TRANSFORM EXPRESSION PREPEND "${directory}/*.")
    message(STATUS "Searching for ${EXPRESSION}...")

    file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS LIST_DIRECTORIES false ${EXPRESSION})

    message(STATUS "FILES: ${SOURCE_FILES}")
    add_custom_command(TARGET ${target} PRE_BUILD COMMAND 
        ${CLANG-FORMAT_PATH} -i --style=file ${SOURCE_FILES}
    )
    message(STATUS "Formatting done!")
endfunction()

## the following 2 should be used on the unit-test target:
# use clang-tidy to do static analysis on the code
function(clang_tidy target)
    find_program(CLANG-TIDY_PATH clang-tidy REQUIRED)
    set_target_properties(${target}
        PROPERTIES CXX_CLANG_TIDY
        "${CLANG-TIDY_PATH};-checks=*;--warnings-as-errors=*"
    )
endfunction()

# use valgrind to find memory issues
function(valgrind target)
    find_program(VALGRIND_PATH valgrind REQUIRED)
    add_custom_target(valgrind_target
        COMMAND ${VALGRIND_PATH} --leak-check=yes
            $<TARGET_FILE:${target}>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
# memcheck-cover can be used to view the output as html
endfunction()

# doxygen helper function to generate documentation
function(generate_doxygen input output)
    find_package(Doxygen)
    if (NOT DOXYGEN_FOUND)
        add_custom_target(doxygen COMMAND false
            COMMENT "Doxygen was not found!"
        )
        return()
    endif()
    set(DOXYGEN_OUTPUT_DIRECTORY "${output}")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_PDF YES)

    doxygen_add_docs(doxygen 
        "${input}"
        ALL 
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Generate Doxygen Documentation"
    )
endfunction()

