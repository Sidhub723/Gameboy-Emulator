add_library(GBEmuCore INTERFACE)

add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(boot)
add_subdirectory(interconnect)

target_link_libraries(GBEmuCore INTERFACE
    GBEmuCPU
    GBEmuMemory
    GBEmuBoot
    GBEmuInterconnect
)