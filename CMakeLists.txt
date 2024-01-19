cmake_minimum_required(VERSION 3.12)
project(json_parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCES
    src/ast.cpp
    src/scanner.cpp
    src/visitor.cpp
)

set(LIB_HEADERS
    inc/json_parser.h
    inc/ast.h
    inc/scanner.h
    inc/visitor.h
)

add_library(json_parser SHARED ${LIB_SOURCES})
target_include_directories(json_parser PUBLIC include)

add_subdirectory(examples)

install(TARGETS json_parser DESTINATION lib)
install(FILES ${LIB_HEADERS} DESTINATION include)
