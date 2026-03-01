#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "XenonLookAndFeel.h"
#include "EnvelopeSection.h"
#include "OscSection.h"
#include "FilterSection.h"
#include "ReverbSection.h"
#include "MasterSection.h"

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - PluginEditor.h
    Main GUI Header
  -=-=-=-=-=-=-=-=-=-=-=-=-
*/

class XenonAudioProcessorEditor : public juce::AudioProcessorEditor
{
  public:
    XenonAudioProcessorEditor(XenonAudioProcessor&);
    ~XenonAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

  private:
    XenonAudioProcessor& audioProcessor;

    XenonLookAndFeel lnf;

    EnvelopeSection envelopeSection;
    OscSection oscSection;
    FilterSection filterSection;
    ReverbSection reverbSection;
    MasterSection masterSection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XenonAudioProcessorEditor)
};