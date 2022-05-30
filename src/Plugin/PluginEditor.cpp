#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);


    addAndMakeVisible(mFreqSlider);
    mFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mFreqSlider.setRange(0, 10);
    mFreqSlider.setValue(0, juce::dontSendNotification);
    mFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    addAndMakeVisible(mDepthSlider);
    mDepthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mDepthSlider.setRange(0, 0.5);
    mDepthSlider.setValue(0, juce::dontSendNotification);
    mDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    setSize(paramControlWidth * 2, paramControlHeight + paramLabelHeight);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto freqArea = area.removeFromLeft(paramControlWidth);
    mFreqSlider.setBounds(freqArea);
    mDepthSlider.setBounds(area);
}