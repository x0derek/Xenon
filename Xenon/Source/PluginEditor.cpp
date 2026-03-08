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
    masterSection(p.apvts)
{
    setLookAndFeel(&lnf);

    addAndMakeVisible(envelopeSection);
    addAndMakeVisible(oscSection);
    addAndMakeVisible(filterSection);
    addAndMakeVisible(reverbSection);
    addAndMakeVisible(chorusSection);
    addAndMakeVisible(masterSection);

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
    const int w2 = (getWidth() - 3 * m) / 2;
    const int h3 = (getHeight() - t - m * 5) / 4;

    // ROW 1
    drawSection({ m, t, w2, h3 }, "ENVELOPE");
    drawSection({ m * 2 + w2, t, w2, h3 }, "OSCILLATOR");
    // ROW 2
    drawSection({ m, t + h3 + m, w2, h3 }, "FILTER");
    drawSection({ m * 2 + w2, t + h3 + m, w2, h3 }, "REVERB");
    // ROW 3
    drawSection({ m, t + h3 * 2 + m * 2, getWidth() - m * 2, h3 }, "CHORUS");
    // ROW 4
    drawSection({ m, t + h3 * 3 + m * 3, getWidth() - m * 2, h3 }, "MASTER");
}

void XenonAudioProcessorEditor::resized()
{
    const int m = 10;
    const int t = 46;
    const int w2 = (getWidth() - m * 3) / 2;
    const int h3 = (getHeight() - t - m * 5) / 4;

    // ROW 1
    envelopeSection.setBounds(m, t, w2, h3);
    oscSection.setBounds(m * 2 + w2, t, w2, h3);
    // ROW 2
    filterSection.setBounds(m, t + h3 + m, w2, h3);
    reverbSection.setBounds(m * 2 + w2, t + h3 + m, w2, h3);
    // ROW 3
    chorusSection.setBounds(m, t + h3 * 2 + m * 2, getWidth() - m * 2, h3);
    // ROW 4
    masterSection.setBounds(m, t + h3 * 3 + m * 3, getWidth() - m * 2, h3);
}