/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace TreeNamesIDs
{
    static String decayID{ "DECAY" };
    static String decayName{ "Decay" };

    static String dryWetID{ "DRYWET" };
    static String dryWetName{ "DryWet" };

    static String gain5ID{ "GAIN5" };
    static String gain5Name{ "Gain5" };

    static String gain6ID{ "GAIN6" };
    static String gain6Name{ "Gain6" };

    static String tau1ID{ "TAU1" };
    static String tau1Name{ "tau1" };

    static String tau2ID{ "TAU2" };
    static String tau2Name{ "tau2" };

    static String tau3ID{ "TAU3" };
    static String tau3Name{ "tau3" };

    static String tau4ID{ "TAU4" };
    static String tau4Name{ "tau4" };
}

//==============================================================================
SchroederReverberatorAudioProcessor::SchroederReverberatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), mAPVTS(*this, nullptr, "PARAMETERS", this->createParameters())
#endif
{
    //this->rev = new SchroederReverberator(44100);
    m_revL = new SchroederReverberator(44100);
    m_revR = new SchroederReverberator(44100);
    m_DryWet = 1.;
    mAPVTS.addParameterListener("DECAY", this);
    mAPVTS.addParameterListener("DRYWET", this);
    
}

SchroederReverberatorAudioProcessor::~SchroederReverberatorAudioProcessor()
{
    delete m_revL;
    delete m_revR;
}

//==============================================================================
const String SchroederReverberatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SchroederReverberatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SchroederReverberatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SchroederReverberatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SchroederReverberatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SchroederReverberatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SchroederReverberatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SchroederReverberatorAudioProcessor::setCurrentProgram (int index)
{
}

const String SchroederReverberatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SchroederReverberatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SchroederReverberatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    delete m_revL;
    delete m_revR;
    m_revL = new SchroederReverberator((int) sampleRate);
    m_revR = new SchroederReverberator((int) sampleRate);
}

void SchroederReverberatorAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SchroederReverberatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SchroederReverberatorAudioProcessor::parameterChanged(const String& param, float value) 
{
    if (param.compare(TreeNamesIDs::decayID) == 0)
    {
        m_revL->setRt60((double) value);
        m_revR->setRt60((double) value);
    }
    else if (param.compare(TreeNamesIDs::dryWetID) == 0)
    {
        this->setDryWet(value); 
    }

    //if in delays[] a value of -1 is stored the corresponding delay value isn't updated
    else if (param.compare(TreeNamesIDs::tau1ID) == 0)
    {
        double delays[NB_COMBS];
        delays[0] = value;
        delays[1] = -1;
        delays[2] = -1;
        delays[3] = -1;

        m_revL->updateCombDelays(delays);
        m_revR->updateCombDelays(delays);
    }
    else if (param.compare(TreeNamesIDs::tau2ID) == 0)
    {
        double delays[NB_COMBS];
        delays[0] = -1;
        delays[1] = value;
        delays[2] = -1;
        delays[3] = -1;

        m_revL->updateCombDelays(delays);
        m_revR->updateCombDelays(delays);
    }
    else if (param.compare(TreeNamesIDs::tau1ID) == 0)
    {
        double delays[NB_COMBS];
        delays[0] = -1;
        delays[1] = -1;
        delays[2] = value;
        delays[3] = -1;

        m_revL->updateCombDelays(delays);
        m_revR->updateCombDelays(delays);
    }
    else if (param.compare(TreeNamesIDs::tau1ID) == 0)
    {
        double delays[NB_COMBS];
        delays[0] = -1;
        delays[1] = -1;
        delays[2] = -1;
        delays[3] = value;

        m_revL->updateCombDelays(delays);
        m_revR->updateCombDelays(delays);
    }
    
}

void SchroederReverberatorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    float* channelLeftW = buffer.getWritePointer(0);
    float* channelRightW = buffer.getWritePointer(1);
    const float* channelLeftR = buffer.getReadPointer(0);
    const float* channelRightR = buffer.getReadPointer(1);
    float sampleL = 0;
    float sampleR = 0;
    for(int i = 0; i < buffer.getNumSamples(); i++)
    {
        sampleL = this->m_revL->next(channelLeftR[i]);
        sampleR = this->m_revR->next(channelRightR[i]);
       
        channelLeftW[i] = sampleL * m_DryWet + channelLeftR[i] * (1 - m_DryWet);
        channelRightW[i] = sampleR * m_DryWet + channelRightR[i] * (1 - m_DryWet);
    }
}

//==============================================================================
bool SchroederReverberatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SchroederReverberatorAudioProcessor::createEditor()
{
    return new foleys::MagicPluginEditor(magicState);
}

//==============================================================================
void SchroederReverberatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SchroederReverberatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SchroederReverberatorAudioProcessor::setDryWet(float dryWet)
{
    this->m_DryWet = dryWet;
}

AudioProcessorValueTreeState::ParameterLayout SchroederReverberatorAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

    //Creation of parameters
    parameters.push_back (std::make_unique<AudioParameterFloat> (TreeNamesIDs::dryWetID, TreeNamesIDs::dryWetName, 0.0f, 1.0f, 1.f));
    parameters.push_back (std::make_unique<AudioParameterFloat> (TreeNamesIDs::decayID, TreeNamesIDs::decayName, 0.0f, 15.0f, 2.0f));

    parameters.push_back(std::make_unique<AudioParameterFloat>(TreeNamesIDs::gain5ID, TreeNamesIDs::gain5Name, 0.65f, 0.75f, 0.7f));
    parameters.push_back(std::make_unique<AudioParameterFloat>(TreeNamesIDs::gain6ID, TreeNamesIDs::gain6Name, 0.65f, 0.75f, 0.7f));

    parameters.push_back(std::make_unique<AudioParameterFloat>(TreeNamesIDs::tau1ID, TreeNamesIDs::tau1Name, NormalisableRange<float>(0.03f, 0.045f), 0.03f));
    parameters.push_back(std::make_unique<AudioParameterFloat>(TreeNamesIDs::tau2ID, TreeNamesIDs::tau2Name, NormalisableRange<float>(0.03f, 0.045f), 0.03f));
    parameters.push_back(std::make_unique<AudioParameterFloat>(TreeNamesIDs::tau3ID, TreeNamesIDs::tau3Name, NormalisableRange<float>(0.03f, 0.045f), 0.03f));
    parameters.push_back(std::make_unique<AudioParameterFloat>(TreeNamesIDs::tau4ID, TreeNamesIDs::tau4Name, NormalisableRange<float>(0.03f, 0.045f), 0.03f));

    return { parameters.begin(), parameters.end() };
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SchroederReverberatorAudioProcessor();
}
