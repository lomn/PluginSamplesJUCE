#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TresTresMolloAudioProcessor::TresTresMolloAudioProcessor()
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
    phase = 0.;
}

TresTresMolloAudioProcessor::~TresTresMolloAudioProcessor()
{
}

//==============================================================================
const String TresTresMolloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TresTresMolloAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TresTresMolloAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TresTresMolloAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TresTresMolloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TresTresMolloAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TresTresMolloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TresTresMolloAudioProcessor::setCurrentProgram (int index)
{
}

const String TresTresMolloAudioProcessor::getProgramName (int index)
{
    return {};
}

void TresTresMolloAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TresTresMolloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    inverseSampleRate = 1.0 / (float)sampleRate;
    phase = 0.;
    two_pi = 2.0 * float_Pi;
    depth = 1.;
    freq = 0.1;
}

void TresTresMolloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

float TresTresMolloAudioProcessor::modulation(float x, int waveform)
{
    switch (waveform)
    {
    case 0:
        return cos(x);
        break;
    case 1:
        if (x <= float_Pi)
        {
            return ((x/float_Pi)-0.5)*2.0;
        }
        else
        {
            return (((two_pi - x)/float_Pi)-0.5)*2.0;
        }
        break;
    default:
        return 0;
    }

    return 0;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TresTresMolloAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TresTresMolloAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    int nsamp = buffer.getNumSamples();
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    float mod = 0.;

    for (int i = 0; i < nsamp; i++)
    {
        mod = (1. + depth * modulation(phase, 1)) / 2.;
        //modulation = cos(phase);
        channelDataL[i] *= mod;
        channelDataR[i] *= mod;

        phase += freq * inverseSampleRate * two_pi;
    }

    if (phase >= two_pi)
    {
        phase -= two_pi;
    }
}

//==============================================================================
bool TresTresMolloAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TresTresMolloAudioProcessor::createEditor()
{
    return new TresTresMolloAudioProcessorEditor (*this);
}

//==============================================================================
void TresTresMolloAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TresTresMolloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TresTresMolloAudioProcessor();
}
