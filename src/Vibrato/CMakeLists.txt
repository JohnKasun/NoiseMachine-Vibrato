add_library(Vibrato OBJECT include/vibrato.h vibrato.cpp)

target_include_directories(Vibrato
    PUBLIC
        ./include
)

target_link_libraries(Vibrato
        Lfo
        Utility
)