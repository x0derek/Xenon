#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    Xenon - PluginProcessor.h
    Main Audio Processor Header
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/

// PRESETS
struct Preset {
  juce::String name;
  float attack, decay, sustain, release;
  int waveType;
  float filterCutoff, filterResonance;
  float reverbSize, reverbMix;
  float chorusRate, chorusDepth;
  float gain, pitch, tune;
};
 static const std::vector<Preset> presets = {
  // name        A      D      S      R    wave  cut    res   rvbSz  rvbMx  chrR   chrD   gain  pitch  tune
  { "PAD",      1.2f,  0.8f,  0.7f,  2.5f,  0,  2000,  0.8f,  0.8f,  0.6f,  0.4f,  0.5f,  1.0f,  0.0f, 440.0f },
  { "LEAD",     0.01f, 0.2f,  0.6f,  0.15f, 1,  4000,  1.5f,  0.2f,  0.15f, 0.0f,  0.0f,  1.1f,  0.0f, 440.0f },
  { "BASS",     0.01f, 0.3f,  0.8f,  0.2f,  1,  800,   2.0f,  0.1f,  0.05f, 0.0f,  0.0f,  1.3f, -12.0f,440.0f },
  { "PLUCK",    0.001f,0.3f,  0.0f,  0.4f,  0,  5000,  1.2f,  0.3f,  0.2f,  0.0f,  0.0f,  1.0f,  0.0f, 440.0f },
  { "STRINGS",  0.8f,  0.5f,  0.8f,  1.5f,  2,  3000,  0.9f,  0.6f,  0.4f,  0.8f,  0.6f,  0.9f,  0.0f, 440.0f },
  { "AMBIENT",  2.0f,  1.5f,  0.9f,  3.5f,  0,  1500,  0.7f,  1.0f,  0.9f,  0.3f,  0.7f,  0.8f,  0.0f, 432.0f },
  { "PIANO",    0.001f,0.8f,  0.4f,  0.6f,  3,  8000,  0.5f,  0.3f,  0.2f,  0.0f,  0.0f,  1.0f,  0.0f, 440.0f },
};

class XenonAudioProcessor : public juce::AudioProcessor
{
  public:
    XenonAudioProcessor();
    ~XenonAudioProcessor() override;

    void loadPreset(int index);
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