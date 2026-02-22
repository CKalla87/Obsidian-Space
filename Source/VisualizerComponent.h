#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class VisualizerComponent : public juce::Component,
                            private juce::Timer
{
public:
    VisualizerComponent (std::atomic<float>* roomSizeParam,
                         std::atomic<float>* decayParam,
                         std::atomic<float>* dampingParam);

    void paint (juce::Graphics& g) override;

private:
    void timerCallback() override;
    juce::Path createWavePath (float phase, float offset) const;

    std::atomic<float>* roomSizeParam = nullptr;
    std::atomic<float>* decayParam = nullptr;
    std::atomic<float>* dampingParam = nullptr;

    float phase = 0.0f;
};
