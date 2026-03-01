#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    Xenon - XenonLookAndFeel.h
    Custom LookAndFeel for the Xenon Plugin
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

static const juce::Colour colBackground { 0xff0d0d14 };
static const juce::Colour colPanel { 0xff16161f };
static const juce::Colour colBorder { 0xff2a2a3a };
static const juce::Colour colAccent { 0xff00e5ff };
static const juce::Colour colAccent2 { 0xffb400ff };
static const juce::Colour colKnobBg { 0xff1e1e2e };
static const juce::Colour colText { 0xffccccdd };
static const juce::Colour colTextDim { 0xff666680 };

class XenonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    XenonLookAndFeel()
    {
        setColour(juce::Label::textColourId, colText);
        setColour(juce::Slider::textBoxTextColourId, colText);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour(juce::ComboBox::backgroundColourId, colKnobBg);
        setColour(juce::ComboBox::textColourId, colText);
        setColour(juce::ComboBox::outlineColourId, colBorder);
        setColour(juce::ComboBox::arrowColourId, colAccent);
        setColour(juce::PopupMenu::backgroundColourId, colPanel);
        setColour(juce::PopupMenu::textColourId, colText);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, colAccent.withAlpha(0.3f));
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override
    {
        juce::ignoreUnused(slider);

        float radius = (float)juce::jmin(width / 2, height / 2) - 6.0f;
        float centreX = (float)x + (float)width  * 0.5f;
        float centreY = (float)y + (float)height * 0.5f;
        float rx = centreX - radius;
        float ry = centreY - radius;
        float rw = radius * 2.0f;
        float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // SHADOW
        g.setColour(juce::Colours::black.withAlpha(0.6f));
        g.fillEllipse(rx + 2, ry + 3, rw, rw);

        // KNOB BACKGROUND
        g.setColour(colKnobBg);
        g.fillEllipse(rx, ry, rw, rw);
        g.setColour(colBorder);
        g.drawEllipse(rx, ry, rw, rw, 1.5f);

        // TORUS (background + value)
        juce::Path bgArc;
        bgArc.addArc(rx + 4, ry + 4, rw - 8, rw - 8, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(colBorder);
        g.strokePath(bgArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // VALUE
        juce::Path valArc;
        valArc.addArc(rx + 4, ry + 4, rw - 8, rw - 8, rotaryStartAngle, angle, true);
        juce::ColourGradient grad(colAccent2, centreX - radius, centreY,
                                  colAccent,  centreX + radius, centreY, false);
        g.setGradientFill(grad);
        g.strokePath(valArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // POINTER
        float tLen = radius * 0.55f;
        g.setColour(colAccent);
        g.drawLine(centreX, centreY,
                   centreX + std::sin(angle) * tLen,
                   centreY - std::cos(angle) * tLen, 2.5f);

        // CENTER
        g.setColour(colKnobBg.brighter(0.2f));
        g.fillEllipse(centreX - 4, centreY - 4, 8, 8);
        g.setColour(colAccent.withAlpha(0.5f));
        g.drawEllipse(centreX - 4, centreY - 4, 8, 8, 1.0f);
    }
};