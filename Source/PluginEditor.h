/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ObsidianSpaceLookAndFeel.h"
#include "HeaderComponent.h"
#include "FooterComponent.h"
#include "VisualizerComponent.h"
#include "KnobControl.h"
#include "LabeledSliderRow.h"

class SliderPanel : public juce::Component
{
public:
    SliderPanel (const juce::String& title,
                 const juce::String& firstLabel, LabeledSliderRow::Unit firstUnit,
                 const juce::String& secondLabel, LabeledSliderRow::Unit secondUnit);

    LabeledSliderRow& getFirstRow();
    LabeledSliderRow& getSecondRow();

    void resized() override;
    void paint (juce::Graphics& g) override;

private:
    juce::Label titleLabel;
    LabeledSliderRow firstRow;
    LabeledSliderRow secondRow;
};

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

    ObsidianSpaceLookAndFeel lookAndFeel;
    HeaderComponent header;
    VisualizerComponent visualizer;
    FooterComponent footer;

    KnobControl roomSizeKnob;
    KnobControl decayKnob;
    KnobControl preDelayKnob;
    KnobControl dampingKnob;

    SliderPanel outputPanel;
    SliderPanel tonePanel;

    juce::Rectangle<int> knobRowBounds;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preDelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObsidianSpaceAudioProcessorEditor)
};

