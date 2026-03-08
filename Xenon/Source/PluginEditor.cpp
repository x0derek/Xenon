#include "PluginProcessor.h"
#include "PluginEditor.h"

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    Xenon- PluginEditor.cpp
    GUI Implementation
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

XenonAudioProcessorEditor::XenonAudioProcessorEditor(XenonAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    envelopeSection(p.apvts),
    oscSection(p.apvts),
    filterSection(p.apvts),
    reverbSection(p.apvts),
    chorusSection(p.apvts),
    masterSection(p.apvts),
    presetSection(p)
{
    setLookAndFeel(&lnf);

    addAndMakeVisible(envelopeSection);
    addAndMakeVisible(oscSection);
    addAndMakeVisible(filterSection);
    addAndMakeVisible(reverbSection);
    addAndMakeVisible(chorusSection);
    addAndMakeVisible(masterSection);
    addAndMakeVisible(presetSection);

    setSize(500, 698);
}

XenonAudioProcessorEditor::~XenonAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void XenonAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(colBackground);

    // HEADER
    juce::Rectangle<float> header(0, 0, getWidth(), 40);
    g.setColour(colPanel);
    g.fillRect(header);
    g.setColour(colBorder);
    g.drawLine(0.0f, 40.0f, (float)getWidth(), 40.0f, 1.0f);
    g.setColour(colAccent);
    g.setFont(juce::FontOptions(22.0f).withStyle("Bold"));
    g.drawText("XENON", header, juce::Justification::centred);
    g.setColour(colAccent.withAlpha(0.3f));
    g.drawLine(20.0f, 39.0f, (float)(getWidth() - 20), 39.0f, 1.0f);

    auto drawSection = [&](juce::Rectangle<int> b, const juce::String& title)
    {
        g.setColour(colPanel);
        g.fillRoundedRectangle(b.toFloat(), 8.0f);
        g.setColour(colBorder);
        g.drawRoundedRectangle(b.toFloat(), 8.0f, 1.0f);
        g.setColour(colAccent2.withAlpha(0.9f));
        g.setFont(juce::FontOptions(10.0f).withStyle("Bold"));
        g.drawText(title, b.getX() + 10, b.getY() + 5, 120, 13, juce::Justification::left);
    };

    const int m = 10;
    const int t = 46;
    const int wL = (getWidth() - m * 3) / 2;
    const int wR = wL;
    const int xR = m * 2 + wL;
    const int rowH = (getHeight() - t - m * 5) / 4;

    // LEFT COLUMN
    drawSection({ m, t, wL, rowH }, "ENVELOPE");
    drawSection({ m, t + rowH + m, wL, rowH }, "FILTER");
    drawSection({ m, t + (rowH + m) * 2, wL, rowH * 2 + m }, "MASTER");
    // RIGHT COLUMN
    drawSection({ xR, t, wR, rowH }, "OSCILLATOR");
    drawSection({ xR, t + rowH + m, wR, rowH }, "REVERB");
    drawSection({ xR, t + (rowH + m) * 2, wR, rowH }, "CHORUS");
   drawSection({ xR, t + (rowH + m) * 3, wR, rowH }, "PRESETS");
}

void XenonAudioProcessorEditor::resized()
{
    const int m = 10;
    const int t = 46;
    const int w2 = (getWidth() - m * 3) / 2;
    const int h3 = (getHeight() - t - m * 5) / 4;

    //LEFT COLUMN
    envelopeSection.setBounds(m, t, w2, h3);
    filterSection.setBounds(m, t + h3 + m, w2, h3);
    masterSection.setBounds(m, t + (h3 + m) * 2, w2, h3 * 2 + m);
    //RIGHT COLUMN
    oscSection.setBounds(m * 2 + w2, t, w2, h3);
    reverbSection.setBounds(m * 2 + w2, t + h3 + m, w2, h3);
    chorusSection.setBounds(m * 2 + w2, t + (h3 + m) * 2, w2, h3);
    presetSection.setBounds(m * 2 + w2, t + (h3 + m) * 3, w2, h3);
}