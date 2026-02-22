#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class KnobControl : public juce::Component
{
public:
    enum class Unit
    {
        Percent,
        Seconds,
        Milliseconds,
        Hertz,
        None
    };

    KnobControl (const juce::String& titleText, Unit unitType);

    juce::Slider& getSlider();
    void resized() override;
    void paint (juce::Graphics& g) override;

private:
    void updateValueText();
    juce::String formatValue (double value) const;

    juce::Slider knob;
    juce::Label label;
    juce::Label valueBox;
    Unit unit = Unit::None;
};
