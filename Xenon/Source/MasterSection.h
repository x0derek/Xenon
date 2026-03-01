#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - MasterSection.h
    UI Component for Master Settings
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class MasterSection : public juce::Component
{
    public:
        MasterSection(juce::AudioProcessorValueTreeState& apvts)
            : gainAtt(apvts, "gain", gainSlider),
            pitchAtt(apvts, "pitch", pitchSlider)
        {
            auto setup = [&](juce::Slider& s)
            {
                s.setSliderStyle(juce::Slider::RotaryVerticalDrag);
                s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
                addAndMakeVisible(s);
            };

            setup(gainSlider);
            setup(pitchSlider);

            auto setupLabel = [&](juce::Label& l, const juce::String& t)
            {
                l.setText(t, juce::dontSendNotification);
                l.setJustificationType(juce::Justification::centred);
                l.setFont(juce::FontOptions(11.0f));
                addAndMakeVisible(l);
            };

            setupLabel(gainLabel, "GAIN");
            setupLabel(pitchLabel, "PITCH");
        }

        void resized() override
        {
            const int labelH = 15;
            const int knobH = 90;
            const int knobY = 24;
            const int colW = getWidth() / 2;

            auto place = [&](juce::Slider& knob, juce::Label& label, int col)
            {
                int x = col * colW;
                knob.setBounds(x, knobY, colW, knobH);
                label.setBounds(x, knobY + knobH, colW, labelH);
            };

            place(gainSlider, gainLabel, 0);
            place(pitchSlider, pitchLabel, 1);
        }

    private:
        juce::Slider gainSlider, pitchSlider;
        juce::Label gainLabel, pitchLabel;

        juce::AudioProcessorValueTreeState::SliderAttachment gainAtt, pitchAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterSection)
};