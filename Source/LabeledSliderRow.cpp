#include "LabeledSliderRow.h"

LabeledSliderRow::LabeledSliderRow (const juce::String& titleText, Unit unitType)
    : unit (unitType)
{
    label.setText (titleText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centredLeft);
    label.setFont (juce::Font (11.5f, juce::Font::plain));
    label.setColour (juce::Label::textColourId, ObsidianStyle::textSecondary());
    addAndMakeVisible (label);

    valueBox.setJustificationType (juce::Justification::centred);
    valueBox.setFont (juce::Font (11.5f, juce::Font::plain));
    valueBox.setColour (juce::Label::textColourId, ObsidianStyle::textPrimary());
    valueBox.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (valueBox);

    slider.setSliderStyle (juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.onValueChange = [this] { updateValueText(); };
    addAndMakeVisible (slider);

    updateValueText();
}

juce::Slider& LabeledSliderRow::getSlider()
{
    return slider;
}

void LabeledSliderRow::resized()
{
    auto bounds = getLocalBounds();
    const int topRowHeight = 18;
    const int valueWidth = 64;
    const int valueHeight = 18;

    auto top = bounds.removeFromTop (topRowHeight);
    valueBox.setBounds (top.removeFromRight (valueWidth));
    label.setBounds (top);

    bounds.removeFromTop (8);
    slider.setBounds (bounds.removeFromTop (22));
}

void LabeledSliderRow::paint (juce::Graphics& g)
{
    auto box = valueBox.getBounds().toFloat();
    g.setColour (ObsidianStyle::panelFillDark());
    g.fillRoundedRectangle (box, 5.0f);
    g.setColour (ObsidianStyle::borderPurple());
    g.drawRoundedRectangle (box, 5.0f, 1.0f);
}

void LabeledSliderRow::updateValueText()
{
    valueBox.setText (formatValue (slider.getValue()), juce::dontSendNotification);
}

juce::String LabeledSliderRow::formatValue (double value) const
{
    switch (unit)
    {
        case Unit::Percent:
            return juce::String (juce::roundToInt (value)) + "%";
        case Unit::Hertz:
            if (value >= 1000.0)
                return juce::String (value / 1000.0, 1) + "kHz";
            return juce::String (juce::roundToInt (value)) + "Hz";
        case Unit::None:
        default:
            return juce::String (value, 1);
    }
}
