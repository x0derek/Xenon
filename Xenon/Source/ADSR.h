#pragma once
#include <JuceHeader.h>

/*
  -=-=-=-=-=-=-=-=-=-=-=-=-=
    Xenon - ADSR.h
    ADSR Envelope Generator
  -=-=-=-=-=-=-=-=-=-=-=-=-=
*/

class ADSR : public juce::SynthesiserVoice
{
    public:

        enum class WaveType { SINE = 0, SAW = 1, SQUARE = 2, TRIANGLE = 3 };

        void setADSRParameters(float attack, float decay, float sustain, float release)
        {
            juce::ADSR::Parameters params;
            params.attack = attack;
            params.decay = decay;
            params.sustain = sustain;
            params.release = release;
            adsr.setParameters(params);
        }

        void setWaveType(int type)
        {
            waveType = static_cast<WaveType>(juce::jlimit(0, 3, type));
        }

        void setFilterParameters(float cutoffHz, float resonance)
        {
            filterCutoff = cutoffHz;
            filterResonance = resonance;
            updateFilter();
        }

        //PITCH
        void setPitchSemitones(float semitones)
        {
            pitchSemitones = semitones;
        }

        bool canPlaySound(juce::SynthesiserSound* sound) override
        {
            return sound != nullptr;
        }

        void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override
        {
            currentAngle = 0.0;
            currentNote = midiNoteNumber;
            updateAngleDelta();
            level = velocity * 0.25;
            adsr.noteOn();
            updateFilter();
        }

        void stopNote(float, bool allowTailOff) override
        {
            if (allowTailOff)
                adsr.noteOff();
            else
            {
                clearCurrentNote();
                adsr.reset();
                angleDelta = 0.0;
            }
        }

        void pitchWheelMoved(int) override {}
        void controllerMoved(int, int) override {}

        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
        {
            if (!adsr.isActive())
            {
                clearCurrentNote();
                angleDelta = 0.0;
                return;
            }

            updateAngleDelta();

            auto* leftChannel = outputBuffer.getWritePointer(0, startSample);
            auto* rightChannel = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

            for (int i = 0; i < numSamples; ++i)
            {
                float adsrGain = adsr.getNextSample();
                float sample = generateWaveSample() * level * adsrGain;

                sample = (float)filter.processSingleSampleRaw(sample);

                leftChannel[i] += sample;
                if (rightChannel != nullptr)
                    rightChannel[i] += sample;

                currentAngle += angleDelta;
                if (currentAngle >= juce::MathConstants<double>::twoPi)
                    currentAngle -= juce::MathConstants<double>::twoPi;
            }
        }

        void setCurrentPlaybackSampleRate(double newRate) override
        {
            sampleRate = newRate;
            adsr.setSampleRate(newRate);
            updateFilter();
        }

    private:

        float generateWaveSample()
        {
            switch (waveType)
            {
                case WaveType::SAW:
                    return (float)(2.0 * (currentAngle / juce::MathConstants<double>::twoPi) - 1.0);
                case WaveType::SQUARE:
                    return currentAngle < juce::MathConstants<double>::pi ? 1.0f : -1.0f;
                case WaveType::TRIANGLE:
                    return (float)(2.0 * std::abs(2.0 * (currentAngle / juce::MathConstants<double>::twoPi) - 1.0) - 1.0);
                default:
                    return (float)std::sin(currentAngle);
            }
        }

        void updateAngleDelta()
        {
            if (sampleRate <= 0.0) return;

            double freq = juce::MidiMessage::getMidiNoteInHertz(currentNote) * std::pow(2.0, (double)pitchSemitones / 12.0);
            angleDelta = freq * juce::MathConstants<double>::twoPi / sampleRate;
        }

        void updateFilter()
        {
            if (sampleRate <= 0.0) return;
            float safeCutoff = juce::jlimit(20.0f, (float)(sampleRate * 0.49), filterCutoff);
            filter.setCoefficients(
                juce::IIRCoefficients::makeLowPass(sampleRate, (double)safeCutoff, (double)filterResonance)
            );
        }

        double currentAngle = 0.0;
        double angleDelta = 0.0;
        double sampleRate = 44100.0;
        float level = 0.0f;
        int currentNote = 69;
        float pitchSemitones = 0.0f;
        WaveType waveType = WaveType::SINE;

        juce::ADSR adsr;
        juce::IIRFilter filter;
        float filterCutoff = 8000.0f;
        float filterResonance = 0.7f;
};