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
    lowPassQ0Attachment(audioProcessor.apvts, "lowPassQ0", lowPassQ0Slider),
    lowPassFreq1Attachment(audioProcessor.apvts, "lowPassCutoff1", lowPassFreq1Slider),
    lowPassQ1Attachment(audioProcessor.apvts, "lowPassQ1", lowPassQ1Slider),
    lowPassFreq2Attachment(audioProcessor.apvts, "lowPassCutoff2", lowPassFreq2Slider),
    lowPassQ2Attachment(audioProcessor.apvts, "lowPassQ2", lowPassQ2Slider),
    lowPassFreq3Attachment(audioProcessor.apvts, "lowPassCutoff3", lowPassFreq3Slider),
    lowPassQ3Attachment(audioProcessor.apvts, "lowPassQ3", lowPassQ3Slider)
{
    //instantiation of sliders and corresponding labels
    //low pass filter cut off frequency
    //addAndMakeVisible(lowPassFreq0Slider);
    createCutoffFreqSlider(&lowPassFreq0Slider, "lowPassCutoff0");
    //addAndMakeVisible(lowPassFreq1Slider);
    createCutoffFreqSlider(&lowPassFreq1Slider, "lowPassCutoff1");
    //addAndMakeVisible(lowPassFreq2Slider);
    createCutoffFreqSlider(&lowPassFreq2Slider, "lowPassCutoff2");
    //addAndMakeVisible(lowPassFreq3Slider);
    createCutoffFreqSlider(&lowPassFreq3Slider, "lowPassCutoff3");
    
    /*addAndMakeVisible(lowPassFreq0Slider);
    lowPassFreq0Slider.setRange (20.f, 20000.f);
    lowPassFreq0Slider.setTextValueSuffix (" Hz");
    lowPassFreq0Slider.addListener (this);
    lowPassFreq0Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassCutoff0")->load());
    
    addAndMakeVisible(lowPassFreq1Slider);
    lowPassFreq1Slider.setRange (20.f, 20000.f);
    lowPassFreq1Slider.setTextValueSuffix (" Hz");
    lowPassFreq1Slider.addListener (this);
    lowPassFreq1Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassCutoff1")->load());
    
    addAndMakeVisible(lowPassFreq2Slider);
    lowPassFreq2Slider.setRange (20.f, 20000.f);
    lowPassFreq2Slider.setTextValueSuffix (" Hz");
    lowPassFreq2Slider.addListener (this);
    lowPassFreq2Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassCutoff2")->load());
    
    addAndMakeVisible(lowPassFreq3Slider);
    lowPassFreq3Slider.setRange (20.f, 20000.f);
    lowPassFreq3Slider.setTextValueSuffix (" Hz");
    lowPassFreq3Slider.addListener (this);
    lowPassFreq3Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassCutoff3")->load());*/
    
    //cut off frequency label
    addAndMakeVisible(lowPassFreq0Label);
    lowPassFreq0Label.setText("Frequency", juce::dontSendNotification);
    lowPassFreq0Label.attachToComponent(&lowPassFreq0Slider, true);
    
    addAndMakeVisible(lowPassFreq1Label);
    lowPassFreq1Label.setText("Frequency", juce::dontSendNotification);
    lowPassFreq1Label.attachToComponent(&lowPassFreq1Slider, true);
    
    addAndMakeVisible(lowPassFreq2Label);
    lowPassFreq2Label.setText("Frequency", juce::dontSendNotification);
    lowPassFreq2Label.attachToComponent(&lowPassFreq2Slider, true);
    
    addAndMakeVisible(lowPassFreq3Label);
    lowPassFreq3Label.setText("Frequency", juce::dontSendNotification);
    lowPassFreq3Label.attachToComponent(&lowPassFreq3Slider, true);
    
    //low pass filter resonance
    addAndMakeVisible(lowPassQ0Slider);
    lowPassQ0Slider.setRange (0.1f, 10.f);
    lowPassQ0Slider.setTextValueSuffix ("Hz/Hz");
    lowPassQ0Slider.addListener (this);
    lowPassQ0Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassQ0")->load());
    
    addAndMakeVisible(lowPassQ1Slider);
    lowPassQ1Slider.setRange (0.1f, 10.f);
    lowPassQ1Slider.setTextValueSuffix ("Hz/Hz");
    lowPassQ1Slider.addListener (this);
    lowPassQ1Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassQ1")->load());
    
    addAndMakeVisible(lowPassQ2Slider);
    lowPassQ2Slider.setRange (0.1f, 10.f);
    lowPassQ2Slider.setTextValueSuffix ("Hz/Hz");
    lowPassQ2Slider.addListener (this);
    lowPassQ2Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassQ2")->load());
    
    addAndMakeVisible(lowPassQ3Slider);
    lowPassQ3Slider.setRange (0.1f, 10.f);
    lowPassQ3Slider.setTextValueSuffix ("Hz/Hz");
    lowPassQ3Slider.addListener (this);
    lowPassQ3Slider.setValue(audioProcessor.apvts.getRawParameterValue("lowPassQ3")->load());
    
    //resonance label
    addAndMakeVisible(lowPassQ0Label);
    lowPassQ0Label.setText("Resonance", juce::dontSendNotification);
    lowPassQ0Label.attachToComponent(&lowPassQ0Slider, true);
    
    addAndMakeVisible(lowPassQ1Label);
    lowPassQ1Label.setText("Resonance", juce::dontSendNotification);
    lowPassQ1Label.attachToComponent(&lowPassQ1Slider, true);
    
    addAndMakeVisible(lowPassQ2Label);
    lowPassQ2Label.setText("Resonance", juce::dontSendNotification);
    lowPassQ2Label.attachToComponent(&lowPassQ2Slider, true);
    
    addAndMakeVisible(lowPassQ3Label);
    lowPassQ3Label.setText("Resonance", juce::dontSendNotification);
    lowPassQ3Label.attachToComponent(&lowPassQ3Slider, true);
    
    //set the size of the GUI (width, height)
    setSize (600, 500);
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
    //position slider elements (x coordinate, y coordinate, width, height)
    //coordiante (0,0) is the upper left corner
    lowPassFreq0Slider.setBounds(100, 75, 300, 50);
    lowPassQ0Slider.setBounds(100, 125, 300, 50);
    lowPassFreq1Slider.setBounds(100, 175, 300, 50);
    lowPassQ1Slider.setBounds(100, 225, 300, 50);
    lowPassFreq2Slider.setBounds(100, 275, 300, 50);
    lowPassQ2Slider.setBounds(100, 325, 300, 50);
    lowPassFreq3Slider.setBounds(100, 375, 300, 50);
    lowPassQ3Slider.setBounds(100, 425, 300, 50);
}

//must be implemented as we have the Slider Listener as an interface
void DigitalDJAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &lowPassFreq0Slider)
    {
        DBG("LowPass0 Freq updated to " << audioProcessor.apvts.getRawParameterValue("lowPassCutoff0")->load() << "Hz");
    }
    else if (slider == &lowPassQ0Slider)
    {
        DBG("LowPass0 Q updated to " << audioProcessor.apvts.getRawParameterValue("lowPassQ0")->load());
    }
    else if (slider == &lowPassFreq1Slider)
    {
        DBG("LowPass1 Freq updated to " << audioProcessor.apvts.getRawParameterValue("lowPassCutoff1")->load() << "Hz");
    }
    else if (slider == &lowPassQ1Slider)
    {
        DBG("LowPass1 Q updated to " << audioProcessor.apvts.getRawParameterValue("lowPassQ1")->load());
    }
    else if (slider == &lowPassFreq2Slider)
    {
        DBG("LowPass2 Freq updated to " << audioProcessor.apvts.getRawParameterValue("lowPassCutoff2")->load() << "Hz");
    }
    else if (slider == &lowPassQ2Slider)
    {
        DBG("LowPass2 Q updated to " << audioProcessor.apvts.getRawParameterValue("lowPassQ2")->load());
    }
    else if (slider == &lowPassFreq3Slider)
    {
        DBG("LowPass3 Freq updated to " << audioProcessor.apvts.getRawParameterValue("lowPassCutoff3")->load() << "Hz");
    }
    else if (slider == &lowPassQ3Slider)
    {
        DBG("LowPass3 Q updated to " << audioProcessor.apvts.getRawParameterValue("lowPassQ3")->load());
    }
}

void DigitalDJAudioProcessorEditor::createCutoffFreqSlider (juce::Slider* slider, juce::String paramName)
{
    addAndMakeVisible(*slider);
    slider->setRange (20.f, 20000.f);
    slider->setTextValueSuffix (" Hz");
    slider->addListener (this);
    slider->setValue(audioProcessor.apvts.getRawParameterValue(paramName)->load());
}
