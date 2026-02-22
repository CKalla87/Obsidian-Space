/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ObsidianSpaceAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ObsidianSpaceAudioProcessor();
    ~ObsidianSpaceAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter management
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

    // UI parameters
    std::atomic<float>* roomSizeParam = nullptr;
    std::atomic<float>* decayParam = nullptr;
    std::atomic<float>* preDelayParam = nullptr;
    std::atomic<float>* dampingParam = nullptr;
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* widthParam = nullptr;
    std::atomic<float>* lowCutParam = nullptr;
    std::atomic<float>* highCutParam = nullptr;
    std::atomic<float>* powerParam = nullptr;

private:
    //==============================================================================
    // DSP processing
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;
    
    void updateParameters();
    
    double currentSampleRate = 44100.0;

    // Cached parameter values
    float cachedRoomSize = 50.0f;
    float cachedDecay = 2.5f;
    float cachedPreDelay = 20.0f;
    float cachedDamping = 8000.0f;
    float cachedMix = 30.0f;
    float cachedWidth = 100.0f;
    float cachedLowCut = 20.0f;
    float cachedHighCut = 12000.0f;
    bool cachedPower = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObsidianSpaceAudioProcessor)
};

