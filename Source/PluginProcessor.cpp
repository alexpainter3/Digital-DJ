/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DigitalDJAudioProcessor::DigitalDJAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(lowPassCutoff0 = new juce::AudioParameterFloat("lowPassCutoff0",
                                                                "lowPassCutoff0",
                                                                juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.5f),
                                                                20.f));
    
    addParameter(lowPassQ0 = new juce::AudioParameterFloat("lowPassQ0",
                                                           "lowPassQ0",
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 1.f, 0.5f),
                                                           1.f));
}

DigitalDJAudioProcessor::~DigitalDJAudioProcessor()
{
}

//==============================================================================
const juce::String DigitalDJAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DigitalDJAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DigitalDJAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DigitalDJAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DigitalDJAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DigitalDJAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DigitalDJAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DigitalDJAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DigitalDJAudioProcessor::getProgramName (int index)
{
    return {};
}

void DigitalDJAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DigitalDJAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //create Spec here
}

void DigitalDJAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DigitalDJAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DigitalDJAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //create buffer here
}

//==============================================================================
bool DigitalDJAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DigitalDJAudioProcessor::createEditor()
{
    return new DigitalDJAudioProcessorEditor (*this);
}

//==============================================================================
void DigitalDJAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //store audio parameter values
    juce::MemoryOutputStream (destData, true).writeFloat (*lowPassCutoff0);
    juce::MemoryOutputStream (destData, true).writeFloat (*lowPassQ0);
}

void DigitalDJAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    *lowPassCutoff0 = juce::MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
    *lowPassQ0 = juce::MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DigitalDJAudioProcessor();
}

//call to update audio parameters
void DigitalDJAudioProcessor::setLPF0(float _freq, float _q)
{
    *lowPassCutoff0 = _freq;
    *lowPassQ0 = _q;
    DBG("Got updated LPF0 values: freq " << lowPassCutoff0->get() << " Q " << lowPassQ0->get());
}

