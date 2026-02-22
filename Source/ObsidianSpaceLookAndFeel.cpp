#include "ObsidianSpaceLookAndFeel.h"

void ObsidianSpaceLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                                 float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                                 juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (3.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    auto centre = bounds.getCentre();

    if (slider.isMouseOver())
    {
        g.setColour (ObsidianStyle::accentViolet().withAlpha (0.25f));
        g.fillEllipse (bounds.expanded (3.0f));
    }

    juce::ColourGradient bodyGradient (juce::Colour::fromRGB (0x1b, 0x15, 0x24),
                                       centre.x, centre.y - radius,
                                       juce::Colour::fromRGB (0x0e, 0x0a, 0x14),
                                       centre.x, centre.y + radius, false);
    g.setGradientFill (bodyGradient);
    g.fillEllipse (bounds);

    g.setColour (juce::Colour::fromRGB (0x2b, 0x22, 0x36).withAlpha (0.8f));
    g.drawEllipse (bounds, 1.0f);

    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::Path arc;
    arc.addArc (bounds.getX() + 3.0f, bounds.getY() + 3.0f,
                bounds.getWidth() - 6.0f, bounds.getHeight() - 6.0f,
                rotaryStartAngle, angle, true);
    g.setColour (ObsidianStyle::accentPurple().withAlpha (0.9f));
    g.strokePath (arc, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path pointer;
    auto pointerLength = radius * 0.65f;
    juce::Rectangle<float> pointerRect (-3.0f, -pointerLength, 6.0f, 16.0f);
    pointer.addRoundedRectangle (pointerRect, 3.0f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (centre.x, centre.y));

    g.setColour (ObsidianStyle::accentLight());
    g.fillPath (pointer);
}

void ObsidianSpaceLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                                 float sliderPos, float minSliderPos, float maxSliderPos,
                                                 const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    juce::ignoreUnused (minSliderPos, maxSliderPos);

    if (style != juce::Slider::LinearHorizontal)
    {
        juce::LookAndFeel_V4::drawLinearSlider (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        return;
    }

    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (1.0f);
    auto trackHeight = 8.0f;
    auto track = juce::Rectangle<float> (bounds.getX(), bounds.getCentreY() - trackHeight * 0.5f,
                                         bounds.getWidth(), trackHeight);

    juce::ColourGradient trackGradient (juce::Colour::fromRGB (0x1b, 0x14, 0x24),
                                        track.getX(), track.getY(),
                                        juce::Colour::fromRGB (0x10, 0x0b, 0x18),
                                        track.getRight(), track.getBottom(), false);
    g.setGradientFill (trackGradient);
    g.fillRoundedRectangle (track, trackHeight * 0.5f);

    g.setColour (ObsidianStyle::accentPurple().withAlpha (0.35f));
    g.drawRoundedRectangle (track, trackHeight * 0.5f, 1.0f);

    auto fill = track.withWidth (sliderPos - track.getX());
    juce::ColourGradient fillGradient (ObsidianStyle::accentPurple(),
                                       fill.getX(), fill.getY(),
                                       ObsidianStyle::accentViolet(),
                                       fill.getRight(), fill.getBottom(), false);
    g.setGradientFill (fillGradient);
    g.fillRoundedRectangle (fill, trackHeight * 0.5f);

    auto thumbRadius = 7.0f;
    auto thumbCentre = juce::Point<float> (sliderPos, track.getCentreY());

    if (slider.isMouseOver())
    {
        g.setColour (ObsidianStyle::accentViolet().withAlpha (0.35f));
        g.fillEllipse (thumbCentre.x - thumbRadius - 3.0f, thumbCentre.y - thumbRadius - 3.0f,
                       (thumbRadius + 3.0f) * 2.0f, (thumbRadius + 3.0f) * 2.0f);
    }

    g.setColour (ObsidianStyle::accentLight());
    g.fillEllipse (thumbCentre.x - thumbRadius, thumbCentre.y - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);
}
