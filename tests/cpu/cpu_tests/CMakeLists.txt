set(failRegex "[.]*Failure([.]*)")

create_test_sourcelist(CPUTests GBEmuCPU_Tests.cpp
    sample.cpp
    xora.cpp
)

add_executable(GBEmuCPU_Tests ${CPUTests})
target_link_libraries(GBEmuCPU_Tests PRIVATE GBEmuCPUTest test_config)

set(TestsToRun ${CPUTests})
remove(TestsToRun GBEmuCPU_Tests.cpp)

foreach(test ${TestsToRun})
    get_filename_component(TName ${test} NAME_WE)
    add_test(NAME ${TName} COMMAND GBEmuCPU_Tests ${TName} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_property(TEST ${TName} PROPERTY FAIL_REGULAR_EXPRESSION ${failRegex})
endforeach()
