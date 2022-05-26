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
                       ), apvts(*this, nullptr, "Parameters", createParameterLayout())//,
                          //lowPassFilter0(juce::dsp::IIR::Coefficients<float>::makeLowPass(11400, 20.f, 1.f))
#endif
{
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

juce::AudioProcessorValueTreeState::ParameterLayout DigitalDJAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassCutoff0",
                                                            "lowPassCutoff0",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.5f),
                                                            20.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassQ0",
                                                            "lowPassQ0",
                                                            juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 0.5f),
                                                            1.f));
    
    return layout;
}


//==============================================================================
void DigitalDJAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;
    spec.maximumBlockSize = samplesPerBlock;
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

    //Creates an AudioBlock that points to the data in an AudioBuffer (in this case, the buffer arg)
    juce::dsp::AudioBlock<float> block (buffer);
    //lowPassFilter0.process(juce::dsp::ProcessContextReplacing<float>(block));
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
    //TODO
}

void DigitalDJAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //TODO
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DigitalDJAudioProcessor();
}
