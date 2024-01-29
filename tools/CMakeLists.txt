set(INCLUDE_DIR ../include)

add_executable(jcheck Check.cpp)
target_include_directories(jcheck PUBLIC ${INCLUDE_DIR})
target_link_libraries(jcheck PRIVATE jparser)

add_executable(jmatch Match.cpp)
target_include_directories(jmatch PUBLIC ${INCLUDE_DIR})
target_link_libraries(jmatch PRIVATE jparser)

add_executable(jcolor Color.cpp)
target_include_directories(jcolor PUBLIC ${INCLUDE_DIR})
target_link_libraries(jcolor PRIVATE jparser)

add_executable(jpretty Pretty.cpp)
target_include_directories(jpretty PUBLIC ${INCLUDE_DIR})
target_link_libraries(jpretty PRIVATE jparser)

add_executable(jugly Ugly.cpp)
target_include_directories(jugly PUBLIC ${INCLUDE_DIR})
target_link_libraries(jugly PRIVATE jparser)

install(TARGETS jcheck DESTINATION bin)
install(TARGETS jmatch DESTINATION bin)
install(TARGETS jcolor DESTINATION bin)
install(TARGETS jpretty DESTINATION bin)
install(TARGETS jugly DESTINATION bin)

install(FILES color_palette.json DESTINATION bin)
install(FILES color_palette.default.json DESTINATION bin)
