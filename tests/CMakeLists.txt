enable_testing()

file(GLOB PASS_CASES pass/*.json)
file(GLOB FAIL_CASES fail/*.json)

foreach(case ${PASS_CASES})
  get_filename_component(name ${case} NAME)
  add_test(NAME ${name} COMMAND jcheck ${case})
endforeach()

foreach(case ${FAIL_CASES})
  get_filename_component(name ${case} NAME)
  add_test(NAME ${name} COMMAND jcheck ${case})
  set_tests_properties(${name} PROPERTIES EXPECT_FAIL 1 WILL_FAIL 1)
endforeach()

