
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
