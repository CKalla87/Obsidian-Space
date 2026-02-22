#include "KnobControl.h"

KnobControl::KnobControl (const juce::String& titleText, Unit unitType)
    : unit (unitType)
{
    knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    knob.setMouseDragSensitivity (100);
    knob.onValueChange = [this] { updateValueText(); };
    addAndMakeVisible (knob);

    label.setText (titleText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (12.0f, juce::Font::plain));
    label.setColour (juce::Label::textColourId, ObsidianStyle::textSecondary().withAlpha (0.9f));
    addAndMakeVisible (label);

    valueBox.setJustificationType (juce::Justification::centred);
    valueBox.setFont (juce::Font (12.0f, juce::Font::plain));
    valueBox.setColour (juce::Label::textColourId, ObsidianStyle::textPrimary());
    valueBox.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (valueBox);

    updateValueText();
}

juce::Slider& KnobControl::getSlider()
{
    return knob;
}

void KnobControl::resized()
{
    auto bounds = getLocalBounds();
    const int knobSize = 90;
    const int labelHeight = 16;
    const int valueHeight = 20;
    const int spacing = 6;

    const int totalHeight = knobSize + spacing + labelHeight + spacing + valueHeight;
    auto knobX = bounds.getX() + (bounds.getWidth() - knobSize) / 2;
    auto knobY = bounds.getY() + (bounds.getHeight() - totalHeight) / 2;

    knob.setBounds (knobX, knobY, knobSize, knobSize);
    label.setBounds (bounds.getX(), knob.getBottom() + spacing, bounds.getWidth(), labelHeight);
    valueBox.setBounds (bounds.getX() + 8, label.getBottom() + spacing, bounds.getWidth() - 16, valueHeight);
}

void KnobControl::paint (juce::Graphics& g)
{
    auto box = valueBox.getBounds().toFloat();
    g.setColour (ObsidianStyle::panelFillDark());
    g.fillRoundedRectangle (box, 6.0f);
    g.setColour (ObsidianStyle::borderPurple());
    g.drawRoundedRectangle (box, 6.0f, 1.0f);
}

void KnobControl::updateValueText()
{
    valueBox.setText (formatValue (knob.getValue()), juce::dontSendNotification);
}

juce::String KnobControl::formatValue (double value) const
{
    switch (unit)
    {
        case Unit::Percent:
            return juce::String (juce::roundToInt (value)) + "%";
        case Unit::Seconds:
            return juce::String (value, 1) + "s";
        case Unit::Milliseconds:
            return juce::String (juce::roundToInt (value)) + "ms";
        case Unit::Hertz:
            if (value >= 1000.0)
                return juce::String (value / 1000.0, 1) + "kHz";
            return juce::String (juce::roundToInt (value)) + "Hz";
        case Unit::None:
        default:
            return juce::String (value, 1);
    }
}
