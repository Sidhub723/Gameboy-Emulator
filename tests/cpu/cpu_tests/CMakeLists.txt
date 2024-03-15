set(failRegex "[.]*Failure([.]*)")

add_executable(cpu_sanity_check template_test.cpp)
target_link_libraries(cpu_sanity_check PRIVATE GBEmuCPUTest test_config)
add_test(NAME cpu_sanity_check COMMAND cpu_sanity_check)
set_property(TEST cpu_sanity_check PROPERTY FAIL_REGULAR_EXPRESSION "${failRegex}")