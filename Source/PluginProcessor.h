/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
using namespace std;
//==============================================================================
/**
*/
class YxAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    YxAudioProcessor();
    ~YxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

	AudioParameterFloat * mGate;
	AudioParameterInt* mInterval; //this is the actual note-player
	AudioParameterInt* mDivision; //global. All are on 1/4, 1/8, or 1/16th notes

	AudioParameterInt* mVelo; //global velocity;

	AudioParameterInt* Y_div;
	AudioParameterInt* X_div;


	AudioParameterInt* zero_zero;
	AudioParameterInt* zero_one;
	AudioParameterInt* zero_two;
	AudioParameterInt* zero_three;

	AudioParameterInt* one_zero;
	AudioParameterInt* one_one;
	AudioParameterInt* one_two;
	AudioParameterInt* one_three;

	AudioParameterInt* two_zero;
	AudioParameterInt* two_one;
	AudioParameterInt* two_two;
	AudioParameterInt* two_three;

	AudioParameterInt* three_zero;
	AudioParameterInt* three_one;
	AudioParameterInt* three_two;
	AudioParameterInt* three_three;

	int Note;
	double rate;
	double beatsPerBuffer;
	double mBPM;
	double currentPing;
	double nextPing;
	double noteEnd; //has to happen sooner than the next noteEND by like a sample or 4.
	bool firstPlayed = false; // dealing with starting directly on the 1/4 note
	bool notePlaying = false; //dealing with if statements between midi on or off commands WITHIN a buffer. reeee.
	int noteOnBuffIndex; //for dealing with non-zero note-on command indeces in the buffer
	int noteOffBuffIndex; //for dealing with non-zero note-off command indeces in the buffer

	int X_coor = 0;
	int Y_coor = 0;

	//int Y_div = 1; //this pairing will go through all 16 steps in a row, hardcode for now. 
	//int X_div = 4;

	double currentXPing;
	double nextXPing;
	double currentYPing;
	double nextYPing;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YxAudioProcessor)
};
