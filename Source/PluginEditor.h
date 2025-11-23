#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
class LFOVisualizer : public juce::Component, public juce::Timer
{
public:
    LFOVisualizer(FlarksiesisAudioProcessor& p) : processor(p)
    {
        // Don't start timer in constructor - wait for component to be fully initialized
    }
    
    ~LFOVisualizer() override
    {
        stopTimer();
    }
    
    void visibilityChanged() override
    {
        if (isVisible())
            startTimerHz(30);
        else
            stopTimer();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF1A1A1A));
        
        // Draw waveform
        juce::Path path;
        bool firstPoint = true;
        
        int waveformIndex = static_cast<int>(*processor.getAPVTS().getRawParameterValue("waveform"));
        auto waveform = static_cast<FlarksiesisAudioProcessor::LFOWaveform>(waveformIndex);
        
        for (int x = 0; x < getWidth(); ++x)
        {
            float phase = static_cast<float>(x) / getWidth();
            float value = 0.0f;
            
            // Simple LFO generation for visualization
            switch (waveform)
            {
                case FlarksiesisAudioProcessor::LFOWaveform::Sine:
                    value = std::sin(phase * juce::MathConstants<float>::twoPi);
                    break;
                case FlarksiesisAudioProcessor::LFOWaveform::Triangle:
                {
                    float t = phase * 4.0f;
                    if (t < 1.0f) value = t;
                    else if (t < 3.0f) value = 2.0f - t;
                    else value = t - 4.0f;
                    break;
                }
                case FlarksiesisAudioProcessor::LFOWaveform::Square:
                    value = phase < 0.5f ? 1.0f : -1.0f;
                    break;
                case FlarksiesisAudioProcessor::LFOWaveform::Saw:
                    value = 2.0f * phase - 1.0f;
                    break;
                case FlarksiesisAudioProcessor::LFOWaveform::Random:
                {
                    // Use deterministic pseudo-random for visualization
                    // Avoid getSystemRandom() in paint - not thread safe
                    juce::Random r(static_cast<int64>(phase * 1000000));
                    value = std::sin(phase * juce::MathConstants<float>::twoPi * 8.0f) * 
                            (r.nextFloat() * 0.3f + 0.7f);
                    break;
                }
            }
            
            float y = getHeight() * 0.5f * (1.0f - value * 0.8f);
            
            if (firstPoint)
            {
                path.startNewSubPath(static_cast<float>(x), y);
                firstPoint = false;
            }
            else
            {
                path.lineTo(static_cast<float>(x), y);
            }
        }
        
        g.setColour(juce::Colour(0xFFFF6600));
        g.strokePath(path, juce::PathStrokeType(2.0f));
        
        // Draw border
        g.setColour(juce::Colour(0xFFFF6600));
        g.drawRect(getLocalBounds(), 1);
    }

    void timerCallback() override
    {
        repaint();
    }

private:
    FlarksiesisAudioProcessor& processor;
};

//==============================================================================
class FlarksiesisAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    FlarksiesisAudioProcessorEditor(FlarksiesisAudioProcessor&);
    ~FlarksiesisAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    FlarksiesisAudioProcessor& audioProcessor;

    // UI Components
    juce::Slider rateSlider, depthSlider, frequencySlider, resonanceSlider;
    juce::Slider mixSlider, stereoWidthSlider, feedbackSlider;
    juce::ComboBox waveformBox, filterTypeBox;
    juce::ToggleButton tempoSyncButton;
    
    juce::Label rateLabel, depthLabel, waveformLabel, tempoSyncLabel;
    juce::Label frequencyLabel, resonanceLabel, filterTypeLabel;
    juce::Label mixLabel, stereoWidthLabel, feedbackLabel;
    juce::Label titleLabel;
    
    LFOVisualizer lfoVisualizer;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stereoWidthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tempoSyncAttachment;

    void setupLabel(juce::Label& label, const juce::String& text);
    void setupSlider(juce::Slider& slider, juce::Slider::SliderStyle style);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlarksiesisAudioProcessorEditor)
};
