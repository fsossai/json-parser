cmake_minimum_required(VERSION 3.12)
project(jparser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS "-O3 -g")

set(LIB_VERSION 0.6.0)

set(JPARSER_SOURCES
  src/AST.cpp
  src/Matcher.cpp
  src/Scanner.cpp
  src/StringVisitor.cpp
  src/Visitor.cpp
)

add_library(jparser STATIC ${JPARSER_SOURCES})
target_include_directories(jparser PUBLIC include)

get_filename_component(INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
configure_file(enable.in ${CMAKE_SOURCE_DIR}/enable @ONLY)
configure_file(jparser.pc.in ${CMAKE_BINARY_DIR}/jparser.pc @ONLY)

install(TARGETS jparser DESTINATION lib)
install(DIRECTORY include DESTINATION .)
install(FILES ${CMAKE_BINARY_DIR}/jparser.pc DESTINATION lib/pkgconfig)

add_subdirectory(tools)
