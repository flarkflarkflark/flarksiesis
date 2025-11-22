#pragma once

#include <JuceHeader.h>
#include <array>
#include <cmath>

//==============================================================================
class FlarksiesisAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    FlarksiesisAudioProcessor();
    ~FlarksiesisAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    
    // LFO waveform types
    enum class LFOWaveform
    {
        Sine = 0,
        Triangle,
        Square,
        Saw,
        Random
    };

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // DSP State
    double currentSampleRate = 44100.0;
    
    // LFO State
    float lfoPhase = 0.0f;
    float lastRandomValue = 0.0f;
    float nextRandomValue = 0.0f;
    float randomInterpolation = 0.0f;
    
    // Filter State (per channel)
    struct FilterState
    {
        std::array<float, 4> z = {0.0f}; // State variables
    };
    std::array<FilterState, 2> filterStates;
    
    // Helper methods
    float generateLFO(LFOWaveform waveform, float phase);
    void processFilter(float* buffer, int numSamples, int channel, 
                      float frequency, float resonance, int filterType);
    float getEffectiveRate(float rateParam, bool tempoSync);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlarksiesisAudioProcessor)
};
