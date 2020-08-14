/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    //drive controls
    addAndMakeVisible(driveKnob = new Slider("Drive"));
    driveKnob->setSliderStyle(Slider::Rotary);
    driveKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(rangeKnob = new Slider("Range"));
    rangeKnob->setSliderStyle(Slider::Rotary);
    rangeKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(blendKnob = new Slider("Blend"));
    blendKnob->setSliderStyle(Slider::Rotary);
    blendKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(volumeKnob = new Slider("Volume"));
    volumeKnob->setSliderStyle(Slider::Rotary);
    volumeKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    
    addAndMakeVisible(densityKnob = new Slider("Density"));
    densityKnob->setSliderStyle(Slider::Rotary);
    densityKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    
    addAndMakeVisible(algorithmKnob = new Slider("Algorithm"));
    algorithmKnob->setSliderStyle(Slider::Rotary);
    algorithmKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    
    driveAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"drive", *driveKnob);
    rangeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"range", *rangeKnob);
    blendAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"blend", *blendKnob);
    volumeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"volume", *volumeKnob);
    densityAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"density", *densityKnob);
    algorithmAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"algorithm", *algorithmKnob);
    
//    //filter controls
//    addAndMakeVisible(filterMenu = new ComboBox("FilterMenu"));
//    filterMenu->addItem("Low Pass", 1);
//    filterMenu->addItem("High Pass", 2);
//    filterMenu->addItem("Band Pass", 3);
//    filterMenu->setJustificationType(Justification::centred);
//    filterMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "filtermenu", *filterMenu);
    
    addAndMakeVisible(filterTypeKnob = new Slider("FilterType"));
    filterTypeKnob->setSliderStyle(Slider::Rotary);
    filterTypeKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    filterTypeAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(),"filtertype", *filterTypeKnob);

    addAndMakeVisible(filterCutoffKnob = new Slider("Cutoff"));
    filterCutoffKnob->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterCutoffKnob->setRange(20.0, 10000.0);
    filterCutoffKnob->setValue(10000.0);
    filterCutoffKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    filterCutoffKnob->setSkewFactorFromMidPoint(1000.0);
    filterAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "cutoff", *filterCutoffKnob);

    addAndMakeVisible(filterResKnob = new Slider("Resonance"));
    filterResKnob->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filterResKnob->setRange(1.0, 20.0);
    filterResKnob->setValue(1);
    filterResKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    resAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "resonance", *filterResKnob);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 400);
}

DistortionPluginAudioProcessorEditor::~DistortionPluginAudioProcessorEditor()
{
}

//==============================================================================
void DistortionPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

//    g.setColour (Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("Drive",((getWidth()/7) * 1) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Range",((getWidth()/7) * 2) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Blend", ((getWidth()/7) * 3) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Volume", ((getWidth()/7) * 4) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Density", ((getWidth()/7) * 5) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Formula", ((getWidth()/7) * 6) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100, Justification::centredBottom);
    
    g.drawText("Filter Type",((getWidth()/7) * 1) - (100 / 2), (getHeight() / 4) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Cutoff",((getWidth()/7) * 2) - (100 / 2), (getHeight() / 4) - (100 / 2), 100, 100, Justification::centredBottom);
    g.drawText("Resonance",((getWidth()/7) * 3) - (100 / 2), (getHeight() / 4) - (100 / 2), 100, 100, Justification::centredBottom);
}

void DistortionPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    driveKnob->setBounds(((getWidth()/7) * 1) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100);
    rangeKnob->setBounds(((getWidth()/7) * 2) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100);
    blendKnob->setBounds(((getWidth()/7) * 3) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth()/7) * 4) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100);
    densityKnob->setBounds(((getWidth()/7) * 5) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100);
    algorithmKnob->setBounds(((getWidth()/7) * 6) - (100 / 2), (getHeight() / 1.5) - (100 / 2), 100, 100);
    
    filterTypeKnob->setBounds(((getWidth()/7) * 1) - (100 / 2), (getHeight() / 4) - (100 / 2), 100, 100);
    filterCutoffKnob->setBounds(((getWidth()/7) * 2) - (100 / 2), (getHeight() / 4) - (100 / 2), 100, 100);
    filterResKnob->setBounds(((getWidth()/7) * 3) - (100 / 2), (getHeight() / 4) - (100 / 2), 100, 100);
}
