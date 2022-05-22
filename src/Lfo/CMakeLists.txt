add_library(Lfo OBJECT include/Lfo.h Lfo.cpp)

target_include_directories(Lfo
    PUBLIC
        ./include
)

target_link_libraries(Lfo
        RingBuffer
        Utility
)