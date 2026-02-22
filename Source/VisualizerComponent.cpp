#include "VisualizerComponent.h"
#include <cmath>

VisualizerComponent::VisualizerComponent (std::atomic<float>* roomSize,
                                          std::atomic<float>* decay,
                                          std::atomic<float>* damping)
    : roomSizeParam (roomSize),
      decayParam (decay),
      dampingParam (damping)
{
    startTimerHz (60);
}

void VisualizerComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    const float radius = 12.0f;

    juce::ColourGradient background (juce::Colour::fromRGB (0x0a, 0x0a, 0x0f),
                                     bounds.getX(), bounds.getY(),
                                     juce::Colour::fromRGB (0x05, 0x05, 0x08),
                                     bounds.getX(), bounds.getBottom(), false);
    g.setGradientFill (background);
    g.fillRoundedRectangle (bounds, radius);

    juce::Path clipPath;
    clipPath.addRoundedRectangle (bounds, radius);
    juce::Graphics::ScopedSaveState state (g);
    g.reduceClipRegion (clipPath);

    g.setColour (ObsidianStyle::accentPurple().withAlpha (0.08f));
    const int verticalDivs = 20;
    const int horizontalDivs = 8;

    for (int i = 1; i < verticalDivs; ++i)
    {
        auto x = bounds.getX() + bounds.getWidth() * (float) i / (float) verticalDivs;
        g.drawLine (x, bounds.getY(), x, bounds.getBottom(), 1.0f);
    }

    for (int i = 1; i < horizontalDivs; ++i)
    {
        auto y = bounds.getY() + bounds.getHeight() * (float) i / (float) horizontalDivs;
        g.drawLine (bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }

    auto drawWave = [&g](const juce::Path& path, juce::Colour colour)
    {
        g.setColour (colour.withAlpha (colour.getFloatAlpha() * 0.35f));
        g.strokePath (path, juce::PathStrokeType (6.0f));
        g.setColour (colour.withAlpha (colour.getFloatAlpha() * 0.6f));
        g.strokePath (path, juce::PathStrokeType (3.5f));
        g.setColour (colour);
        g.strokePath (path, juce::PathStrokeType (2.0f));
    };

    drawWave (createWavePath (phase + 0.2f, -10.0f), ObsidianStyle::accentPurple().withAlpha (0.3f));
    drawWave (createWavePath (phase + 0.6f, -5.0f), ObsidianStyle::accentViolet().withAlpha (0.4f));
    drawWave (createWavePath (phase + 1.0f, 2.0f), ObsidianStyle::accentLavender().withAlpha (0.6f));
    drawWave (createWavePath (phase + 1.4f, 6.0f), ObsidianStyle::accentLight().withAlpha (0.9f));

    const int particleCount = 12;
    for (int i = 0; i < particleCount; ++i)
    {
        auto t = (float) i / (float) (particleCount - 1);
        auto x = bounds.getX() + t * bounds.getWidth();

        auto waveY = bounds.getCentreY();
        auto decayValue = decayParam ? decayParam->load() : 2.5f;
        auto dampingValue = dampingParam ? dampingParam->load() : 8000.0f;
        auto roomValue = roomSizeParam ? roomSizeParam->load() : 50.0f;

        auto decayNorm = juce::jlimit (0.0f, 1.0f, (decayValue - 0.1f) / 9.9f);
        auto dampingNorm = juce::jlimit (0.0f, 1.0f,
                                         (std::log (dampingValue) - std::log (1000.0f)) /
                                         (std::log (20000.0f) - std::log (1000.0f)));
        auto decayFactor = 3.0f + decayNorm * 7.0f;
        auto dampingFactor = 1.0f - dampingNorm * 0.5f;
        auto freq1 = 25.0f * (1.0f + roomValue / 100.0f);
        auto freq2 = 15.0f * (1.0f + roomValue / 150.0f);
        auto envelope = std::exp (-t * decayFactor);
        auto oscillation = (std::sin (t * freq1 + phase) * 0.6f + std::sin (t * freq2 - phase * 0.5f) * 0.4f)
                           * envelope * dampingFactor;
        waveY += oscillation * (bounds.getHeight() * 0.35f);

        auto baseColour = ObsidianStyle::accentLight().withAlpha (0.6f);
        g.setColour (baseColour);
        g.fillEllipse (x - 2.0f, waveY - 2.0f, 4.0f, 4.0f);
        g.setColour (baseColour.withAlpha (0.25f));
        g.fillEllipse (x - 5.0f, waveY - 5.0f, 10.0f, 10.0f);
    }

    g.setColour (ObsidianStyle::borderPurple());
    g.drawRoundedRectangle (bounds, radius, 1.0f);
}

void VisualizerComponent::timerCallback()
{
    phase += 0.02f;
    if (phase > juce::MathConstants<float>::twoPi)
        phase -= juce::MathConstants<float>::twoPi;
    repaint();
}

juce::Path VisualizerComponent::createWavePath (float phaseOffset, float offset) const
{
    auto bounds = getLocalBounds().toFloat().reduced (6.0f, 4.0f);
    auto centreY = bounds.getCentreY();
    const int points = 300;

    auto roomValue = roomSizeParam ? roomSizeParam->load() : 50.0f;
    auto decayValue = decayParam ? decayParam->load() : 2.5f;
    auto dampingValue = dampingParam ? dampingParam->load() : 8000.0f;

    auto decayNorm = juce::jlimit (0.0f, 1.0f, (decayValue - 0.1f) / 9.9f);
    auto dampingNorm = juce::jlimit (0.0f, 1.0f,
                                     (std::log (dampingValue) - std::log (1000.0f)) /
                                     (std::log (20000.0f) - std::log (1000.0f)));
    auto decayFactor = 3.0f + decayNorm * 7.0f;
    auto dampingFactor = 1.0f - dampingNorm * 0.5f;
    auto freq1 = 25.0f * (1.0f + roomValue / 100.0f);
    auto freq2 = 15.0f * (1.0f + roomValue / 150.0f);

    juce::Path path;
    for (int i = 0; i < points; ++i)
    {
        auto t = (float) i / (float) (points - 1);
        auto x = bounds.getX() + t * bounds.getWidth();
        auto envelope = std::exp (-t * decayFactor);
        auto oscillation = (std::sin (t * freq1 + phaseOffset) * 0.6f
                            + std::sin (t * freq2 - phaseOffset * 0.5f) * 0.4f)
                           * envelope * dampingFactor;
        auto y = centreY + oscillation * (bounds.getHeight() * 0.35f) + offset;

        if (i == 0)
            path.startNewSubPath (x, y);
        else
            path.lineTo (x, y);
    }

    return path;
}
