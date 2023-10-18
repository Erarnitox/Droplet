
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
    # check if doxygen is available
    find_package(Doxygen)
    if (NOT DOXYGEN_FOUND)
        add_custom_target(doxygen COMMAND false
            COMMENT "Doxygen was not found!"
        )
        return()
    endif()

    # download a modern style for the doxygen documentation
    include(FetchContent)
    FetchContent_Declare(doxygen-awesome-css 
        GIT_REPOSITORY
            https://github.com/jothepro/doxygen-awesome-css.git
        GIT_TAG
            v2.2.1
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)
    
    # set doxygen options
    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css")

    set(DOXYGEN_OUTPUT_DIRECTORY "${output}")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)

    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_CLASS_DIAGRAMS YES)
    set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CLASS_GRAPH YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_UML_LOOK YES)
    set(DOXYGEN_TEMPLATE_RELATIONS YES)
    set(DOXYGEN_DOT_TRANSPARENT YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    
    #base for generating a documentation pdf file
    #set(DOXYGEN_GENERATE_LATEX YES)
    #set(DOXYGEN_USE_PDFLATEX YES)
    #set(DOXYGEN_PDF_HYPERLINKS YES)

    doxygen_add_docs(doxygen 
        "${input}"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Generate Doxygen Documentation"
    )
endfunction()

