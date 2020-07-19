/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DelayClass.h"

//==============================================================================
SimpleButYetEfficientDelayAudioProcessor::SimpleButYetEfficientDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    this->m_delayTime = 1.;
    this->m_dryWet = 1.;
    this->m_feedBack = 0.;
}

SimpleButYetEfficientDelayAudioProcessor::~SimpleButYetEfficientDelayAudioProcessor()
{
}

void SimpleButYetEfficientDelayAudioProcessor::setDelay(double delay)
{
    this->m_delayLeft->setDelay(delay);
    this->m_delayRight->setDelay(delay);
}

void SimpleButYetEfficientDelayAudioProcessor::setFeedback(double feedback)
{
    this->m_delayLeft->setFeedback(feedback);
    this->m_delayRight->setFeedback(feedback);
}

void SimpleButYetEfficientDelayAudioProcessor::setDryWet(double dryWet)
{
    this->m_dryWet = dryWet;
}

//==============================================================================
const String SimpleButYetEfficientDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleButYetEfficientDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleButYetEfficientDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleButYetEfficientDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleButYetEfficientDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleButYetEfficientDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleButYetEfficientDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleButYetEfficientDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleButYetEfficientDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleButYetEfficientDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimpleButYetEfficientDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->m_delayRight = new Delay((int)sampleRate, this->m_delayTime, this->m_feedBack);
    this->m_delayLeft = new Delay((int)sampleRate, this->m_delayTime, this->m_feedBack);

    m_delayLeft->prepareToPlay();
    m_delayRight->prepareToPlay();
}

void SimpleButYetEfficientDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    this->m_delayLeft->clearBuffer();
    this->m_delayRight->clearBuffer();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleButYetEfficientDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleButYetEfficientDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    float* channelRight = buffer.getWritePointer(0);
    float* channelLeft = buffer.getWritePointer(1);
    float val = 0;



    float dryWet = (float) this->m_dryWet;

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        val = channelRight[i];
        channelRight[i] = val * (1 - dryWet) +
             dryWet* this->m_delayRight->next(val);
        val = 0;

        val = channelLeft[i];
        channelLeft[i] = val * (1 - dryWet) +
            dryWet * this->m_delayLeft->next(val);
        val = 0;
    }
}

//==============================================================================
bool SimpleButYetEfficientDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleButYetEfficientDelayAudioProcessor::createEditor()
{
    return new SimpleButYetEfficientDelayAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleButYetEfficientDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleButYetEfficientDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleButYetEfficientDelayAudioProcessor();
}
