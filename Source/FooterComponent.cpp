#include "FooterComponent.h"

FooterComponent::FooterComponent()
{
}

void FooterComponent::setPowerParam (std::atomic<float>* param)
{
    powerParam = param;
    repaint();
}

void FooterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    auto textBounds = bounds.reduced (32.0f, 6.0f);
    g.setColour (ObsidianStyle::textSecondary().withAlpha (0.7f));
    g.setFont (juce::Font (12.0f, juce::Font::plain));
    g.drawText ("VERSION 1.0.0  |  48kHz / 24bit", textBounds, juce::Justification::centredLeft);

    const bool isPowered = powerParam == nullptr || powerParam->load() > 0.5f;
    auto indicatorArea = textBounds.removeFromRight (120.0f);
    g.setColour (ObsidianStyle::textSecondary().withAlpha (0.8f));
    g.drawText (isPowered ? "POWER ON" : "POWER OFF",
                indicatorArea,
                juce::Justification::centredRight);
}
