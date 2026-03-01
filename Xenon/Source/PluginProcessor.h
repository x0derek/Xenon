#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    Xenon - PluginProcessor.h
    Main Audio Processor Header
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

class XenonAudioProcessor : public juce::AudioProcessor
{
  public:
    XenonAudioProcessor();
    ~XenonAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

  private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::Synthesiser synth;

    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XenonAudioProcessor)
};