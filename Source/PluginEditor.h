/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
/**
*/

class DistortionAudioProcessorEditor : public juce::AudioProcessorEditor, public Slider::Listener {
public:
    DistortionAudioProcessorEditor(DistortionAudioProcessor&);
    ~DistortionAudioProcessorEditor() override;

    //==============================================================================
    String intToHex(int value);
    int hexToInt(String value);
    String colourFromInts(int valueR, int valueG, int valueB);
    void setPrimaryColour();

    void paint(juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionAudioProcessor& audioProcessor;
    Slider postGain;
    Slider preGain;
    Slider distortionType;
    Slider distortionAmount;

    Rectangle<int> right;
    Rectangle<int> topRight;
    Rectangle<int> middleRight;

    Rectangle<int> middleLeft;
    Rectangle<int> left;
    Rectangle<int> topLeft;


    String distortionTypeText = "OVERDRIVE";
    String secondaryColour = "#FFDA03";
    String primaryColour;
    String intensityColour;
    String intensityColourBW;

    float colourMultiplier;

};