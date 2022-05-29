cmake_minimum_required(VERSION 3.0)
Project(JohnK VERSION 0.0.1)

add_subdirectory(JUCE)
juce_add_plugin(VibratoPlugin
    PLUGIN_MANUFACTURER_CODE John
    PLUGIN_CODE Vbro
    FORMATS VST3 Standalone
    PRODUCT_NAME "Vibrato Plugin")

target_link_libraries(VibratoPlugin
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

add_subdirectory(src bin)
add_subdirectory(cmake)
add_subdirectory(test)
add_subdirectory(extern)

