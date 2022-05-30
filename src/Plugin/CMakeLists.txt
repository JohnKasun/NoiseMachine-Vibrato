juce_add_plugin(VibratoPlugin
    PLUGIN_MANUFACTURER_CODE John
    PLUGIN_CODE Vbro
    FORMATS VST3 Standalone
    PRODUCT_NAME "Vibrato Plugin")

target_sources(VibratoPlugin
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp)

target_include_directories(VibratoPlugin
    PUBLIC
        include)

target_compile_definitions(VibratoPlugin
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(VibratoPlugin
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)