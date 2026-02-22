#include "FooterComponent.h"

FooterComponent::FooterComponent()
{
    startTimerHz (30);
}

void FooterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    auto textBounds = bounds.reduced (32.0f, 6.0f);
    g.setColour (ObsidianStyle::textSecondary().withAlpha (0.7f));
    g.setFont (juce::Font (12.0f, juce::Font::plain));
    g.drawText ("VERSION 1.0.0  |  48kHz / 24bit", textBounds, juce::Justification::centredLeft);

    auto indicatorArea = textBounds.removeFromRight (100.0f);
    auto dotX = indicatorArea.getX();
    auto dotY = indicatorArea.getCentreY();
    auto dotRadius = 4.0f;
    auto glow = 0.6f + 0.4f * std::sin (pulsePhase);

    g.setColour (ObsidianStyle::accentLight().withAlpha (glow));
    g.fillEllipse (dotX, dotY - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);

    g.setColour (ObsidianStyle::textSecondary().withAlpha (0.8f));
    g.drawText ("ACTIVE", indicatorArea.withTrimmedLeft (14), juce::Justification::centredLeft);
}

void FooterComponent::timerCallback()
{
    pulsePhase += 0.08f;
    if (pulsePhase > juce::MathConstants<float>::twoPi)
        pulsePhase -= juce::MathConstants<float>::twoPi;
    repaint();
}
