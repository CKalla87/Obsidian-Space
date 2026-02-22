#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class HeaderComponent : public juce::Component
{
public:
    void paint (juce::Graphics& g) override;

private:
    void drawTrackedText (juce::Graphics& g, const juce::String& text,
                          juce::Rectangle<float> area, juce::Font font, float tracking);
};
