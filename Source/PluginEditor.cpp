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
    //low pass filter cut off frequency sliders + labels
    createCutoffFreqSlider(&lowPassFreq0Slider, "lowPassCutoff0", &lowPassFreq0Label, "Frequency 0:");
    createCutoffFreqSlider(&lowPassFreq1Slider, "lowPassCutoff1", &lowPassFreq1Label, "Frequency 1:");
    createCutoffFreqSlider(&lowPassFreq2Slider, "lowPassCutoff2", &lowPassFreq2Label, "Frequency 2:");
    createCutoffFreqSlider(&lowPassFreq3Slider, "lowPassCutoff3", &lowPassFreq3Label, "Frequency 3:");

    //low pass filter resonance sliders + labels
    createResonanceSlider(&lowPassQ0Slider, "lowPassQ0", &lowPassQ0Label, "Resonance 0:");
    createResonanceSlider(&lowPassQ1Slider, "lowPassQ1", &lowPassQ1Label, "Resonance 1:");
    createResonanceSlider(&lowPassQ2Slider, "lowPassQ2", &lowPassQ2Label, "Resonance 2:");
    createResonanceSlider(&lowPassQ3Slider, "lowPassQ3", &lowPassQ3Label, "Resonance 3:");
    
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

void DigitalDJAudioProcessorEditor::createCutoffFreqSlider (juce::Slider* slider, juce::String paramName, juce::Label* label, juce::String labelString)
{
    addAndMakeVisible(*slider);
    slider->setRange (20.f, 20000.f);
    slider->setTextValueSuffix (" Hz");
    slider->addListener (this);
    slider->setValue(audioProcessor.apvts.getRawParameterValue(paramName)->load());
    
    //create and attach a label to the slider
    createLabel(label, slider, labelString);
}

void DigitalDJAudioProcessorEditor::createResonanceSlider(juce::Slider* slider, juce::String paramName, juce::Label* label, juce::String labelString)
{
    addAndMakeVisible(*slider);
    slider->setRange (0.1f, 10.f);
    slider->setTextValueSuffix ("Hz/Hz");
    slider->addListener (this);
    slider->setValue(audioProcessor.apvts.getRawParameterValue(paramName)->load());
    
    //create and attach a label to the slider
    createLabel(label, slider, labelString);
}

void DigitalDJAudioProcessorEditor::createLabel(juce::Label* label, juce::Slider* slider, juce::String labelString)
{
    addAndMakeVisible(*label);
    label->setText(labelString, juce::dontSendNotification);
    label->attachToComponent(slider, true);
}
