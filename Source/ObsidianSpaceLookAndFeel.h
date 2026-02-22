#pragma once

#include <JuceHeader.h>

namespace ObsidianStyle
{
    inline juce::Colour backgroundTop()     { return juce::Colour::fromRGB (0x1a, 0x16, 0x25); }
    inline juce::Colour backgroundMid()     { return juce::Colour::fromRGB (0x12, 0x0f, 0x1d); }
    inline juce::Colour backgroundBottom()  { return juce::Colour::fromRGB (0x0d, 0x0a, 0x15); }
    inline juce::Colour headerTop()         { return juce::Colour::fromRGB (0x1f, 0x1a, 0x2e); }
    inline juce::Colour headerMid()         { return juce::Colour::fromRGB (0x1a, 0x16, 0x28); }
    inline juce::Colour accentPurple()      { return juce::Colour::fromRGB (0x8b, 0x5c, 0xf6); }
    inline juce::Colour accentViolet()      { return juce::Colour::fromRGB (0xa8, 0x55, 0xf7); }
    inline juce::Colour accentLavender()    { return juce::Colour::fromRGB (0xc0, 0x84, 0xfc); }
    inline juce::Colour accentLight()       { return juce::Colour::fromRGB (0xd8, 0xb4, 0xfe); }
    inline juce::Colour borderPurple()      { return accentPurple().withAlpha (0.25f); }
    inline juce::Colour textPrimary()       { return juce::Colour::fromRGB (0xf2, 0xef, 0xff); }
    inline juce::Colour textSecondary()     { return juce::Colour::fromRGB (0xc9, 0xc3, 0xe6); }
    inline juce::Colour panelOverlay()      { return juce::Colour::fromRGB (0x00, 0x00, 0x00).withAlpha (0.20f); }
    inline juce::Colour panelFill()         { return juce::Colour::fromRGB (0x19, 0x14, 0x24); }
    inline juce::Colour panelFillDark()     { return juce::Colour::fromRGB (0x12, 0x0d, 0x1b); }
}

class ObsidianSpaceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override;
};
