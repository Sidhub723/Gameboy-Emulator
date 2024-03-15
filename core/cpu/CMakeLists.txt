add_library(GBEmuCPU 
    cpu.cpp
    cpu_rw.cpp
    cpu_utils.cpp
    ins_admodes.cpp
    ins.cpp
    ins_prefix.cpp
)

target_link_libraries(GBEmuCPU compile_options)
