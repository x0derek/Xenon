#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Voice.h"

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    Xenon - PluginProcessor.cpp
    Audio Logic Implementation
  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

class SimpleSound : public juce::SynthesiserSound
{
    public:
    bool appliesToNote(int) override {return true;}
    bool appliesToChannel(int) override {return true;}
};

juce::AudioProcessorValueTreeState::ParameterLayout XenonAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // TUNE
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "tune", 1 }, "TUNE",
        juce::NormalisableRange<float>(300.0f, 500.0f, 0.1f, 1.0f), 440.0f));

    // ADSR
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "attack", 1 }, "ATTACK",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.01f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "decay", 1 }, "DECAY",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.01f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "sustain", 1 }, "SUSTAIN",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.07f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "release", 1 }, "RELEASE",
        juce::NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.3f), 0.01f));

    // OSCILLATOR
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "waveType", 1 }, "WAVE TYPE",
        juce::StringArray { "SINE", "SAW", "SQUARE", "TRIANGLE" }, 0));
    
    // FILTER
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "filterCutoff", 1 }, "CUTOFF",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 8000.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "filterResonance", 1 }, "RESONANCE",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 0.7f));

    // REVERB
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "reverbSize", 1 }, "SIZE",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.3f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "reverbMix", 1 }, "MIX",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.2f));

    // CHORUS
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "chorusRate", 1 }, "RATE",
        juce::NormalisableRange<float>(0.1f, 5.0f, 0.1f, 1.0f), 1.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "chorusDepth", 1 }, "DEPTH",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f, 1.0f), 0.3f));

    // MASTER
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "gain", 1 }, "GAIN",
        juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "pitch", 1 }, "PITCH",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 1.0f), 0.0f));

    return layout;
}

XenonAudioProcessor::XenonAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor(BusesProperties()
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
#else
    :
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    synth.clearVoices();
    for (int i = 0; i < 8; i++)
        synth.addVoice(new Voice());

    synth.clearSounds();
    synth.addSound(new SimpleSound());
}

XenonAudioProcessor::~XenonAudioProcessor() {}

void XenonAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sampleRate);
    reverb.setSampleRate(sampleRate);
    reverb.reset();
}

void XenonAudioProcessor::releaseResources()
{
    synth.allNotesOff(0, false);
}

bool XenonAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void XenonAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    float tune = apvts.getRawParameterValue("tune") -> load();
    float attack = apvts.getRawParameterValue("attack") -> load();
    float decay = apvts.getRawParameterValue("decay") -> load();
    float sustain = apvts.getRawParameterValue("sustain") -> load();
    float release = apvts.getRawParameterValue("release") -> load();
    int wave = apvts.getRawParameterValue("waveType") -> load();
    float cutoff = apvts.getRawParameterValue("filterCutoff") -> load();
    float reson = apvts.getRawParameterValue("filterResonance") -> load();
    float rvbSize = apvts.getRawParameterValue("reverbSize") -> load();
    float rvbMix = apvts.getRawParameterValue("reverbMix") -> load();
    float chorusRate = apvts.getRawParameterValue("chorusRate") -> load();
    float chorusDepth = apvts.getRawParameterValue("chorusDepth") -> load();
    float gain = apvts.getRawParameterValue("gain") -> load();
    float pitch = apvts.getRawParameterValue("pitch") -> load();

    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<Voice*>(synth.getVoice(i)))
        {
            voice->setTune(tune);
            voice->setADSRParameters(attack, decay, sustain, release);
            voice->setWaveType(wave);
            voice->setFilterParameters(cutoff, reson);
            voice->setChorusParameters(chorusRate, chorusDepth);
            voice->setPitchSemitones(pitch);
        }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    //REVERB
    reverbParams.roomSize = rvbSize;
    reverbParams.wetLevel = rvbMix;
    reverbParams.dryLevel = 1.0f - rvbMix * 0.5f;
    reverbParams.damping = 0.5f;
    reverbParams.width = 1.0f;
    reverb.setParameters(reverbParams);

    if(buffer.getNumChannels() >= 2)
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    else
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());

    //GAIN
    buffer.applyGain(gain);
}

void XenonAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void XenonAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* XenonAudioProcessor::createEditor()
{
    return new XenonAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new XenonAudioProcessor();
}