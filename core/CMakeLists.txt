add_library(GBEmuCore INTERFACE)

add_subdirectory(cpu)
add_subdirectory(ppu)
add_subdirectory(memory)
add_subdirectory(boot)
add_subdirectory(interconnect)

target_link_libraries(GBEmuCore INTERFACE
    GBEmuCPU
    GBEmuPPU
    GBEmuMemory
    GBEmuBoot
    GBEmuInterconnect
)