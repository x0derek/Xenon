#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - OscSection.h
    UI Component for Oscillator Settings
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class OscSection : public juce::Component
{
    public:
        OscSection(juce::AudioProcessorValueTreeState& apvts)
            : waveTypeAtt(apvts, "waveType", waveSelector)
        {
            waveSelector.addItem("SINE", 1);
            waveSelector.addItem("SAW", 2);
            waveSelector.addItem("SQUARE", 3);
            waveSelector.addItem("TRIANGLE", 4);
            waveSelector.setSelectedId(1);
            addAndMakeVisible(waveSelector);

            waveLabel.setText("WAVE TYPE", juce::dontSendNotification);
            waveLabel.setJustificationType(juce::Justification::centred);
            waveLabel.setFont(juce::FontOptions(11.0f));
            addAndMakeVisible(waveLabel);
        }

        void resized() override
        {
            const int cbW = getWidth() - 30;
            const int cx = getWidth() / 2;

            waveLabel.setBounds(10, 24, getWidth() - 20, 15);
            waveSelector.setBounds(cx - cbW / 2, 44, cbW, 30);
        }

    private:
        juce::ComboBox waveSelector;
        juce::Label waveLabel;

        juce::AudioProcessorValueTreeState::ComboBoxAttachment waveTypeAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscSection)
};