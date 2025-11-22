#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlarksiesisAudioProcessor::FlarksiesisAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#else
    :
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

FlarksiesisAudioProcessor::~FlarksiesisAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout FlarksiesisAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // LFO Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "rate", "LFO Rate", 0.01f, 20.0f, 2.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "depth", "LFO Depth", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "waveform", "Waveform", 
        juce::StringArray{"Sine", "Triangle", "Square", "Saw", "Random"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "temposync", "Tempo Sync", false));

    // Filter Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "frequency", "Filter Frequency", 
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 1000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "resonance", "Resonance", 0.1f, 10.0f, 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "filtertype", "Filter Type",
        juce::StringArray{"Lowpass", "Highpass", "Bandpass", "Notch", 
                         "Allpass", "LP 12dB", "HP 12dB", "BP 12dB"}, 0));

    // Mix Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Dry/Wet", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "stereowidth", "Stereo Width", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "feedback", "Feedback", 0.0f, 0.95f, 0.0f));

    return {params.begin(), params.end()};
}

//==============================================================================
const juce::String FlarksiesisAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlarksiesisAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool FlarksiesisAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool FlarksiesisAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double FlarksiesisAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlarksiesisAudioProcessor::getNumPrograms()
{
    return 1;
}

int FlarksiesisAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlarksiesisAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String FlarksiesisAudioProcessor::getProgramName(int index)
{
    return {};
}

void FlarksiesisAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void FlarksiesisAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    // Reset filter states
    for (auto& state : filterStates)
    {
        state.z.fill(0.0f);
    }
    
    // Reset LFO
    lfoPhase = 0.0f;
    lastRandomValue = 0.0f;
    nextRandomValue = 0.0f;
    randomInterpolation = 0.0f;
}

void FlarksiesisAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlarksiesisAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

//==============================================================================
float FlarksiesisAudioProcessor::generateLFO(LFOWaveform waveform, float phase)
{
    switch (waveform)
    {
        case LFOWaveform::Sine:
            return std::sin(phase * juce::MathConstants<float>::twoPi);
            
        case LFOWaveform::Triangle:
        {
            float t = phase * 4.0f;
            if (t < 1.0f) return t;
            if (t < 3.0f) return 2.0f - t;
            return t - 4.0f;
        }
        
        case LFOWaveform::Square:
            return phase < 0.5f ? 1.0f : -1.0f;
            
        case LFOWaveform::Saw:
            return 2.0f * phase - 1.0f;
            
        case LFOWaveform::Random:
        {
            // Smooth random interpolation
            return lastRandomValue + (nextRandomValue - lastRandomValue) * randomInterpolation;
        }
            
        default:
            return 0.0f;
    }
}

float FlarksiesisAudioProcessor::getEffectiveRate(float rateParam, bool tempoSync)
{
    if (!tempoSync)
        return rateParam;
    
    // Get tempo from host
    auto playHead = getPlayHead();
    if (playHead != nullptr)
    {
        juce::Optional<juce::AudioPlayHead::PositionInfo> posInfo = playHead->getPosition();
        if (posInfo.hasValue() && posInfo->getBpm().hasValue())
        {
            double bpm = *posInfo->getBpm();
            // Map rate parameter to musical divisions
            float division = std::pow(2.0f, std::floor(rateParam * 4.0f - 8.0f));
            return static_cast<float>((bpm / 60.0) * division);
        }
    }
    
    return rateParam;
}

void FlarksiesisAudioProcessor::processFilter(float* buffer, int numSamples, int channel,
                                              float frequency, float resonance, int filterType)
{
    auto& state = filterStates[channel];
    
    // Calculate filter coefficients
    float omega = juce::MathConstants<float>::twoPi * frequency / static_cast<float>(currentSampleRate);
    float cosOmega = std::cos(omega);
    float sinOmega = std::sin(omega);
    float alpha = sinOmega / (2.0f * resonance);
    
    float b0, b1, b2, a0, a1, a2;
    
    switch (filterType)
    {
        case 0: // Lowpass 24dB
        {
            b0 = (1.0f - cosOmega) / 2.0f;
            b1 = 1.0f - cosOmega;
            b2 = (1.0f - cosOmega) / 2.0f;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cosOmega;
            a2 = 1.0f - alpha;
            break;
        }
        case 1: // Highpass 24dB
        {
            b0 = (1.0f + cosOmega) / 2.0f;
            b1 = -(1.0f + cosOmega);
            b2 = (1.0f + cosOmega) / 2.0f;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cosOmega;
            a2 = 1.0f - alpha;
            break;
        }
        case 2: // Bandpass
        {
            b0 = alpha;
            b1 = 0.0f;
            b2 = -alpha;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cosOmega;
            a2 = 1.0f - alpha;
            break;
        }
        case 3: // Notch
        {
            b0 = 1.0f;
            b1 = -2.0f * cosOmega;
            b2 = 1.0f;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cosOmega;
            a2 = 1.0f - alpha;
            break;
        }
        case 4: // Allpass
        {
            b0 = 1.0f - alpha;
            b1 = -2.0f * cosOmega;
            b2 = 1.0f + alpha;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cosOmega;
            a2 = 1.0f - alpha;
            break;
        }
        case 5: // Lowpass 12dB
        {
            float k = std::tan(omega / 2.0f);
            float norm = 1.0f / (1.0f + k / resonance + k * k);
            b0 = k * k * norm;
            b1 = 2.0f * b0;
            b2 = b0;
            a0 = 1.0f;
            a1 = 2.0f * (k * k - 1.0f) * norm;
            a2 = (1.0f - k / resonance + k * k) * norm;
            break;
        }
        case 6: // Highpass 12dB
        {
            float k = std::tan(omega / 2.0f);
            float norm = 1.0f / (1.0f + k / resonance + k * k);
            b0 = norm;
            b1 = -2.0f * b0;
            b2 = b0;
            a0 = 1.0f;
            a1 = 2.0f * (k * k - 1.0f) * norm;
            a2 = (1.0f - k / resonance + k * k) * norm;
            break;
        }
        case 7: // Bandpass 12dB
        {
            float k = std::tan(omega / 2.0f);
            float norm = 1.0f / (1.0f + k / resonance + k * k);
            b0 = k / resonance * norm;
            b1 = 0.0f;
            b2 = -b0;
            a0 = 1.0f;
            a1 = 2.0f * (k * k - 1.0f) * norm;
            a2 = (1.0f - k / resonance + k * k) * norm;
            break;
        }
        default:
            b0 = 1.0f; b1 = 0.0f; b2 = 0.0f;
            a0 = 1.0f; a1 = 0.0f; a2 = 0.0f;
    }
    
    // Normalize coefficients
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
    
    // Process samples
    for (int i = 0; i < numSamples; ++i)
    {
        float input = buffer[i];
        float output = b0 * input + b1 * state.z[0] + b2 * state.z[1]
                                  - a1 * state.z[2] - a2 * state.z[3];
        
        // Update state
        state.z[1] = state.z[0];
        state.z[0] = input;
        state.z[3] = state.z[2];
        state.z[2] = output;
        
        buffer[i] = output;
    }
}

void FlarksiesisAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Get parameters
    float rate = getEffectiveRate(*apvts.getRawParameterValue("rate"), 
                                  *apvts.getRawParameterValue("temposync") > 0.5f);
    float depth = *apvts.getRawParameterValue("depth");
    int waveformIndex = static_cast<int>(*apvts.getRawParameterValue("waveform"));
    LFOWaveform waveform = static_cast<LFOWaveform>(waveformIndex);
    float frequency = *apvts.getRawParameterValue("frequency");
    float resonance = *apvts.getRawParameterValue("resonance");
    int filterType = static_cast<int>(*apvts.getRawParameterValue("filtertype"));
    float mix = *apvts.getRawParameterValue("mix");
    float stereoWidth = *apvts.getRawParameterValue("stereowidth");
    float feedback = *apvts.getRawParameterValue("feedback");

    const int numSamples = buffer.getNumSamples();
    float phaseIncrement = rate / static_cast<float>(currentSampleRate);

    // Create dry buffer for mixing
    juce::AudioBuffer<float> dryBuffer(totalNumInputChannels, numSamples);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
        dryBuffer.copyFrom(channel, 0, buffer, channel, 0, numSamples);

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        float localPhase = lfoPhase;
        
        // Add phase offset for stereo width
        if (channel == 1 && totalNumInputChannels == 2)
            localPhase = std::fmod(localPhase + (1.0f - stereoWidth) * 0.5f, 1.0f);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Generate LFO value
            if (waveform == LFOWaveform::Random)
            {
                // Update random values at phase reset
                if (localPhase < phaseIncrement)
                {
                    lastRandomValue = nextRandomValue;
                    nextRandomValue = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;
                    randomInterpolation = 0.0f;
                }
                randomInterpolation = localPhase / (1.0f / rate * static_cast<float>(currentSampleRate));
            }
            
            float lfoValue = generateLFO(waveform, localPhase);
            
            // Apply LFO to frequency
            float modulatedFreq = frequency * std::pow(2.0f, lfoValue * depth * 4.0f);
            modulatedFreq = juce::jlimit(20.0f, 20000.0f, modulatedFreq);
            
            // Simple one-pole filter for smooth modulation
            static float smoothedFreq = frequency;
            smoothedFreq = smoothedFreq * 0.95f + modulatedFreq * 0.05f;
            
            // Store for filtering
            float tempBuffer[1] = {channelData[sample]};
            processFilter(tempBuffer, 1, channel, smoothedFreq, resonance, filterType);
            channelData[sample] = tempBuffer[0];
            
            // Apply feedback
            if (feedback > 0.0f && sample > 0)
                channelData[sample] += channelData[sample - 1] * feedback;
            
            // Advance phase
            localPhase += phaseIncrement;
            if (localPhase >= 1.0f)
                localPhase -= 1.0f;
        }
    }

    // Update global phase
    lfoPhase += phaseIncrement * numSamples;
    if (lfoPhase >= 1.0f)
        lfoPhase -= 1.0f;

    // Mix dry and wet
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto* dryData = dryBuffer.getReadPointer(channel);
        
        for (int sample = 0; sample < numSamples; ++sample)
            channelData[sample] = dryData[sample] * (1.0f - mix) + channelData[sample] * mix;
    }
}

//==============================================================================
bool FlarksiesisAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* FlarksiesisAudioProcessor::createEditor()
{
    return new FlarksiesisAudioProcessorEditor(*this);
}

//==============================================================================
void FlarksiesisAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FlarksiesisAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlarksiesisAudioProcessor();
}
