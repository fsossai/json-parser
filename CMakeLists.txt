cmake_minimum_required(VERSION 3.12)
project(jsonparser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS "-O3 -g")

set(LIB_SOURCES
  src/AST.cpp
  src/Scanner.cpp
  src/Visitor.cpp
  src/StringVisitor.cpp
)

set(LIB_HEADERS
  include/JSONParser.h
  include/AST.h
  include/Scanner.h
  include/Visitor.h
  include/StringVisitor.h
)

add_library(jsonparser SHARED ${LIB_SOURCES})
target_include_directories(jsonparser PUBLIC include)

add_subdirectory(examples)

get_filename_component(INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
configure_file(compile_params.in ${CMAKE_SOURCE_DIR}/compile_params @ONLY)

install(TARGETS jsonparser DESTINATION lib)
install(FILES ${LIB_HEADERS} DESTINATION include)
