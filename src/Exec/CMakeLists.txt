add_executable(Exec main.cpp)

target_link_libraries(Exec
        Vibrato
        Utility
        Lfo
        AudioFileIO
)