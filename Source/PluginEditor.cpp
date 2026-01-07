/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ObsidianSpaceAudioProcessorEditor::ObsidianSpaceAudioProcessorEditor (ObsidianSpaceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (800, 600);
    
    // Initialize fixed background elements
    initializeBackground();

    // Room Size slider
    roomSizeSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    roomSizeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    roomSizeSlider.setColour (juce::Slider::rotarySliderFillColourId, nebulaPurple);
    roomSizeSlider.setColour (juce::Slider::rotarySliderOutlineColourId, starBlue);
    roomSizeSlider.setColour (juce::Slider::thumbColourId, starCyan);
    roomSizeSlider.setColour (juce::Slider::textBoxTextColourId, textColor);
    roomSizeSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceBlack);
    addAndMakeVisible (&roomSizeSlider);
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "ROOMSIZE", roomSizeSlider);
    
    roomSizeLabel.setText ("Room Size", juce::dontSendNotification);
    roomSizeLabel.setJustificationType (juce::Justification::centred);
    roomSizeLabel.setColour (juce::Label::textColourId, textColor);
    addAndMakeVisible (&roomSizeLabel);

    // Damping slider
    dampingSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    dampingSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    dampingSlider.setColour (juce::Slider::rotarySliderFillColourId, starBlue);
    dampingSlider.setColour (juce::Slider::rotarySliderOutlineColourId, nebulaPurple);
    dampingSlider.setColour (juce::Slider::thumbColourId, starCyan);
    dampingSlider.setColour (juce::Slider::textBoxTextColourId, textColor);
    dampingSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceBlack);
    addAndMakeVisible (&dampingSlider);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "DAMPING", dampingSlider);
    
    dampingLabel.setText ("Damping", juce::dontSendNotification);
    dampingLabel.setJustificationType (juce::Justification::centred);
    dampingLabel.setColour (juce::Label::textColourId, textColor);
    addAndMakeVisible (&dampingLabel);

    // Wet slider
    wetSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    wetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    wetSlider.setColour (juce::Slider::rotarySliderFillColourId, starCyan);
    wetSlider.setColour (juce::Slider::rotarySliderOutlineColourId, starBlue);
    wetSlider.setColour (juce::Slider::thumbColourId, starWhite);
    wetSlider.setColour (juce::Slider::textBoxTextColourId, textColor);
    wetSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceBlack);
    addAndMakeVisible (&wetSlider);
    wetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "WET", wetSlider);
    
    wetLabel.setText ("Wet", juce::dontSendNotification);
    wetLabel.setJustificationType (juce::Justification::centred);
    wetLabel.setColour (juce::Label::textColourId, textColor);
    addAndMakeVisible (&wetLabel);

    // Dry slider
    drySlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    drySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    drySlider.setColour (juce::Slider::rotarySliderFillColourId, starBlue);
    drySlider.setColour (juce::Slider::rotarySliderOutlineColourId, nebulaPurple);
    drySlider.setColour (juce::Slider::thumbColourId, starCyan);
    drySlider.setColour (juce::Slider::textBoxTextColourId, textColor);
    drySlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceBlack);
    addAndMakeVisible (&drySlider);
    dryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "DRY", drySlider);
    
    dryLabel.setText ("Dry", juce::dontSendNotification);
    dryLabel.setJustificationType (juce::Justification::centred);
    dryLabel.setColour (juce::Label::textColourId, textColor);
    addAndMakeVisible (&dryLabel);

    // Width slider
    widthSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    widthSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    widthSlider.setColour (juce::Slider::rotarySliderFillColourId, nebulaPurple);
    widthSlider.setColour (juce::Slider::rotarySliderOutlineColourId, starBlue);
    widthSlider.setColour (juce::Slider::thumbColourId, starCyan);
    widthSlider.setColour (juce::Slider::textBoxTextColourId, textColor);
    widthSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceBlack);
    addAndMakeVisible (&widthSlider);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "WIDTH", widthSlider);
    
    widthLabel.setText ("Width", juce::dontSendNotification);
    widthLabel.setJustificationType (juce::Justification::centred);
    widthLabel.setColour (juce::Label::textColourId, textColor);
    addAndMakeVisible (&widthLabel);

    // Freeze slider
    freezeSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    freezeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    freezeSlider.setColour (juce::Slider::rotarySliderFillColourId, starCyan);
    freezeSlider.setColour (juce::Slider::rotarySliderOutlineColourId, nebulaPurple);
    freezeSlider.setColour (juce::Slider::thumbColourId, starWhite);
    freezeSlider.setColour (juce::Slider::textBoxTextColourId, textColor);
    freezeSlider.setColour (juce::Slider::textBoxBackgroundColourId, spaceBlack);
    addAndMakeVisible (&freezeSlider);
    freezeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "FREEZE", freezeSlider);
    
    freezeLabel.setText ("Freeze", juce::dontSendNotification);
    freezeLabel.setJustificationType (juce::Justification::centred);
    freezeLabel.setColour (juce::Label::textColourId, textColor);
    addAndMakeVisible (&freezeLabel);
}

ObsidianSpaceAudioProcessorEditor::~ObsidianSpaceAudioProcessorEditor()
{
}

//==============================================================================
void ObsidianSpaceAudioProcessorEditor::initializeBackground()
{
    juce::Random r (12345); // Fixed seed for consistent background
    stars.clear();
    nebulas.clear();
    
    // Generate fixed star positions
    for (int i = 0; i < 200; ++i)
    {
        Star star;
        star.x = r.nextFloat() * 800.0f;
        star.y = r.nextFloat() * 600.0f;
        star.size = 0.5f + r.nextFloat() * 1.5f;
        stars.push_back (star);
    }
    
    // Generate fixed nebula positions
    for (int i = 0; i < 5; ++i)
    {
        Nebula nebula;
        nebula.x = r.nextFloat() * 800.0f;
        nebula.y = r.nextFloat() * 600.0f;
        nebula.radius = 80.0f + r.nextFloat() * 120.0f;
        nebulas.push_back (nebula);
    }
}

void ObsidianSpaceAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Deep space black background
    g.fillAll (spaceBlack);
    
    // Draw stars (using fixed positions)
    g.setColour (starWhite.withAlpha (0.8f));
    for (const auto& star : stars)
    {
        g.fillEllipse (star.x, star.y, star.size, star.size);
    }
    
    // Draw nebula clouds (using fixed positions)
    g.setColour (nebulaPurple.withAlpha (0.3f));
    for (const auto& nebula : nebulas)
    {
        g.fillEllipse (nebula.x - nebula.radius/2, nebula.y - nebula.radius/2, nebula.radius, nebula.radius);
    }
    
    // Draw title with space glow
    g.setFont (juce::Font (56.0f, juce::Font::bold));
    
    // Cyan glow behind text
    g.setColour (starCyan.withAlpha (0.5f));
    for (int i = -4; i <= 4; ++i)
    {
        for (int j = -4; j <= 4; ++j)
        {
            if (i != 0 || j != 0)
            {
                g.drawText ("OBSIDIAN SPACE", getWidth() / 2 - 220 + i, 30 + j, 440, 70,
                           juce::Justification::centred, false);
            }
        }
    }
    
    // Main title
    juce::ColourGradient titleGradient (starCyan, getWidth() / 2 - 220, 30,
                                       starBlue, getWidth() / 2 - 220, 100,
                                       false);
    g.setGradientFill (titleGradient);
    g.drawText ("OBSIDIAN SPACE", getWidth() / 2 - 220, 30, 440, 70,
               juce::Justification::centred, false);
    
    // Subtitle
    g.setFont (20.0f);
    g.setColour (starBlue.withAlpha (0.9f));
    g.drawText ("Deep Space Reverb", getWidth() / 2 - 150, 95, 300, 30,
               juce::Justification::centred, false);
}

void ObsidianSpaceAudioProcessorEditor::resized()
{
    const int knobSize = 100;
    const int labelHeight = 25;
    const int startY = 150;
    const int spacing = (getWidth() - 100 - (knobSize * 6)) / 5;
    
    int x = 50;
    
    // Room Size
    roomSizeSlider.setBounds (x, startY, knobSize, knobSize);
    roomSizeLabel.setBounds (x, startY + knobSize + 5, knobSize, labelHeight);
    x += knobSize + spacing;
    
    // Damping
    dampingSlider.setBounds (x, startY, knobSize, knobSize);
    dampingLabel.setBounds (x, startY + knobSize + 5, knobSize, labelHeight);
    x += knobSize + spacing;
    
    // Wet
    wetSlider.setBounds (x, startY, knobSize, knobSize);
    wetLabel.setBounds (x, startY + knobSize + 5, knobSize, labelHeight);
    x += knobSize + spacing;
    
    // Dry
    drySlider.setBounds (x, startY, knobSize, knobSize);
    dryLabel.setBounds (x, startY + knobSize + 5, knobSize, labelHeight);
    x += knobSize + spacing;
    
    // Width
    widthSlider.setBounds (x, startY, knobSize, knobSize);
    widthLabel.setBounds (x, startY + knobSize + 5, knobSize, labelHeight);
    x += knobSize + spacing;
    
    // Freeze
    freezeSlider.setBounds (x, startY, knobSize, knobSize);
    freezeLabel.setBounds (x, startY + knobSize + 5, knobSize, labelHeight);
}

