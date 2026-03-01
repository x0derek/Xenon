#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - FilterSection.h
    UI Component for Filter Settings
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class FilterSection : public juce::Component
{
    public:
        FilterSection(juce::AudioProcessorValueTreeState& apvts)
            : cutoffAtt(apvts, "filterCutoff", cutoffKnob),
            resonanceAtt(apvts, "filterResonance", resonanceKnob)
        {
            auto setup = [&](juce::Slider& s)
            {
                s.setSliderStyle(juce::Slider::RotaryVerticalDrag);
                s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
                addAndMakeVisible(s);
            };

            setup(cutoffKnob);
            setup(resonanceKnob);

            auto setupLabel = [&](juce::Label& l, const juce::String& t)
            {
                l.setText(t, juce::dontSendNotification);
                l.setJustificationType(juce::Justification::centred);
                l.setFont(juce::FontOptions(11.0f));
                addAndMakeVisible(l);
            };

            setupLabel(cutoffLabel, "CUTOFF");
            setupLabel(resonanceLabel, "RESONANCE");
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

            place(cutoffKnob, cutoffLabel, 0);
            place(resonanceKnob, resonanceLabel, 1);
        }

    private:
        juce::Slider cutoffKnob, resonanceKnob;
        juce::Label cutoffLabel, resonanceLabel;

        juce::AudioProcessorValueTreeState::SliderAttachment cutoffAtt, resonanceAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterSection)
};