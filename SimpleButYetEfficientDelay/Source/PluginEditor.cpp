/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SimpleButYetEfficientDelayAudioProcessorEditor::SimpleButYetEfficientDelayAudioProcessorEditor (SimpleButYetEfficientDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    setSize (200*3, 200);
    m_timeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_timeKnob.setRange(0., 5., 0.001); // delay time in sec.
    m_timeKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    m_timeKnob.setValue(1.);
    addAndMakeVisible(m_timeKnob);
    m_timeKnob.addListener(this);

    m_feedbackKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_feedbackKnob.setRange(0., 0.999, 0.001); // Feedback value in [0,1[
    m_feedbackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    m_feedbackKnob.setValue(0.);
    addAndMakeVisible(m_feedbackKnob);
    m_feedbackKnob.addListener(this);

    m_dryWetKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_dryWetKnob.setRange(0., 1., 0.001); 
    m_dryWetKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    m_dryWetKnob.setValue(1.);
    addAndMakeVisible(m_dryWetKnob);
    m_dryWetKnob.addListener(this);
}

SimpleButYetEfficientDelayAudioProcessorEditor::~SimpleButYetEfficientDelayAudioProcessorEditor()
{
}

//==============================================================================
void SimpleButYetEfficientDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SimpleButYetEfficientDelayAudioProcessorEditor::resized()
{
    auto border = 4;
    auto area0 = getLocalBounds();
    auto area1 = getLocalBounds();
    auto area2 = getLocalBounds();
    int width = area0.getWidth();
    int height = area0.getHeight();
    //auto dialArea = area.removeFromTop(area.getHeight());
    m_timeKnob.setBounds(area0.removeFromLeft(width*1/3));
    area1.setWidth(width / 3);
    area1.setCentre(width / 2, height / 2);
    m_feedbackKnob.setBounds(area1);
    m_dryWetKnob.setBounds(area2.removeFromRight(width * 1 / 3));
}

void SimpleButYetEfficientDelayAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &m_dryWetKnob)
    {
        processor.setDryWet(this->m_dryWetKnob.getValue());
    }
    if (slider == &m_feedbackKnob)
    {
        processor.setFeedback(m_feedbackKnob.getValue());
    }
    if (slider == &m_timeKnob)
    {
        processor.setDelay(m_timeKnob.getValue());
    }
}