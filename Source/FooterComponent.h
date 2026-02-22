#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class FooterComponent : public juce::Component,
                        private juce::Timer
{
public:
    FooterComponent();
    ~FooterComponent() override = default;

    void paint (juce::Graphics& g) override;

private:
    void timerCallback() override;

    float pulsePhase = 0.0f;
};
