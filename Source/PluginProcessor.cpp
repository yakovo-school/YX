/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
YxAudioProcessor::YxAudioProcessor()
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
	addParameter(mGate = new AudioParameterFloat("gate", "Gate Percentage", 0.03, .98, 0.98));
	addParameter(mInterval = new AudioParameterInt("interval", "Interval", 1, 16, 1));
	addParameter(Y_div = new AudioParameterInt("Y_Div", "Y Interval", 1, 16, 1));
	addParameter(X_div = new AudioParameterInt("X_Div", "X Interval", 1, 16, 4));
	addParameter(mDivision = new AudioParameterInt("div", "Division", 0, 2, 0));
	addParameter(mVelo = new AudioParameterInt("velo", "Velocity", 0, 127, 100));

	addParameter(zero_zero = new AudioParameterInt("0,0", "0,0", 0, 127, 64));
	addParameter(zero_one = new AudioParameterInt("0,1", "0,1", 0, 127, 64));
	addParameter(zero_two = new AudioParameterInt("0,2", "0,2", 0, 127, 64));
	addParameter(zero_three = new AudioParameterInt("0,3", "0,3", 0, 127, 64));

	addParameter(one_zero = new AudioParameterInt("1,0", "1,0", 0, 127, 64));
	addParameter(one_one = new AudioParameterInt("1,1", "1,1", 0, 127, 64));
	addParameter(one_two = new AudioParameterInt("1,2", "1,2", 0, 127, 64));
	addParameter(one_three = new AudioParameterInt("1,3", "1,3", 0, 127, 64));

	addParameter(two_zero = new AudioParameterInt("2,0", "2,0", 0, 127, 64));
	addParameter(two_one = new AudioParameterInt("2,1", "2,1", 0, 127, 64));
	addParameter(two_two = new AudioParameterInt("2,2", "2,2", 0, 127, 64));
	addParameter(two_three = new AudioParameterInt("2,3", "2,3", 0, 127, 64));

	addParameter(three_zero = new AudioParameterInt("3,0", "3,0", 0, 127, 64));
	addParameter(three_one = new AudioParameterInt("3,1", "3,1", 0, 127, 64));
	addParameter(three_two = new AudioParameterInt("3,2", "3,2", 0, 127, 64));
	addParameter(three_three = new AudioParameterInt("3,3", "3,3", 0, 127, 64));
}

YxAudioProcessor::~YxAudioProcessor()
{
}

//==============================================================================
const String YxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool YxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool YxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool YxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double YxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int YxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int YxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void YxAudioProcessor::setCurrentProgram (int index)
{
}

const String YxAudioProcessor::getProgramName (int index)
{
    return {};
}

void YxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void YxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void YxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool YxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void YxAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midi)
{
	jassert(buffer.getNumChannels() == 0);
	buffer.clear();
	midi.clear();
	const int divisions[] = { 0, 1, 2 };

	int buff = buffer.getNumSamples(); //samples/buffer
	double rate = getSampleRate(); //samples/second
	AudioPlayHead *head = getPlayHead();
	AudioPlayHead::CurrentPositionInfo positionInfo;
	head->getCurrentPosition(positionInfo);
	mBPM = positionInfo.bpm;
	double currentPosition = positionInfo.ppqPosition; //use this as the floating ticker
	double beatsPerBuffer = buff * mBPM / 60.0 / rate;
	double div = pow(2, divisions[*mDivision]);
	nextPing = currentPing + *mInterval / div;
	noteEnd = currentPing + *mGate / div;

	vector<vector<int>> matrix = {{*zero_zero, *zero_one, *zero_two, *zero_three},{ *one_zero, *one_one, *one_two, *one_three },{ *two_zero, *two_one, *two_two, *two_three },{ *three_zero, *three_one, *three_two, *three_three } };
	//vector<vector<int>> matrix = { { 0, 1, 2, 3 },{ 4, 5, 6, 7 },{ 8, 9, 10, 11 },{ 12, 13, 14, 15 } };

	if (positionInfo.isPlaying)
	{
		if (firstPlayed == false) //this deals with starting conditions
		{
			Note = matrix[Y_coor][X_coor];
			if (currentPosition == floor(currentPosition)) 
			{
				firstPlayed = true;
				notePlaying = true;
				midi.addEvent(MidiMessage::noteOn(1, Note, uint8(*mVelo)), 0);
				currentPing = currentPosition;
				currentXPing = currentPosition;
				currentYPing = currentPosition;
				noteEnd = currentPing + *mGate / div;
				nextPing = currentPing + *mInterval / div;
				nextXPing = currentXPing +  *X_div/div;   //first note played means first note move;
				nextYPing = currentYPing + *Y_div/div;
			} 

			else if (currentPosition + beatsPerBuffer >= floor(currentPosition) + 1) 
			{
				
				firstPlayed = true;
				notePlaying = true;
				currentPing = ceil(currentPosition);
				currentXPing = ceil(currentPosition);
				currentYPing = ceil(currentPosition);
				noteEnd = currentPing + *mGate / div;
				noteOnBuffIndex = (currentPing - currentPosition) / beatsPerBuffer * buff + 0.5; //target units are in samples. starting with ppq
				midi.addEvent(MidiMessage::noteOn(1, Note, uint8(*mVelo)), noteOnBuffIndex);
				nextPing = currentPing + *mInterval / div;
				nextXPing = currentXPing + *X_div/div;   //first note played means first note move;
				nextYPing = currentYPing + *Y_div/div;
			}//the case where we need to catch up to the next quarter note
		}


		//deals with stopping note;
		if (currentPosition + beatsPerBuffer >= noteEnd && notePlaying == true)
		{
			notePlaying = false;
			noteOffBuffIndex = (noteEnd - currentPosition) / beatsPerBuffer * buff + 0.5;
			midi.addEvent(MidiMessage::noteOff(1, Note, uint8(*mVelo)), noteOffBuffIndex);
		}

		//section for selecting the next note
		//Move the X:
		if (currentPosition + beatsPerBuffer >= nextXPing)
		{
			X_coor = (X_coor + 1) % 4;
			currentXPing = nextXPing;
			nextXPing = currentXPing + *X_div/div;
		}

		//Move the Y:
		if (currentPosition + beatsPerBuffer >= nextYPing)
		{
			Y_coor = (Y_coor + 1) % 4;
			currentYPing = nextYPing;
			nextYPing = currentYPing + *Y_div/div;
		}

		//deals with the 3,5,7 case that I described on Rob's wall
		if (currentPosition - nextPing >= 1)  
		{
			nextPing = floor(currentPosition) + float(1 / div);
		}

		//X and Y move as dealing with situation on robs wall. 
		if (currentPosition - nextXPing >= 1)
		{
			nextXPing = floor(currentPosition) + float(1/div);
		}
		if (currentPosition - nextYPing >= 1)
		{
			nextYPing = floor(currentPosition) + float(1/div);
		}

		//starting next note during sustained operation;
		if (currentPosition + beatsPerBuffer >= nextPing && notePlaying == false)
		{
			Note = matrix[Y_coor][X_coor];
			notePlaying = true;
			currentPing = nextPing;
			noteEnd = currentPing + *mGate / div;
			noteOnBuffIndex = (nextPing - currentPosition) / beatsPerBuffer * buff + 0.5;
			midi.addEvent(MidiMessage::noteOn(1, Note, uint8(*mVelo)), noteOnBuffIndex);
			nextPing = currentPing + *mInterval / div;
		}

	}

	//purge when stop conditions;
	else
	{
		midi.clear();
		firstPlayed = false;
		notePlaying = false;
	}
	
}

//==============================================================================
bool YxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* YxAudioProcessor::createEditor()
{
    return new YxAudioProcessorEditor (*this);
}

//==============================================================================
void YxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void YxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new YxAudioProcessor();
}
