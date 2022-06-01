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

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    juce::AudioProcessorValueTreeState& mValueTreeState;

    AudioPluginAudioProcessor& processorRef;

    juce::Slider mFreqSlider;
    juce::Label mFreqLabel;
    std::unique_ptr<SliderAttachment> mFreqAttachment;

    juce::Slider mDepthSlider;
    juce::Label mDepthLabel;
    std::unique_ptr<SliderAttachment> mDepthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};