add_library(GBEmuCPU 
    cpu.cpp
    cpu_rw.cpp
    cpu_utils.cpp
    ins_admodes.cpp
    ins.cpp
    ins_prefix.cpp
)

add_library(GBEmuCPUNoRW
    cpu.cpp
    cpu_utils.cpp
    ins_admodes.cpp
    ins.cpp
    ins_prefix.cpp
)

target_link_libraries(GBEmuCPU compile_options)
target_link_libraries(GBEmuCPUNoRW PRIVATE compile_options)
