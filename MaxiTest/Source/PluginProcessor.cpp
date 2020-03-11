/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MaxiTestAudioProcessor::MaxiTestAudioProcessor()
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
	// Setup a single parameter to control cutoff
	pCutoff = new AudioParameterFloat("cutoff", "Cutoff", 100.f, 10000.f, 1000.f);
	addParameter(pCutoff);
}

MaxiTestAudioProcessor::~MaxiTestAudioProcessor()
{
}

//==============================================================================
const String MaxiTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MaxiTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MaxiTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MaxiTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MaxiTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MaxiTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MaxiTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MaxiTestAudioProcessor::setCurrentProgram (int index)
{
}

const String MaxiTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void MaxiTestAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MaxiTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// We need to inform maximilian of the processing context
	maxiSettings::setup(sampleRate, getNumInputChannels(), samplesPerBlock);
}

void MaxiTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MaxiTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MaxiTestAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Retrieve a pointer to each channel
	auto channelDataL = buffer.getWritePointer(0);
	auto channelDataR = buffer.getWritePointer(1);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
		// Store input sample in local variable
		auto inL = channelDataL[i];
		auto inR = channelDataR[i];

		// Assign filtered signal to the buffer ready to be output
		channelDataL[i] = filterLeftChannel.lopass(inL, pCutoff->get() / 10000.f);
		channelDataR[i] = filterRightChannel.lopass(inR, pCutoff->get() / 10000.f);
    }
}

//==============================================================================
bool MaxiTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MaxiTestAudioProcessor::createEditor()
{
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void MaxiTestAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MaxiTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MaxiTestAudioProcessor();
}
