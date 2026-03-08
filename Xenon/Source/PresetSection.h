#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - PresetSection.h
    UI Component for Preset Settings
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class PresetSection : public juce::Component
{
    public:
        PresetSection(XenonAudioProcessor& p) : processor(p)
        {
            for (int i = 0; i < (int)presets.size(); ++i)
                presetBox.addItem(presets[i].name, i + 1);
            
            presetBox.setSelectedId(1);
            addAndMakeVisible(presetBox);

            loadButton.setButtonText("LOAD");
            loadButton.onClick = [this]()
            {
                int selected = presetBox.getSelectedId() - 1;
                processor.loadPreset(selected);
            };
            addAndMakeVisible(loadButton);
        }

        void resized() override
        {
            presetBox.setBounds(10, 30, getWidth() - 20, 28);
            loadButton.setBounds(getWidth()/2 - 40, 65, 80, 24);
        }

    private:
        XenonAudioProcessor& processor;
        juce::ComboBox presetBox;
        juce::TextButton loadButton;
        juce::Label presetLabel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetSection)
};