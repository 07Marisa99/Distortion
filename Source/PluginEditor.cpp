/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p) {
    Rectangle<int> screen = Desktop::getInstance().getDisplays().getMainDisplay().userArea;

    float screenWidth = screen.getWidth() / 2;
    float screenHeight = screenWidth / 2.4;

    setSize(screenWidth, screenHeight);

    audioProcessor.distortionType = OVERDRIVE;
    
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor() {
}

String DistortionAudioProcessorEditor::intToHex(int value) {
    std::stringstream stringStream;
    stringStream << std::hex << value;
    std::string hexString(stringStream.str());
    return String(hexString);
}

int DistortionAudioProcessorEditor::hexToInt(String value) {
    int intValue;
    std::stringstream stringStream;
    stringStream << value;
    stringStream >> std::hex >> intValue;
    return intValue;
}

String DistortionAudioProcessorEditor::colourFromInts(int valueR, int valueG, int valueB) {
    String valueRString = intToHex(valueR);
    if (valueRString.length() == 1) {
        valueRString = "0" + valueRString;
    }
    String valueGString = intToHex(valueG);
    if (valueGString.length() == 1) {
        valueGString = "0" + valueGString;
    }
    String valueBString = intToHex(valueB);
    if (valueBString.length() == 1) {
        valueBString = "0" + valueBString;
    }
    return "#" + valueRString + valueGString + valueBString;
}

void DistortionAudioProcessorEditor::setPrimaryColour() {
    int rValue = hexToInt(String(secondaryColour.operator[](1) + secondaryColour.operator[](2))) / 7;
    int gValue = hexToInt(String(secondaryColour.operator[](3) + secondaryColour.operator[](4))) / 7;
    int bValue = hexToInt(String(secondaryColour.operator[](5) + secondaryColour.operator[](6))) / 7;
    primaryColour = colourFromInts(rValue, gValue, bValue);
}

void DistortionAudioProcessorEditor::sliderValueChanged(Slider *slider) {
    if (slider == &preGain) {
        audioProcessor.preGainValue = preGain.getValue();
    } else if (slider == &postGain) {
        audioProcessor.postGainValue = postGain.getValue();
    } else if (slider == &distortionAmount) {
        audioProcessor.distortionAmountValue = 1 - distortionAmount.getValue();

        colourMultiplier = distortionAmount.getValue() / 0.99;
        int rValue = 0;
        int gValue = 0;
        int bValue = 0;
        int bwValue = 191 * colourMultiplier + 64;
        if (colourMultiplier <= 0.5) {
            rValue = 255 * colourMultiplier * 2;
            gValue = 255;
        } else {
            rValue = 255;
            gValue = (255 - (255 * colourMultiplier)) * 2;
        }
        intensityColour = colourFromInts(rValue, gValue, bValue);
        intensityColourBW = colourFromInts(bwValue, bwValue, bwValue);

    } else if (slider == &distortionType) {
        repaint();

        switch ((int) distortionType.getValue()) {
        case 1:
            audioProcessor.distortionType = OVERDRIVE;
            distortionTypeText = "OVERDRIVE";
            secondaryColour = "#FFDA03";
            break;
        case 2:
            audioProcessor.distortionType = DEEPFRY;
            distortionTypeText = "DEEPFRY";
            secondaryColour = "#29F729";
            break;
        case 3:
          audioProcessor.distortionType = SINE;
            distortionTypeText = "SINE";
            secondaryColour = "#ADFEED";
            break;
        case 4:
            audioProcessor.distortionType = DESTROY;
            distortionTypeText = "DESTROY";
            secondaryColour = "#AF73AF";
            break;
        case 5:
            audioProcessor.distortionType = UNNAMED;
            distortionTypeText = "UNNAMED";
            secondaryColour = "#FFB7C5";
            break;
        case 6:
            audioProcessor.distortionType = UNNAMED2;
            distortionTypeText = "UNNAMED2";
            secondaryColour = "#DDDDDD";
            break;
        }
    }
}
//==============================================================================
void DistortionAudioProcessorEditor::paint (juce::Graphics& g) {
    setPrimaryColour();

    g.fillAll(Colour::fromString(primaryColour));
    
    distortionAmount.setColour(Slider::ColourIds::thumbColourId, Colour::fromString(secondaryColour));
    distortionAmount.setColour(Slider::ColourIds::rotarySliderFillColourId, Colour::fromString(intensityColourBW));
    distortionAmount.setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colour::fromString("#070707"));

    distortionType.setColour(Slider::ColourIds::rotarySliderFillColourId, Colour::fromString(intensityColourBW));
    distortionType.setColour(Slider::ColourIds::rotarySliderOutlineColourId, Colour::fromString("#070707"));
    distortionType.setColour(Slider::ColourIds::thumbColourId, Colour::fromString(secondaryColour));

    preGain.setColour(Slider::ColourIds::trackColourId, Colour::fromString(intensityColourBW));
    preGain.setColour(Slider::ColourIds::backgroundColourId, Colour::fromString("#070707"));
    preGain.setColour(Slider::ColourIds::thumbColourId, Colour::fromString(secondaryColour));

    postGain.setColour(Slider::ColourIds::trackColourId, Colour::fromString(intensityColourBW));
    postGain.setColour(Slider::ColourIds::backgroundColourId, Colour::fromString("#070707"));
    postGain.setColour(Slider::ColourIds::thumbColourId, Colour::fromString(secondaryColour));

    float fontSize = getLocalBounds().getWidth() / (640 / 21);

    Font font("Arial Rounded MT Bold", fontSize, 1);

    g.setColour(Colour::fromString(intensityColour));
    g.setFont(font);
    g.drawFittedText (String(int((distortionAmount.getValue() / 0.99) * 100)) + "%", middleRight, juce::Justification::centred, 1);
    g.setColour(Colour::fromString(secondaryColour));
    g.drawFittedText(distortionTypeText, middleLeft, juce::Justification::centred, 1);
}

void DistortionAudioProcessorEditor::resized()
{
    float halfWidth = getLocalBounds().getWidth() / 2;
    float widthMod = 2.5;
    
    float middleRightWidth = getLocalBounds().getWidth() / widthMod;
    float middleRightHeight = getLocalBounds().getHeight();
    
    float middleLeftWidth = getLocalBounds().getWidth() / widthMod;
    float middleLeftHeight = getLocalBounds().getHeight();
    
    float rightWidth = halfWidth - middleRightWidth;
    float rightHeight = getLocalBounds().getHeight() * 0.6;
    
    float leftWidth = halfWidth - middleLeftWidth;
    float leftHeight = getLocalBounds().getHeight() * 0.6;

    float middleRightX = halfWidth + rightWidth / 8;
    float middleRightY = 0;
    
    float middleLeftX = getLocalBounds().getWidth() - halfWidth - middleLeftWidth - leftWidth / 8;
    float middleLeftY = 0;
    
    float rightX = getLocalBounds().getWidth() - rightWidth;
    float rightY = getLocalBounds().getHeight() * 0.2;
    
    float leftX = 0;
    float leftY = getLocalBounds().getHeight() * 0.2;


    middleRight = Rectangle<int>(middleRightX, middleRightY, middleRightWidth, middleRightHeight);
    middleLeft = Rectangle<int>(middleLeftX, middleLeftY, middleLeftWidth, middleLeftHeight);
    right = Rectangle<int>(rightX, rightY, rightWidth, rightHeight);
    left = Rectangle<int>(leftX, leftY, leftWidth, leftHeight);


    postGain.setBounds(right);
    postGain.setSliderStyle(Slider::LinearVertical);
    postGain.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    postGain.setRange(0.125, 8);
    postGain.setSkewFactorFromMidPoint(1);
    postGain.setValue(1);
    postGain.addListener(this);

    addAndMakeVisible(postGain);

    preGain.setBounds(left);
    preGain.setSliderStyle(Slider::LinearVertical);
    preGain.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    preGain.setRange(0.125, 8);
    preGain.setSkewFactorFromMidPoint(1);
    preGain.setValue(1);
    preGain.addListener(this);

    addAndMakeVisible(preGain);

    distortionAmount.setBounds(middleRight);
    distortionAmount.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    distortionAmount.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    distortionAmount.setRange(0, 0.99);
    distortionAmount.setValue(distortionAmount.getMaximum() / 2);
    distortionAmount.addListener(this);

    addAndMakeVisible(distortionAmount);

    distortionType.setBounds(middleLeft);
    distortionType.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    distortionType.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    distortionType.setRange(Range<double>(1, 6), 1);
    distortionType.setValue(1);
    distortionType.addListener(this);

    addAndMakeVisible(distortionType);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}