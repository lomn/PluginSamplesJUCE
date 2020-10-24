#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TresTresMolloAudioProcessorEditor::TresTresMolloAudioProcessorEditor (TresTresMolloAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (300, 150);
    depthKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    depthKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    depthKnob.setRange(0.0, 1.0, 0.01);
    depthKnob.setValue(1.0);
    addAndMakeVisible(depthKnob);
    depthKnob.addListener(this);

    freqKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    freqKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    freqKnob.setRange(0.01, 10.0, 0.01);
    freqKnob.setValue(1.0);
    addAndMakeVisible(freqKnob);
    freqKnob.addListener(this);
}

TresTresMolloAudioProcessorEditor::~TresTresMolloAudioProcessorEditor()
{
}

//==============================================================================
void TresTresMolloAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::darkred);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    //g.drawFittedText ("[TresTresMollo]", getLocalBounds(), Justification::topLeft, 1);
}

void TresTresMolloAudioProcessorEditor::resized()
{
    auto border = 4;
    auto area = getLocalBounds();
    auto dialArea = area.removeFromTop(area.getHeight() / 2);
    freqKnob.setBounds(dialArea.removeFromLeft(dialArea.getWidth() / 2));
    depthKnob.setBounds(dialArea);
}

void TresTresMolloAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &depthKnob)
    {
        //depth modification code
        processor.depth = depthKnob.getValue();
    }
    if (slider == &freqKnob)
    {
        //frequency change code
        processor.freq = freqKnob.getValue();
    }
}