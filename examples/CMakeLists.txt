set(INCLUDE_DIR ../include)

add_executable(checker Checker.cpp)
target_include_directories(checker PUBLIC ${INCLUDE_DIR})
target_link_libraries(checker PRIVATE jsonparser)

add_executable(prettify Prettify.cpp)
target_include_directories(prettify PUBLIC ${INCLUDE_DIR})
target_link_libraries(prettify PRIVATE jsonparser)
