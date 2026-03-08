#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - ChorusSection.h
    UI Component for Chorus Settings
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class ChorusSection : public juce::Component
{
    public:
        ChorusSection(juce::AudioProcessorValueTreeState& apvts)
            : rateAtt(apvts, "chorusRate", rateKnob),
            depthAtt(apvts, "chorusDepth", depthKnob)
        {
            auto setup = [&](juce::Slider& s)
            {
                s.setSliderStyle(juce::Slider::RotaryVerticalDrag);
                s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
                addAndMakeVisible(s);
            };

            setup(rateKnob);
            setup(depthKnob);

            auto setupLabel = [&](juce::Label& l, const juce::String& t)
            {
                l.setText(t, juce::dontSendNotification);
                l.setJustificationType(juce::Justification::centred);
                l.setFont(juce::FontOptions(11.0f));
                addAndMakeVisible(l);
            };
            setupLabel(rateLabel, "Rate");
            setupLabel(depthLabel, "Depth");
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

            place(rateKnob, rateLabel, 0);
            place(depthKnob, depthLabel, 1);
        }

    private:
        juce::Slider rateKnob;
        juce::Slider depthKnob;
        juce::Label rateLabel;
        juce::Label depthLabel;

        juce::AudioProcessorValueTreeState::SliderAttachment rateAtt;
        juce::AudioProcessorValueTreeState::SliderAttachment depthAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusSection)
};