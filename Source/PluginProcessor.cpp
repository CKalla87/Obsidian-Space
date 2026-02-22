/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
ObsidianSpaceAudioProcessor::ObsidianSpaceAudioProcessor()
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
    // Get parameter pointers
    roomSizeParam = apvts.getRawParameterValue ("ROOMSIZE");
    decayParam = apvts.getRawParameterValue ("DECAY");
    preDelayParam = apvts.getRawParameterValue ("PREDELAY");
    dampingParam = apvts.getRawParameterValue ("DAMPING");
    mixParam = apvts.getRawParameterValue ("MIX");
    widthParam = apvts.getRawParameterValue ("WIDTH");
    lowCutParam = apvts.getRawParameterValue ("LOWCUT");
    highCutParam = apvts.getRawParameterValue ("HIGHCUT");
    
    // Initialize reverb parameters with default values
    reverbParams.roomSize = 0.5f;
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.3f;
    reverbParams.dryLevel = 0.7f;
    reverbParams.width = 1.0f;
    reverbParams.freezeMode = false;
    reverb.setParameters (reverbParams);
}

ObsidianSpaceAudioProcessor::~ObsidianSpaceAudioProcessor()
{
}

//==============================================================================
const juce::String ObsidianSpaceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ObsidianSpaceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ObsidianSpaceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ObsidianSpaceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ObsidianSpaceAudioProcessor::getTailLengthSeconds() const
{
    return 10.0; // Long tail for reverb
}

int ObsidianSpaceAudioProcessor::getNumPrograms()
{
    return 1;
}

int ObsidianSpaceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ObsidianSpaceAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ObsidianSpaceAudioProcessor::getProgramName (int index)
{
    return {};
}

void ObsidianSpaceAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ObsidianSpaceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    
    reverb.prepare (spec);
    
    updateParameters();
}

void ObsidianSpaceAudioProcessor::releaseResources()
{
    reverb.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ObsidianSpaceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ObsidianSpaceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateParameters();

    // Process audio
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    
    // Process reverb
    reverb.process (context);
}

//==============================================================================
bool ObsidianSpaceAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ObsidianSpaceAudioProcessor::createEditor()
{
    return new ObsidianSpaceAudioProcessorEditor (*this);
}

//==============================================================================
void ObsidianSpaceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ObsidianSpaceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
void ObsidianSpaceAudioProcessor::updateParameters()
{
    const float tolerance = 0.001f;
    
    float roomSize = roomSizeParam->load();
    float decay = decayParam->load();
    float preDelay = preDelayParam->load();
    float damping = dampingParam->load();
    float mix = mixParam->load();
    float width = widthParam->load();
    float lowCut = lowCutParam->load();
    float highCut = highCutParam->load();
    
    bool needsUpdate = false;
    
    if (std::abs (roomSize - cachedRoomSize) > tolerance)
    {
        reverbParams.roomSize = juce::jlimit (0.0f, 1.0f, roomSize / 100.0f);
        cachedRoomSize = roomSize;
        needsUpdate = true;
    }
    
    if (std::abs (decay - cachedDecay) > tolerance)
    {
        cachedDecay = decay;
    }

    if (std::abs (preDelay - cachedPreDelay) > tolerance)
    {
        cachedPreDelay = preDelay;
    }

    if (std::abs (damping - cachedDamping) > tolerance)
    {
        auto dampingNorm = (std::log (damping) - std::log (1000.0f))
                           / (std::log (20000.0f) - std::log (1000.0f));
        reverbParams.damping = juce::jlimit (0.0f, 1.0f, dampingNorm);
        cachedDamping = damping;
        needsUpdate = true;
    }
    
    if (std::abs (mix - cachedMix) > tolerance)
    {
        auto wet = juce::jlimit (0.0f, 1.0f, mix / 100.0f);
        reverbParams.wetLevel = wet;
        reverbParams.dryLevel = 1.0f - wet;
        cachedMix = mix;
        needsUpdate = true;
    }
    
    if (std::abs (width - cachedWidth) > tolerance)
    {
        reverbParams.width = juce::jlimit (0.0f, 1.0f, width / 200.0f);
        cachedWidth = width;
        needsUpdate = true;
    }
    
    if (std::abs (lowCut - cachedLowCut) > tolerance)
    {
        cachedLowCut = lowCut;
    }

    if (std::abs (highCut - cachedHighCut) > tolerance)
    {
        cachedHighCut = highCut;
    }
    
    if (needsUpdate)
    {
        reverb.setParameters (reverbParams);
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout ObsidianSpaceAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Room Size: 0 to 100, default 50
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("ROOMSIZE", 1), "Room Size",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        50.0f
    ));

    // Decay: 0.1 to 10.0 seconds, default 2.5
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("DECAY", 1), "Decay",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.01f),
        2.5f
    ));

    // Pre-Delay: 0 to 200 ms, default 20
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("PREDELAY", 1), "Pre-Delay",
        juce::NormalisableRange<float> (0.0f, 200.0f, 1.0f),
        20.0f
    ));

    // Damping: 1000 to 20000 Hz, default 8000
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("DAMPING", 1), "Damping",
        juce::NormalisableRange<float> (1000.0f, 20000.0f, 1.0f),
        8000.0f
    ));

    // Mix: 0 to 100, default 30
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("MIX", 1), "Mix",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        30.0f
    ));

    // Width: 0 to 200, default 100
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("WIDTH", 1), "Width",
        juce::NormalisableRange<float> (0.0f, 200.0f, 0.1f),
        100.0f
    ));

    // Low Cut: 20 to 500 Hz, default 20
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("LOWCUT", 1), "Low Cut",
        juce::NormalisableRange<float> (20.0f, 500.0f, 1.0f),
        20.0f
    ));

    // High Cut: 2000 to 20000 Hz, default 12000
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("HIGHCUT", 1), "High Cut",
        juce::NormalisableRange<float> (2000.0f, 20000.0f, 1.0f),
        12000.0f
    ));

    return { params.begin(), params.end() };
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ObsidianSpaceAudioProcessor();
}

