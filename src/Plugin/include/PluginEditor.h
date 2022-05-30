#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:

    enum Dimen_t {
        paramControlWidth = 250,
        paramControlHeight = 250,
        paramLabelHeight = 50
    };

    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    AudioPluginAudioProcessor& processorRef;

    juce::Slider mFreqSlider;
    juce::Slider mDepthSlider;
    juce::Label mFreqLabel;
    juce::Label mDepthLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};