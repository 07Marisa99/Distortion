/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
enum DistortionType {
    OVERDRIVE,
    DEEPFRY,
    SINE,
    DESTROY,
    UNNAMED,
    UNNAMED2,
    NOISY_CIRCUIT,
};
class DistortionAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistortionAudioProcessor();
    ~DistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    float doDistortion(float input);
    float doOverdrive(float input);
    float doDeepFry(float input);
    float doSine(float input);
    float doDestroy(float input);
    float doUnnamedDistortion(float input);
    float doUnnamedDistortionTwo(float input);
    float doNoisyCircuit(float input);

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

    DistortionType distortionType = OVERDRIVE;
    float distortionAmountValue;
    float preGainValue;
    float postGainValue;
private:
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionAudioProcessor)
};
