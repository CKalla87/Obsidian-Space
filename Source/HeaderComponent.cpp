#include "HeaderComponent.h"

void HeaderComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient headerGradient (ObsidianStyle::headerTop(), bounds.getX(), bounds.getY(),
                                         ObsidianStyle::headerMid(), bounds.getX(), bounds.getBottom(), false);
    g.setGradientFill (headerGradient);
    g.fillRect (bounds);

    g.setColour (ObsidianStyle::borderPurple());
    g.drawLine (bounds.getX(), bounds.getBottom() - 0.5f, bounds.getRight(), bounds.getBottom() - 0.5f, 1.0f);

    auto content = bounds.reduced (32.0f, 10.0f);

    juce::Font titleFont (32.0f, juce::Font::plain);
    g.setColour (ObsidianStyle::textPrimary());
    drawTrackedText (g, "OBSIDIAN SPACE", content.removeFromLeft (360.0f), titleFont, 2.5f);

    juce::Font typeFont (12.5f, juce::Font::plain);
    g.setFont (typeFont);
    g.setColour (ObsidianStyle::textSecondary().withAlpha (0.7f));
    g.drawText ("ALGORITHMIC REVERB", content, juce::Justification::centredRight);
}

void HeaderComponent::drawTrackedText (juce::Graphics& g, const juce::String& text,
                                       juce::Rectangle<float> area, juce::Font font, float tracking)
{
    g.setFont (font);
    auto x = area.getX();
    auto y = area.getY() + (area.getHeight() - font.getHeight()) * 0.5f;

    for (auto character : text)
    {
        auto glyph = juce::String::charToString (character);
        auto width = font.getStringWidthFloat (glyph);
        g.drawText (glyph, x, y, width + tracking, font.getHeight(), juce::Justification::centredLeft, false);
        x += width + tracking;
    }
}
