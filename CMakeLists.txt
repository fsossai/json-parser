cmake_minimum_required(VERSION 3.12)
project(jsonparser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS "-O3 -g")

set(LIB_SOURCES
  src/AST.cpp
  src/Matcher.cpp
  src/Scanner.cpp
  src/StringVisitor.cpp
  src/Visitor.cpp
)

set(LIB_HEADERS
  include/AST.h
  include/JSONParser.h
  include/Matcher.h
  include/Scanner.h
  include/StringVisitor.h
  include/Visitor.h
)

add_library(jsonparser STATIC ${LIB_SOURCES})
target_include_directories(jsonparser PUBLIC include)

get_filename_component(INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
configure_file(enable.in ${CMAKE_SOURCE_DIR}/enable @ONLY)

install(TARGETS jsonparser DESTINATION lib)
install(FILES ${LIB_HEADERS} DESTINATION include)

add_subdirectory(tools)
