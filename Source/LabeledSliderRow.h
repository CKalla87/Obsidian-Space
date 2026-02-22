#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class LabeledSliderRow : public juce::Component
{
public:
    enum class Unit
    {
        Percent,
        Hertz,
        None
    };

    LabeledSliderRow (const juce::String& titleText, Unit unitType);

    juce::Slider& getSlider();
    void resized() override;
    void paint (juce::Graphics& g) override;

private:
    void updateValueText();
    juce::String formatValue (double value) const;

    juce::Label label;
    juce::Label valueBox;
    juce::Slider slider;
    Unit unit = Unit::None;
};
