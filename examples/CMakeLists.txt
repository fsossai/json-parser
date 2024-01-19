set(INCLUDE_DIR ../include)

add_executable(checker checker.cpp)
target_include_directories(checker PUBLIC ${INCLUDE_DIR})
target_link_libraries(checker PRIVATE json_parser)

add_executable(prettify prettify.cpp)
target_include_directories(prettify PUBLIC ${INCLUDE_DIR})
target_link_libraries(prettify PRIVATE json_parser)
