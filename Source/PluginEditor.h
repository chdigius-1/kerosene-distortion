/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistortionPluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor&);
    ~DistortionPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    ScopedPointer<Slider> filterCutoffKnob;
    ScopedPointer<Slider> filterResKnob;
    ScopedPointer<Slider> filterTypeKnob;
    
    ScopedPointer<Slider> driveKnob;
    ScopedPointer<Slider> rangeKnob;
    ScopedPointer<Slider> blendKnob;
    ScopedPointer<Slider> volumeKnob;
    ScopedPointer<Slider> densityKnob;
    ScopedPointer<Slider> algorithmKnob;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterTypeAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> resAttachment;
   
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> densityAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> algorithmAttachment;
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPluginAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessorEditor)
};
