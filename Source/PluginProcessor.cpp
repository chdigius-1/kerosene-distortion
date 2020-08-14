/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionPluginAudioProcessor::DistortionPluginAudioProcessor()
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
    //add drive controls
    state = new AudioProcessorValueTreeState(*this, nullptr);
    state->createAndAddParameter("drive", "Drive", "Drive", NormalisableRange<float>(0.00, 1.0, 0.0001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("range", "Range", "Range", NormalisableRange<float>(0.01, 3000.0, 0.0001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("blend", "Blend", "Blend", NormalisableRange<float>(0.01, 0.7, 0.001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("volume", "Volume", "Volume", NormalisableRange<float>(0.0, 3.0, 0.001), 1.0, nullptr, nullptr);
    state->createAndAddParameter("density", "Density", "Density", NormalisableRange<float>(1, 100, .01), 1.0, nullptr, nullptr);
    state->createAndAddParameter("algorithm", "Algorithm", "Algorithm", NormalisableRange<float>(1, 7 , 1.0), 1.0, nullptr, nullptr);
    
    state->state = ValueTree("drive");
    state->state = ValueTree("range");
    state->state = ValueTree("blend");
    state->state = ValueTree("volume");
    state->state = ValueTree("density");
    state->state = ValueTree("algorithm");
    
    NormalisableRange<float> cutoffRange(20.0f, 10000.0f);
    NormalisableRange<float> resRange(1.0f, 25.0f);
    NormalisableRange<float> filterTypeRange(1, 3, 1);
    
    state->createAndAddParameter("filtertype", "FilterType", "FilterType", filterTypeRange,1 , nullptr, nullptr);
    state->createAndAddParameter("cutoff", "Cutoff", "Cutoff", cutoffRange, 10000.0f , nullptr, nullptr);
    state->createAndAddParameter("resonance", "Resonance", "Resonance", resRange, 1.0f, nullptr, nullptr);

    //add filter controls
    state -> state = ValueTree("filtertype");
    state -> state = ValueTree("cutoff");
    state -> state = ValueTree("resonance");
}

DistortionPluginAudioProcessor::~DistortionPluginAudioProcessor()
{
}

//==============================================================================
const String DistortionPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String DistortionPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DistortionPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    lastSampleRate = sampleRate;
    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumInputChannels();
    
    stateVariableFilter.reset();
    stateVariableFilter.prepare(spec);
    updateFilter();
}

//void DistortionPluginAudioProcessor::updateFilter(){
//    
//}

void DistortionPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DistortionPluginAudioProcessor::updateFilter(){
    int filterType = *state->getRawParameterValue("filtertype");
    int cutoff = *state->getRawParameterValue("cutoff");
    int res = *state->getRawParameterValue("resonance");
    
    if (filterType == 1) {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);
    }
    
    if (filterType == 2) {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);
    }
    
    if (filterType == 3) {
        stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, res);
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DistortionPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    float drive = *state->getRawParameterValue("drive");
    float range = *state->getRawParameterValue("range");
    float blend = *state->getRawParameterValue("blend");
    float volume = *state->getRawParameterValue("volume");
    float density = *state->getRawParameterValue("density");
    float algorithm = *state->getRawParameterValue("algorithm");
    float origDensity = density;
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++){
            
        // TODO - CJD - want to be able to turn distortion section on and off
            
            float cleanSig = *channelData;

            //TODO CJD - Tweak drive/range/density values for each algorithm for the best sounds
            //TODO CJD - More algorithms??
            //TODO CJD - play with range values for density - there is a best range for each equation
            *channelData *= drive * range;
            
            if(algorithm == 1){
                *channelData *= ((drive/2 * range/300.0));
                density = 99.9 - origDensity; //invert the range for this equation
                *channelData = ((((2.0 / float_Pi) * (atan(*channelData*density)) * sin(*channelData/density)) * blend) + ((cleanSig * (1.0 / blend))/2)) * volume;
//                *channelData = ((((2.0 / float_Pi) * (atan(pow(*channelData,density)) * sin(*channelData)) * blend) + (cleanSig * (1.0 / blend)))/2) * volume;
            } else if(algorithm == 2){
                *channelData = ((((2.0 / float_Pi) * atan(*channelData) * blend) + (cleanSig * (1.0 / blend)))/2) * volume; // the basic distortion algorithm
            } else if(algorithm == 3){
                *channelData *= ((drive/2 * range/300.0));
                density = 99.9 - origDensity; //invert the range for this equation
                *channelData = ((atan(((9 * *channelData)/density) - ((27 * pow(*channelData,3))/(density*25.0)))*blend) + (cleanSig * (1.0/blend)/2)) * (volume);
//                *channelData = atan(((9 * *channelData)/density) - ((27 * pow(*channelData,3))/(density*25))) * (volume/2);
            } else if(algorithm == 4){
                *channelData *= ((drive/2 * range/300.0));
                density = origDensity / 20.00;
                *channelData = ((tanh(cos(*channelData)/pow(density,*channelData))*blend) + (cleanSig * (1.0/blend)/2)) * volume;
            } else if (algorithm == 5){
                *channelData *= ((drive/2 * range/300.0));
                *channelData = ((((sin(*channelData) - ((1/(density*2))*sin(*channelData*2)) + (1/(density*3)) * sin(*channelData*3)))*blend) + (cleanSig * (1.0/blend)/2)) * volume;
            } else if (algorithm == 6){
                *channelData = ((tanh(density * *channelData)*blend) + (cleanSig * (1.0/blend)/2)) * volume;
            } else {
                *channelData = ((((2.0 / float_Pi) * cos(*channelData / tan(origDensity))) * blend) + (cleanSig * (1.0/blend)/2)) * volume;
            }
            channelData++;
        }
    }
    
    updateFilter();
    dsp::AudioBlock<float> block(buffer);
    stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));

}

AudioProcessorValueTreeState& DistortionPluginAudioProcessor::getState(){
    return *state;
}

//==============================================================================
bool DistortionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DistortionPluginAudioProcessor::createEditor()
{
    return new DistortionPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);
}

void DistortionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
    
    if(tree.isValid()) {
        state->state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionPluginAudioProcessor();
}
