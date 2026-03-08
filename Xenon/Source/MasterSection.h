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
            pitchAtt(apvts, "pitch", pitchSlider),
            tuneAtt(apvts, "tune", tuneSlider)
        {
            auto setup = [&](juce::Slider& s)
            {
                s.setSliderStyle(juce::Slider::LinearVertical);
                s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
                addAndMakeVisible(s);
            };

            setup(gainSlider);
            setup(pitchSlider);
            setup(tuneSlider);

            auto setupLabel = [&](juce::Label& l, const juce::String& t)
            {
                l.setText(t, juce::dontSendNotification);
                l.setJustificationType(juce::Justification::centred);
                l.setFont(juce::FontOptions(11.0f));
                addAndMakeVisible(l);
            };

            setupLabel(gainLabel, "GAIN");
            setupLabel(pitchLabel, "PITCH");
            setupLabel(tuneLabel, "TUNE");
        }

        void resized() override
        {
            const int labelH = 15;
            const int sliderH = getHeight() - labelH - 24;
            const int sliderY = 24;
            const int colW = getWidth() / 3;

            auto place = [&](juce::Slider& knob, juce::Label& label, int col)
            {
                int x = col * colW;
                knob.setBounds(x, sliderY, colW, sliderH);
                label.setBounds(x, sliderY + sliderH, colW, labelH);
            };

            place(gainSlider, gainLabel, 0);
            place(pitchSlider, pitchLabel, 1);
            place(tuneSlider, tuneLabel, 2);
        }

    private:
        juce::Slider gainSlider, pitchSlider, tuneSlider;
        juce::Label gainLabel, pitchLabel, tuneLabel;

        juce::AudioProcessorValueTreeState::SliderAttachment gainAtt, pitchAtt, tuneAtt;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterSection)
};