
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
    set(DOXYGEN_PROJECT_LOGO "${PROJECT_SOURCE_DIR}/res/logo.png")
    #set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/Readme.md")
    
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

# Create dependency graph
macro(generate_dep_graph)
    # Check if the 'dot' command is available
    find_program(DOT_EXECUTABLE dot)
    if(NOT DOT_EXECUTABLE)
        message(FATAL_ERROR "Graphviz 'dot' command not found. Make sure Graphviz is installed.")
    endif()

    set(DOT_FILE "dep_graph.dot")
    set(PNG_FILE "${CMAKE_SOURCE_DIR}/docs/dep_graph.png")

    add_custom_command(
        OUTPUT
            "${DOT_FILE}"
        COMMAND
            ${CMAKE_COMMAND} --graphviz="${DOT_FILE}" "."
        WORKING_DIRECTORY
            ${CMAKE_BINARY_DIR}
        DEPENDS
            ${PROJECT_NAME}
        COMMENT
            "Creates a new dependency graph"
    )

    add_custom_command(
        OUTPUT
            "${PNG_FILE}"
        COMMAND
            ${DOT_EXECUTABLE} -Tpng -o${PNG_FILE} ${DOT_FILE}
        WORKING_DIRECTORY
            ${CMAKE_BINARY_DIR}
        DEPENDS
            ${DOT_FILE}
        COMMENT
            "Creates a new dependency graph as png"
    )

    add_custom_target(dep_graph
        WORKING_DIRECTORY
            ${CMAKE_BINARY_DIR}
        DEPENDS
            ${PNG_FILE}
        COMMENT
            "Creates a new dependency graph for the documentation"
        VERBATIM
    )
endmacro()

macro(copy_compile_commands)
    add_custom_command(
        OUTPUT
            "${CMAKE_SOURCE_DIR}/compile_commands.json"
        COMMAND
            ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/compile_commands.json" "${CMAKE_SOURCE_DIR}/compile_commands.json"
        WORKING_DIRECTORY
            ${CMAKE_BINARY_DIR}
        DEPENDS
            ${PROJECT_NAME}
        COMMENT
            "Copy the compile_commands.json into the source dir for the IDE"
    )
endmacro()

# link system libraries to target
function(target_link_libraries_system target)
  set(libs ${ARGN})
  foreach(lib ${libs})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${target} SYSTEM PUBLIC ${lib_include_dirs})
    target_link_libraries(${target} PUBLIC ${lib})
  endforeach(lib)
endfunction(target_link_libraries_system)
