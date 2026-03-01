#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - EnvelopeSection.h
    UI Component for ADSR Envelope
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class EnvelopeSection : public juce::Component
{
    public:
        EnvelopeSection(juce::AudioProcessorValueTreeState& apvts)
            : attackAtt(apvts, "attack", attackKnob),
            decayAtt(apvts, "decay", decayKnob),
            sustainAtt(apvts, "sustain", sustainKnob),
            releaseAtt(apvts, "release", releaseKnob)
        {
            auto setup = [&](juce::Slider& s)
            {
                s.setSliderStyle(juce::Slider::RotaryVerticalDrag);
                s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
                addAndMakeVisible(s);
            };

            setup(attackKnob);
            setup(decayKnob);
            setup(sustainKnob);
            setup(releaseKnob);

            auto setupLabel = [&](juce::Label& l, const juce::String& t)
            {
                l.setText(t, juce::dontSendNotification);
                l.setJustificationType(juce::Justification::centred);
                l.setFont(juce::FontOptions(11.0f));
                addAndMakeVisible(l);
            };
            setupLabel(attackLabel, "ATTACK");
            setupLabel(decayLabel, "DECAY");
            setupLabel(sustainLabel, "SUSTAIN");
            setupLabel(releaseLabel, "RELEASE");
        }

        void resized() override
        {
            const int labelH = 15;
            const int knobH = 90;
            const int knobY = 24;
            const int colW = getWidth() / 4;

            auto place = [&](juce::Slider& knob, juce::Label& label, int col)
            {
                int x = col * colW;
                knob.setBounds(x, knobY, colW, knobH);
                label.setBounds(x, knobY + knobH, colW, labelH);
            };

            place(attackKnob, attackLabel, 0);
            place(decayKnob, decayLabel, 1);
            place(sustainKnob, sustainLabel, 2);
            place(releaseKnob, releaseLabel, 3);
        }
    
    private:
        juce::Slider attackKnob, decayKnob, sustainKnob, releaseKnob;
        juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

        juce::AudioProcessorValueTreeState::SliderAttachment attackAtt, decayAtt, sustainAtt, releaseAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeSection)
};