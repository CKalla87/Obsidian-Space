#include "HeaderComponent.h"

HeaderComponent::HeaderComponent()
{
    powerButton.setClickingTogglesState (true);
    powerButton.setToggleState (true, juce::dontSendNotification);
    powerButton.setTooltip ("Power");
    addAndMakeVisible (powerButton);
}

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
    auto textArea = content;
    textArea.removeFromRight (44.0f);

    juce::Font titleFont (32.0f, juce::Font::plain);
    g.setColour (ObsidianStyle::textPrimary());
    drawTrackedText (g, "OBSIDIAN SPACE", textArea.removeFromLeft (360.0f), titleFont, 2.5f);

    juce::Font typeFont (12.5f, juce::Font::plain);
    g.setFont (typeFont);
    g.setColour (ObsidianStyle::textSecondary().withAlpha (0.7f));
    g.drawText ("ALGORITHMIC REVERB", textArea, juce::Justification::centredRight);
}

void HeaderComponent::resized()
{
    auto bounds = getLocalBounds().reduced (32, 10);
    const int buttonSize = 28;
    auto buttonY = bounds.getY() + (bounds.getHeight() - buttonSize) / 2;
    powerButton.setBounds (bounds.getRight() - buttonSize, buttonY, buttonSize, buttonSize);
}

juce::Button& HeaderComponent::getPowerButton()
{
    return powerButton;
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

void HeaderComponent::PowerButton::paintButton (juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
                                                bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused (shouldDrawButtonAsDown);

    auto bounds = getLocalBounds().toFloat().reduced (1.5f);
    auto centre = bounds.getCentre();
    const bool isOn = getToggleState();

    auto baseFill = ObsidianStyle::panelFillDark().withAlpha (isOn ? 0.9f : 0.7f);
    g.setColour (baseFill);
    g.fillEllipse (bounds);

    auto ringColour = isOn
        ? ObsidianStyle::accentLight()
        : ObsidianStyle::textSecondary().withAlpha (0.6f);

    if (shouldDrawButtonAsHighlighted)
        ringColour = ringColour.withAlpha (juce::jmin (1.0f, ringColour.getFloatAlpha() + 0.2f));

    g.setColour (ringColour);
    g.drawEllipse (bounds, 1.4f);

    const float iconRadius = bounds.getWidth() * 0.28f;
    const float strokeWidth = isOn ? 2.2f : 1.8f;

    juce::Path icon;
    icon.addCentredArc (centre.x, centre.y, iconRadius, iconRadius, 0.0f,
                        juce::MathConstants<float>::pi * 0.25f,
                        juce::MathConstants<float>::pi * 1.75f, true);
    g.strokePath (icon, juce::PathStrokeType (strokeWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    g.drawLine (centre.x, centre.y - iconRadius - 2.0f,
                centre.x, centre.y - iconRadius + 7.0f, strokeWidth);
}
