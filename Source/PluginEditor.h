/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DigitalDJAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::Slider::Listener
{
public:
    DigitalDJAudioProcessorEditor (DigitalDJAudioProcessor&);
    ~DigitalDJAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    void createCutoffFreqSlider(juce::Slider* slider, juce::String paramName, juce::Label* label, juce::String labelString);
    void createResonanceSlider(juce::Slider* slider, juce::String paramName, juce::Label* label, juce::String labelString);
    void createLabel(juce::Label* label, juce::Slider* slider, juce::String labelString);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DigitalDJAudioProcessor& audioProcessor;
    
    // Add handles for GUI elements here
    juce::Slider lowPassFreq0Slider;
    juce::Label  lowPassFreq0Label;
    juce::Slider lowPassQ0Slider;
    juce::Label  lowPassQ0Label;
    juce::Slider lowPassFreq1Slider;
    juce::Label  lowPassFreq1Label;
    juce::Slider lowPassQ1Slider;
    juce::Label  lowPassQ1Label;
    juce::Slider lowPassFreq2Slider;
    juce::Label  lowPassFreq2Label;
    juce::Slider lowPassQ2Slider;
    juce::Label  lowPassQ2Label;
    juce::Slider lowPassFreq3Slider;
    juce::Label  lowPassFreq3Label;
    juce::Slider lowPassQ3Slider;
    juce::Label  lowPassQ3Label;
    
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassFreq0Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassQ0Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassFreq1Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassQ1Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassFreq2Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassQ2Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassFreq3Attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lowPassQ3Attachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DigitalDJAudioProcessorEditor)
};
