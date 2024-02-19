cmake_minimum_required(VERSION 3.12)
project(jparser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS "-O3 -g")

add_library(jparser STATIC
  src/AST.cpp
  src/Matcher.cpp
  src/Scanner.cpp
  src/StringVisitor.cpp
)
target_include_directories(jparser PUBLIC include)

get_filename_component(INSTALL_DIR_ABSOLUTE ${CMAKE_INSTALL_PREFIX} ABSOLUTE)
configure_file(enable.in ${CMAKE_SOURCE_DIR}/enable @ONLY)
configure_file(jparser.pc.in ${CMAKE_BINARY_DIR}/jparser.pc @ONLY)

install(TARGETS jparser DESTINATION lib)
install(DIRECTORY include DESTINATION .)
install(FILES ${CMAKE_BINARY_DIR}/jparser.pc DESTINATION lib/pkgconfig)

add_custom_target(
  CompileCommands ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
)

enable_testing()

add_subdirectory(tools)
add_subdirectory(tests)
