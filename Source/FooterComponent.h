#pragma once

#include <JuceHeader.h>
#include "ObsidianSpaceLookAndFeel.h"

class FooterComponent : public juce::Component
{
public:
    FooterComponent();
    ~FooterComponent() override = default;

    void paint (juce::Graphics& g) override;
    void setPowerParam (std::atomic<float>* param);

private:
    std::atomic<float>* powerParam = nullptr;
};
