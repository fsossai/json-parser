cmake_minimum_required(VERSION 3.12)
project(json_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS "-O3 -g")

set(LIB_SOURCES
    src/ast.cpp
    src/scanner.cpp
    src/visitor.cpp
    src/string_visitor.cpp
)

set(LIB_HEADERS
    include/json_parser.h
    include/ast.h
    include/scanner.h
    include/visitor.h
    include/string_visitor.h
)

add_library(json_parser SHARED ${LIB_SOURCES})
target_include_directories(json_parser PUBLIC include)

add_subdirectory(examples)

get_filename_component(INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
configure_file(compile_params.in ${CMAKE_SOURCE_DIR}/compile_params @ONLY)

install(TARGETS json_parser DESTINATION lib)
install(FILES ${LIB_HEADERS} DESTINATION include)
