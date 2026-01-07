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
class ObsidianSpaceAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ObsidianSpaceAudioProcessorEditor (ObsidianSpaceAudioProcessor&);
    ~ObsidianSpaceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ObsidianSpaceAudioProcessor& audioProcessor;

    // Outer space theme colors
    juce::Colour spaceBlack = juce::Colour::fromFloatRGBA (0.02f, 0.02f, 0.05f, 1.0f);  // Deep space black
    juce::Colour nebulaPurple = juce::Colour::fromFloatRGBA (0.4f, 0.2f, 0.6f, 1.0f);  // Nebula purple
    juce::Colour starBlue = juce::Colour::fromFloatRGBA (0.3f, 0.5f, 0.9f, 1.0f);  // Star blue
    juce::Colour starCyan = juce::Colour::fromFloatRGBA (0.2f, 0.8f, 1.0f, 1.0f);  // Bright cyan
    juce::Colour starWhite = juce::Colour::fromFloatRGBA (0.95f, 0.95f, 1.0f, 1.0f);  // Star white
    juce::Colour textColor = starWhite;

    // Fixed star and nebula positions (to prevent background from changing)
    struct Star
    {
        float x, y, size;
    };
    struct Nebula
    {
        float x, y, radius;
    };
    std::vector<Star> stars;
    std::vector<Nebula> nebulas;
    void initializeBackground();

    // Controls
    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetSlider;
    juce::Slider drySlider;
    juce::Slider widthSlider;
    juce::Slider freezeSlider;
    
    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetLabel;
    juce::Label dryLabel;
    juce::Label widthLabel;
    juce::Label freezeLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freezeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObsidianSpaceAudioProcessorEditor)
};

