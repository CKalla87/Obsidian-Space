#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class HeaderComponent : public juce::Component
{
public:
    HeaderComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;

    juce::Button& getPowerButton();

private:
    void drawTrackedText (juce::Graphics& g, const juce::String& text,
                          juce::Rectangle<float> area, juce::Font font, float tracking);

    class PowerButton : public juce::ToggleButton
    {
    public:
        void paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;
    };

    PowerButton powerButton;
};
