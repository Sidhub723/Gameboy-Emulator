add_library(GBEmuCPU 
    cpu.cpp
    cpu_init.cpp
    cpu_rw.cpp
    ins_admodes.cpp
    ins.cpp
    ins_prefix.cpp
    interrupt.cpp
)

add_library(GBEmuCPUNoRW
    cpu.cpp
    cpu_init.cpp
    ins_admodes.cpp
    ins.cpp
    ins_prefix.cpp
    interrupt.cpp
)

target_link_libraries(GBEmuCPU compile_options)
target_link_libraries(GBEmuCPUNoRW PRIVATE compile_options)
