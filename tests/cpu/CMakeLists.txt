add_library(test_config INTERFACE)
target_include_directories(test_config INTERFACE ${CMAKE_SOURCE_DIR}/tests/cpu)

add_library(GBEmuCPUTest cpu_test_rw.cpp)
target_link_libraries(GBEmuCPUTest PRIVATE GBEmuCPUNoRW PUBLIC compile_options)

add_subdirectory(cpu_tests)