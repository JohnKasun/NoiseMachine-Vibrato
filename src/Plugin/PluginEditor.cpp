#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processorRef(p), mValueTreeState(vts)
{
    juce::ignoreUnused(processorRef);

    addAndMakeVisible(mFreqSlider);
    mFreqAttachment.reset(new SliderAttachment(mValueTreeState, "freq", mFreqSlider));
    mFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, paramControlWidth, paramLabelHeight);
    addAndMakeVisible(mFreqLabel);
    mFreqLabel.setText("Speed", juce::dontSendNotification);
    mFreqLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(mDepthSlider);
    mDepthAttachment.reset(new SliderAttachment(mValueTreeState, "depth", mDepthSlider));
    mDepthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, paramControlWidth, paramLabelHeight);
    addAndMakeVisible(mDepthLabel);
    mDepthLabel.setText("Depth", juce::dontSendNotification);
    mDepthLabel.setJustificationType(juce::Justification::centred);

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

    mFreqSlider.setBounds(freqArea.removeFromTop(paramControlHeight));
    mFreqLabel.setBounds(freqArea);

    mDepthSlider.setBounds(area.removeFromTop(paramControlHeight));
    mDepthLabel.setBounds(area);
}