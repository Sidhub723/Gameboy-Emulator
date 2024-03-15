add_library(GBEmuInterconnect gb.cpp)

target_link_libraries(GBEmuInterconnect PRIVATE
    GBEmuCPU
    GBEmuMemory
    GBEmuBoot
)