#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - ReverbSection.h
    UI Component for Reverb Settings
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class ReverbSection : public juce::Component
{
    public:
        ReverbSection(juce::AudioProcessorValueTreeState& apvts)
            : reverbSizeAtt(apvts, "reverbSize", reverbSizeKnob),
            reverbMixAtt(apvts, "reverbMix", reverbMixKnob)
        {
            auto setup = [&](juce::Slider& s)
            {
                s.setSliderStyle(juce::Slider::RotaryVerticalDrag);
                s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
                addAndMakeVisible(s);
            };

            setup(reverbSizeKnob);
            setup(reverbMixKnob);

            auto setupLabel = [&](juce::Label& l, const juce::String& t)
            {
                l.setText(t, juce::dontSendNotification);
                l.setJustificationType(juce::Justification::centred);
                l.setFont(juce::FontOptions(11.0f));
                addAndMakeVisible(l);
            };

            setupLabel(reverbSizeLabel, "SIZE");
            setupLabel(reverbMixLabel, "MIX");
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

            place(reverbSizeKnob, reverbSizeLabel, 0);
            place(reverbMixKnob, reverbMixLabel, 1);
        }

    private:
        juce::Slider reverbSizeKnob, reverbMixKnob;
        juce::Label reverbSizeLabel, reverbMixLabel;

        juce::AudioProcessorValueTreeState::SliderAttachment reverbSizeAtt;
        juce::AudioProcessorValueTreeState::SliderAttachment reverbMixAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbSection)
};