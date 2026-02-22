/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

SliderPanel::SliderPanel (const juce::String& title,
                          const juce::String& firstLabel, LabeledSliderRow::Unit firstUnit,
                          const juce::String& secondLabel, LabeledSliderRow::Unit secondUnit)
    : firstRow (firstLabel, firstUnit),
      secondRow (secondLabel, secondUnit)
{
    titleLabel.setText (title, juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centredLeft);
    titleLabel.setFont (juce::Font (12.0f, juce::Font::plain));
    titleLabel.setColour (juce::Label::textColourId, ObsidianStyle::textSecondary().withAlpha (0.85f));
    addAndMakeVisible (titleLabel);

    addAndMakeVisible (firstRow);
    addAndMakeVisible (secondRow);
}

LabeledSliderRow& SliderPanel::getFirstRow()
{
    return firstRow;
}

LabeledSliderRow& SliderPanel::getSecondRow()
{
    return secondRow;
}

void SliderPanel::resized()
{
    auto bounds = getLocalBounds().reduced (20, 18);
    titleLabel.setBounds (bounds.removeFromTop (18));
    bounds.removeFromTop (12);

    auto rowHeight = (bounds.getHeight() - 16) / 2;
    firstRow.setBounds (bounds.removeFromTop (rowHeight));
    bounds.removeFromTop (16);
    secondRow.setBounds (bounds.removeFromTop (rowHeight));
}

void SliderPanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour (ObsidianStyle::panelFill());
    g.fillRoundedRectangle (bounds, 12.0f);
    g.setColour (ObsidianStyle::panelOverlay());
    g.fillRoundedRectangle (bounds, 12.0f);
    g.setColour (ObsidianStyle::borderPurple());
    g.drawRoundedRectangle (bounds, 12.0f, 1.0f);
}

//==============================================================================
ObsidianSpaceAudioProcessorEditor::ObsidianSpaceAudioProcessorEditor (ObsidianSpaceAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      lookAndFeel(),
      header(),
      visualizer (p.roomSizeParam, p.decayParam, p.dampingParam),
      footer(),
      roomSizeKnob ("ROOM SIZE", KnobControl::Unit::Percent),
      decayKnob ("DECAY", KnobControl::Unit::Seconds),
      preDelayKnob ("PRE-DELAY", KnobControl::Unit::Milliseconds),
      dampingKnob ("DAMPING", KnobControl::Unit::Hertz),
      outputPanel ("OUTPUT CONTROL", "DRY / WET MIX", LabeledSliderRow::Unit::Percent,
                   "STEREO WIDTH", LabeledSliderRow::Unit::Percent),
      tonePanel ("TONE SHAPING", "LOW CUT", LabeledSliderRow::Unit::Hertz,
                 "HIGH CUT", LabeledSliderRow::Unit::Hertz)
{
    setLookAndFeel (&lookAndFeel);
    setSize (1288, 991);

    addAndMakeVisible (header);
    addAndMakeVisible (visualizer);
    addAndMakeVisible (roomSizeKnob);
    addAndMakeVisible (decayKnob);
    addAndMakeVisible (preDelayKnob);
    addAndMakeVisible (dampingKnob);
    addAndMakeVisible (outputPanel);
    addAndMakeVisible (tonePanel);
    addAndMakeVisible (footer);

    footer.setPowerParam (audioProcessor.powerParam);
    header.getPowerButton().onClick = [this] { footer.repaint(); };

    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "ROOMSIZE", roomSizeKnob.getSlider());
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "DECAY", decayKnob.getSlider());
    preDelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "PREDELAY", preDelayKnob.getSlider());
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "DAMPING", dampingKnob.getSlider());

    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "MIX", outputPanel.getFirstRow().getSlider());
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "WIDTH", outputPanel.getSecondRow().getSlider());
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "LOWCUT", tonePanel.getFirstRow().getSlider());
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "HIGHCUT", tonePanel.getSecondRow().getSlider());
    powerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (audioProcessor.apvts, "POWER", header.getPowerButton());
}

ObsidianSpaceAudioProcessorEditor::~ObsidianSpaceAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

//==============================================================================
void ObsidianSpaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient background (ObsidianStyle::backgroundTop(), bounds.getX(), bounds.getY(),
                                     ObsidianStyle::backgroundBottom(), bounds.getX(), bounds.getBottom(), false);
    background.addColour (0.5f, ObsidianStyle::backgroundMid());
    g.setGradientFill (background);
    g.fillAll();

    if (! knobRowBounds.isEmpty())
    {
        auto row = knobRowBounds.toFloat();
        g.setColour (ObsidianStyle::panelFill());
        g.fillRoundedRectangle (row, 12.0f);
        g.setColour (ObsidianStyle::panelOverlay());
        g.fillRoundedRectangle (row, 12.0f);
        g.setColour (ObsidianStyle::borderPurple());
        g.drawRoundedRectangle (row, 12.0f, 1.0f);
    }
}

void ObsidianSpaceAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    const int headerHeight = 80;
    const int footerHeight = 40;
    const int knobRowHeight = 200;
    const int bottomRowHeight = 260;
    const int sectionGap = 40;
    const int contentPadding = 40;
    const int visualizerHeight = 256;

    header.setBounds (bounds.removeFromTop (headerHeight));

    footer.setBounds (bounds.removeFromBottom (footerHeight));

    auto content = bounds.reduced (contentPadding);
    auto vizArea = content.removeFromTop (visualizerHeight);
    visualizer.setBounds (vizArea);
    content.removeFromTop (sectionGap);

    knobRowBounds = content.removeFromTop (knobRowHeight);
    const int knobSize = 90;
    const int gap = 40;
    const int totalWidth = knobSize * 4 + gap * 3;
    const int startX = knobRowBounds.getX() + (knobRowBounds.getWidth() - totalWidth) / 2;
    const int knobY = knobRowBounds.getY();

    roomSizeKnob.setBounds (startX, knobY, knobSize, knobRowBounds.getHeight());
    decayKnob.setBounds (startX + (knobSize + gap), knobY, knobSize, knobRowBounds.getHeight());
    preDelayKnob.setBounds (startX + (knobSize + gap) * 2, knobY, knobSize, knobRowBounds.getHeight());
    dampingKnob.setBounds (startX + (knobSize + gap) * 3, knobY, knobSize, knobRowBounds.getHeight());

    content.removeFromTop (sectionGap);

    auto bottomRow = content.removeFromTop (juce::jmin (bottomRowHeight, content.getHeight()));
    auto left = bottomRow.removeFromLeft ((bottomRow.getWidth() - 32) / 2);
    bottomRow.removeFromLeft (32);
    auto right = bottomRow;

    outputPanel.setBounds (left);
    tonePanel.setBounds (right);

    // Footer already positioned from bottom.
}

