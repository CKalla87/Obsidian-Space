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
    roomSizeParam = apvts.getRawParameterValue("ROOMSIZE");
    dampingParam = apvts.getRawParameterValue("DAMPING");
    wetLevelParam = apvts.getRawParameterValue("WET");
    dryLevelParam = apvts.getRawParameterValue("DRY");
    widthParam = apvts.getRawParameterValue("WIDTH");
    freezeParam = apvts.getRawParameterValue("FREEZE");
    
    // Initialize reverb parameters with default values
    reverbParams.roomSize = 0.5f;
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.33f;
    reverbParams.dryLevel = 0.4f;
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
    float damping = dampingParam->load();
    float wetLevel = wetLevelParam->load();
    float dryLevel = dryLevelParam->load();
    float width = widthParam->load();
    float freeze = freezeParam->load();
    
    bool needsUpdate = false;
    
    if (std::abs (roomSize - cachedRoomSize) > tolerance)
    {
        reverbParams.roomSize = roomSize;
        cachedRoomSize = roomSize;
        needsUpdate = true;
    }
    
    if (std::abs (damping - cachedDamping) > tolerance)
    {
        reverbParams.damping = damping;
        cachedDamping = damping;
        needsUpdate = true;
    }
    
    if (std::abs (wetLevel - cachedWetLevel) > tolerance)
    {
        reverbParams.wetLevel = wetLevel;
        cachedWetLevel = wetLevel;
        needsUpdate = true;
    }
    
    if (std::abs (dryLevel - cachedDryLevel) > tolerance)
    {
        reverbParams.dryLevel = dryLevel;
        cachedDryLevel = dryLevel;
        needsUpdate = true;
    }
    
    if (std::abs (width - cachedWidth) > tolerance)
    {
        reverbParams.width = width;
        cachedWidth = width;
        needsUpdate = true;
    }
    
    if (std::abs (freeze - cachedFreeze) > tolerance)
    {
        reverbParams.freezeMode = freeze > 0.5f;
        cachedFreeze = freeze;
        needsUpdate = true;
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

    // Room Size: 0.0 to 1.0, default 0.5
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("ROOMSIZE", 1), "Room Size",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f
    ));

    // Damping: 0.0 to 1.0, default 0.5
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("DAMPING", 1), "Damping",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.5f
    ));

    // Wet Level: 0.0 to 1.0, default 0.33
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("WET", 1), "Wet Level",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.33f
    ));

    // Dry Level: 0.0 to 1.0, default 0.4
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("DRY", 1), "Dry Level",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.4f
    ));

    // Width: 0.0 to 1.0, default 1.0
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("WIDTH", 1), "Width",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        1.0f
    ));

    // Freeze: 0.0 to 1.0, default 0.0 (toggle)
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID ("FREEZE", 1), "Freeze",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f),
        0.0f
    ));

    return { params.begin(), params.end() };
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ObsidianSpaceAudioProcessor();
}

