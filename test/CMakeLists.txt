add_executable(Tests main.cpp)
target_include_directories(Tests PUBLIC include)
target_sources(Tests 
    PRIVATE
        testVibrato.cpp
        testLfo.cpp
)
target_link_libraries(Tests
    PUBLIC
        Lfo
        Vibrato
)