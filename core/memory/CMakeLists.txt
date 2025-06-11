add_library(GBEmuMemory vram.cpp io.cpp rom.cpp hram.cpp)

target_link_libraries(GBEmuMemory compile_options)