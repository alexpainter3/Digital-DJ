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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DigitalDJAudioProcessor& audioProcessor;
    
    // Add handles for GUI elements here
    juce::Slider lowPassFreq0Slider;
    juce::Label  lowPassFreq0Label;
    juce::Slider lowPassQ0Slider;
    juce::Label  lowPassQ0Label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DigitalDJAudioProcessorEditor)
};
