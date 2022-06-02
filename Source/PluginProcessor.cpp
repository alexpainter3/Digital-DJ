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
    
    //Create parameters for lowpass filters and add to Parameter Layout that is passed to the apvts
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassCutoff0",
                                                            "lowPassCutoff0",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.5f),
                                                            20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassQ0",
                                                            "lowPassQ0",
                                                            juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 0.5f),
                                                            1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassCutoff1",
                                                            "lowPassCutoff1",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.5f),
                                                            20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassQ1",
                                                            "lowPassQ1",
                                                            juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 0.5f),
                                                            1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassCutoff2",
                                                            "lowPassCutoff2",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.5f),
                                                            20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassQ2",
                                                            "lowPassQ2",
                                                            juce::NormalisableRange<float>(0.1f, 10.f, 0.1f, 0.5f),
                                                            1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassCutoff3",
                                                            "lowPassCutoff3",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.5f),
                                                            20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat> ("lowPassQ3",
                                                            "lowPassQ3",
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
    
    //pass the spec to both Filter Chains
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    
    //TODO - refactor repeated code
    //initialize filter coefficients
    //first, create the coefficients using the initial apvts settings
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass0Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,
                                                         apvts.getRawParameterValue("lowPassCutoff0")->load(),
                                                         apvts.getRawParameterValue("lowPassQ0")->load());
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass1Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,
                                                         apvts.getRawParameterValue("lowPassCutoff1")->load(),
                                                         apvts.getRawParameterValue("lowPassQ1")->load());
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass2Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,
                                                         apvts.getRawParameterValue("lowPassCutoff2")->load(),
                                                         apvts.getRawParameterValue("lowPassQ2")->load());
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass3Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,
                                                         apvts.getRawParameterValue("lowPassCutoff3")->load(),
                                                         apvts.getRawParameterValue("lowPassQ3")->load());
    //then, pass these coefficients to each audio channel
    *leftChain.get<0>().coefficients = *lowPass0Coefficients;
    *rightChain.get<0>().coefficients = *lowPass0Coefficients;
    *leftChain.get<1>().coefficients = *lowPass1Coefficients;
    *rightChain.get<1>().coefficients = *lowPass1Coefficients;
    *leftChain.get<2>().coefficients = *lowPass2Coefficients;
    *rightChain.get<2>().coefficients = *lowPass2Coefficients;
    *leftChain.get<3>().coefficients = *lowPass3Coefficients;
    *rightChain.get<3>().coefficients = *lowPass3Coefficients;
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

    //update filter coefficients
    //first, create the coefficients using the current apvts settings
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass0Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                         apvts.getRawParameterValue("lowPassCutoff0")->load(),
                                                         apvts.getRawParameterValue("lowPassQ0")->load());
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass1Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                         apvts.getRawParameterValue("lowPassCutoff1")->load(),
                                                         apvts.getRawParameterValue("lowPassQ1")->load());
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass2Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                         apvts.getRawParameterValue("lowPassCutoff2")->load(),
                                                         apvts.getRawParameterValue("lowPassQ2")->load());
    juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass3Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                         apvts.getRawParameterValue("lowPassCutoff3")->load(),
                                                         apvts.getRawParameterValue("lowPassQ3")->load());
    //then, pass these coefficients to each audio channel
    *leftChain.get<0>().coefficients = *lowPass0Coefficients;
    *rightChain.get<0>().coefficients = *lowPass0Coefficients;
    *leftChain.get<1>().coefficients = *lowPass1Coefficients;
    *rightChain.get<1>().coefficients = *lowPass1Coefficients;
    *leftChain.get<2>().coefficients = *lowPass2Coefficients;
    *rightChain.get<2>().coefficients = *lowPass2Coefficients;
    *leftChain.get<3>().coefficients = *lowPass3Coefficients;
    *rightChain.get<3>().coefficients = *lowPass3Coefficients;
    
    //Creates an AudioBlock that points to the data in an AudioBuffer (in this case, the buffer arg)
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::AudioBlock<float> leftBlock = block.getSingleChannelBlock(0);
    juce::dsp::AudioBlock<float> rightBlock = block.getSingleChannelBlock(1);
    
    //wrap Audio Blocks with a Context that the chain will be able to use
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    
    //pass contexts to the chains
    leftChain.process(leftContext);
    rightChain.process(rightContext);
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    DBG("Writing current apvts state to memory");
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void DigitalDJAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        DBG("VALID TREE - Updating tree state");
        apvts.replaceState(tree);
        //update filter coefficients
        //first, create the coefficients using the current apvts settings
        juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass0Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                             apvts.getRawParameterValue("lowPassCutoff0")->load(),
                                                             apvts.getRawParameterValue("lowPassQ0")->load());
        juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass1Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                             apvts.getRawParameterValue("lowPassCutoff1")->load(),
                                                             apvts.getRawParameterValue("lowPassQ1")->load());
        juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass2Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                             apvts.getRawParameterValue("lowPassCutoff2")->load(),
                                                             apvts.getRawParameterValue("lowPassQ2")->load());
        juce::dsp::IIR::Filter<float>::CoefficientsPtr lowPass3Coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
                                                             apvts.getRawParameterValue("lowPassCutoff3")->load(),
                                                             apvts.getRawParameterValue("lowPassQ3")->load());
        //then, pass these coefficients to each audio channel
        *leftChain.get<0>().coefficients = *lowPass0Coefficients;
        *rightChain.get<0>().coefficients = *lowPass0Coefficients;
        *leftChain.get<1>().coefficients = *lowPass1Coefficients;
        *rightChain.get<1>().coefficients = *lowPass1Coefficients;
        *leftChain.get<2>().coefficients = *lowPass2Coefficients;
        *rightChain.get<2>().coefficients = *lowPass2Coefficients;
        *leftChain.get<3>().coefficients = *lowPass3Coefficients;
        *rightChain.get<3>().coefficients = *lowPass3Coefficients;
    }
    else
    {
        DBG("INVALID TREE - NOT Updating tree state");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DigitalDJAudioProcessor();
}
