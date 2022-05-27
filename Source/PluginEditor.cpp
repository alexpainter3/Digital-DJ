/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DigitalDJAudioProcessorEditor::DigitalDJAudioProcessorEditor (DigitalDJAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    lowPassFreq0Attachment(audioProcessor.apvts, "lowPassCutoff0", lowPassFreq0Slider),
    lowPassQ0Attachment(audioProcessor.apvts, "lowPassQ0", lowPassQ0Slider)
{
    //instantiation of sliders and corresponding labels
    //low pass filter cut off frequency
    addAndMakeVisible(lowPassFreq0Slider);
    lowPassFreq0Slider.setRange (20.f, 20000.f);
    lowPassFreq0Slider.setTextValueSuffix (" Hz");
    lowPassFreq0Slider.addListener (this);
    lowPassFreq0Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassCutoff0")->load());
    
    addAndMakeVisible(lowPassFreq0Label);
    lowPassFreq0Label.setText("Frequency", juce::dontSendNotification);
    lowPassFreq0Label.attachToComponent(&lowPassFreq0Slider, false);
    
    //low pass filter resonance
    addAndMakeVisible(lowPassQ0Slider);
    lowPassQ0Slider.setRange (0.1f, 10.f);
    lowPassQ0Slider.setTextValueSuffix ("Hz/Hz");
    lowPassQ0Slider.addListener (this);
    lowPassQ0Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassQ0")->load());
    
    addAndMakeVisible(lowPassQ0Label);
    lowPassQ0Label.setText("Resonance", juce::dontSendNotification);
    lowPassQ0Label.attachToComponent(&lowPassFreq0Label, false);
    
    //set the size of the GUI
    setSize (800, 600);
}

DigitalDJAudioProcessorEditor::~DigitalDJAudioProcessorEditor()
{
}

//==============================================================================
void DigitalDJAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DigitalDJAudioProcessorEditor::resized()
{
    //position slider elements
    lowPassFreq0Slider.setBounds(50, 100, 300, 300);
    lowPassQ0Slider.setBounds(450, 100, 300, 300);
}

//must be implemented as we have the Slider Listener as an interface
void DigitalDJAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &lowPassFreq0Slider)
    {
        DBG("Low Pass Freq updated to " << audioProcessor.apvts.getRawParameterValue("lowPassCutoff0")->load() << "Hz");
    }
    else if (slider == &lowPassQ0Slider)
    {
        DBG("Low Pass Q updated to " << audioProcessor.apvts.getRawParameterValue("lowPassQ0")->load());
    }
}
