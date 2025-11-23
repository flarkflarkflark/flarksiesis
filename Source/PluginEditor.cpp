#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlarksiesisAudioProcessorEditor::FlarksiesisAudioProcessorEditor(FlarksiesisAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), lfoVisualizer(p)
{
    // Set colors
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFF6600));
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFFF6600));
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colour(0xFF333333));
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colour(0xFFFF6600));
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF1A1A1A));
    getLookAndFeel().setColour(juce::ComboBox::textColourId, juce::Colour(0xFFFF6600));
    getLookAndFeel().setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFFFF6600));
    getLookAndFeel().setColour(juce::ToggleButton::textColourId, juce::Colour(0xFFFF6600));
    getLookAndFeel().setColour(juce::ToggleButton::tickColourId, juce::Colour(0xFFFF6600));

    // Title
    setupLabel(titleLabel, "FLARKSIESIS v2.0");
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    // LFO Section
    setupLabel(rateLabel, "Rate");
    setupLabel(depthLabel, "Depth");
    setupLabel(waveformLabel, "Waveform");
    setupLabel(tempoSyncLabel, "Tempo Sync");

    setupSlider(rateSlider, juce::Slider::RotaryHorizontalVerticalDrag);
    setupSlider(depthSlider, juce::Slider::RotaryHorizontalVerticalDrag);
    
    addAndMakeVisible(waveformBox);
    waveformBox.addItem("Sine", 1);
    waveformBox.addItem("Triangle", 2);
    waveformBox.addItem("Square", 3);
    waveformBox.addItem("Saw", 4);
    waveformBox.addItem("Random", 5);
    
    addAndMakeVisible(tempoSyncButton);
    tempoSyncButton.setButtonText("Tempo Sync");

    // Filter Section
    setupLabel(frequencyLabel, "Frequency");
    setupLabel(resonanceLabel, "Resonance");
    setupLabel(filterTypeLabel, "Filter Type");

    setupSlider(frequencySlider, juce::Slider::RotaryHorizontalVerticalDrag);
    setupSlider(resonanceSlider, juce::Slider::RotaryHorizontalVerticalDrag);
    
    addAndMakeVisible(filterTypeBox);
    filterTypeBox.addItem("Lowpass", 1);
    filterTypeBox.addItem("Highpass", 2);
    filterTypeBox.addItem("Bandpass", 3);
    filterTypeBox.addItem("Notch", 4);
    filterTypeBox.addItem("Allpass", 5);
    filterTypeBox.addItem("LP 12dB", 6);
    filterTypeBox.addItem("HP 12dB", 7);
    filterTypeBox.addItem("BP 12dB", 8);

    // Mix Section
    setupLabel(mixLabel, "Mix");
    setupLabel(stereoWidthLabel, "Width");
    setupLabel(feedbackLabel, "Feedback");

    setupSlider(mixSlider, juce::Slider::RotaryHorizontalVerticalDrag);
    setupSlider(stereoWidthSlider, juce::Slider::RotaryHorizontalVerticalDrag);
    setupSlider(feedbackSlider, juce::Slider::RotaryHorizontalVerticalDrag);

    // LFO Visualizer
    addAndMakeVisible(lfoVisualizer);

    // Create attachments
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "rate", rateSlider);
    depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "depth", depthSlider);
    frequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "frequency", frequencySlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "resonance", resonanceSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "mix", mixSlider);
    stereoWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "stereowidth", stereoWidthSlider);
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "feedback", feedbackSlider);
    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "waveform", waveformBox);
    filterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "filtertype", filterTypeBox);
    tempoSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "temposync", tempoSyncButton);

    setSize(800, 600);
    setResizable(true, true);
    setResizeLimits(600, 450, 1200, 900);
}

FlarksiesisAudioProcessorEditor::~FlarksiesisAudioProcessorEditor()
{
}

//==============================================================================
void FlarksiesisAudioProcessorEditor::setupLabel(juce::Label& label, const juce::String& text)
{
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colour(0xFFFF6600));
    addAndMakeVisible(label);
}

void FlarksiesisAudioProcessorEditor::setupSlider(juce::Slider& slider, juce::Slider::SliderStyle style)
{
    slider.setSliderStyle(style);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFFF6600));
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF1A1A1A));
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFFFF6600));
    addAndMakeVisible(slider);
}

void FlarksiesisAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xFF000000));
    
    // Draw sections
    g.setColour(juce::Colour(0xFF1A1A1A));
    g.fillRoundedRectangle(10, 60, getWidth() - 20, 180, 5);
    g.fillRoundedRectangle(10, 250, getWidth() - 20, 180, 5);
    g.fillRoundedRectangle(10, 440, getWidth() - 20, 140, 5);
    
    // Draw section outlines
    g.setColour(juce::Colour(0xFFFF6600));
    g.drawRoundedRectangle(10, 60, getWidth() - 20, 180, 5, 2);
    g.drawRoundedRectangle(10, 250, getWidth() - 20, 180, 5, 2);
    g.drawRoundedRectangle(10, 440, getWidth() - 20, 140, 5, 2);
    
    // Section labels
    g.setFont(juce::Font(16.0f, juce::Font::bold));
    g.drawText("LFO", 20, 65, 100, 20, juce::Justification::left);
    g.drawText("FILTER", 20, 255, 100, 20, juce::Justification::left);
    g.drawText("OUTPUT", 20, 445, 100, 20, juce::Justification::left);
    
    // flarkAUDIO branding
    g.setFont(juce::Font(12.0f));
    g.setColour(juce::Colour(0x88FF6600));
    g.drawText("flarkAUDIO", getWidth() - 90, getHeight() - 25, 80, 20, juce::Justification::right);
}

void FlarksiesisAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    // Title
    titleLabel.setBounds(area.removeFromTop(50).reduced(10));
    
    // LFO Section
    auto lfoArea = area.removeFromTop(180).reduced(15);
    lfoArea.removeFromTop(25); // Skip section label
    
    auto lfoControlsArea = lfoArea.removeFromLeft(lfoArea.getWidth() / 2);
    
    auto rateArea = lfoControlsArea.removeFromLeft(lfoControlsArea.getWidth() / 2);
    rateLabel.setBounds(rateArea.removeFromTop(20));
    rateSlider.setBounds(rateArea.reduced(5));
    
    depthLabel.setBounds(lfoControlsArea.removeFromTop(20));
    depthSlider.setBounds(lfoControlsArea.reduced(5));
    
    auto lfoOptionsArea = lfoArea;
    waveformLabel.setBounds(lfoOptionsArea.removeFromTop(20));
    waveformBox.setBounds(lfoOptionsArea.removeFromTop(30).reduced(10, 0));
    lfoOptionsArea.removeFromTop(10);
    tempoSyncLabel.setBounds(lfoOptionsArea.removeFromTop(20));
    tempoSyncButton.setBounds(lfoOptionsArea.removeFromTop(30).reduced(10, 0));
    
    // Visualizer
    lfoVisualizer.setBounds(lfoOptionsArea.reduced(10));
    
    area.removeFromTop(10);
    
    // Filter Section
    auto filterArea = area.removeFromTop(180).reduced(15);
    filterArea.removeFromTop(25);
    
    auto freqArea = filterArea.removeFromLeft(filterArea.getWidth() / 3);
    frequencyLabel.setBounds(freqArea.removeFromTop(20));
    frequencySlider.setBounds(freqArea.reduced(5));
    
    auto resArea = filterArea.removeFromLeft(filterArea.getWidth() / 2);
    resonanceLabel.setBounds(resArea.removeFromTop(20));
    resonanceSlider.setBounds(resArea.reduced(5));
    
    filterTypeLabel.setBounds(filterArea.removeFromTop(20));
    filterTypeBox.setBounds(filterArea.removeFromTop(30).reduced(10, 0));
    
    area.removeFromTop(10);
    
    // Output Section
    auto outputArea = area.removeFromTop(140).reduced(15);
    outputArea.removeFromTop(25);
    
    auto mixArea = outputArea.removeFromLeft(outputArea.getWidth() / 3);
    mixLabel.setBounds(mixArea.removeFromTop(20));
    mixSlider.setBounds(mixArea.reduced(5));
    
    auto widthArea = outputArea.removeFromLeft(outputArea.getWidth() / 2);
    stereoWidthLabel.setBounds(widthArea.removeFromTop(20));
    stereoWidthSlider.setBounds(widthArea.reduced(5));
    
    feedbackLabel.setBounds(outputArea.removeFromTop(20));
    feedbackSlider.setBounds(outputArea.reduced(5));
}
