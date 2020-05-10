/*
Copyright 2002, 2004 Leighton Hargreaves
All Rights Reserved

This file is part of AnarchyRhythms

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met: 

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer. 
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution. 

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


//ExternalStateData_v1.h  -  ANARCHY RHYTHMS

//This defines state data for v1.1, and is included for backwards compatibility
//with v1.1 fxp files

//Leighton Hargreaves 16-1-02


#ifndef EXTERNALSTATEDATAHEADERGATE_V1
#define EXTERNALSTATEDATAHEADERGATE_V1





#include "MyConsts.h"

#include "MidiMap_v1.h"



enum WindowSizeEnum
{
	WSmall = 0,
	WLarge
};

enum OscillatorType
{
	oscSine = 0,
	oscSquare,
	oscSaw,
	oscNoise,
	oscWAV
};


class PatternData
{
	public:

		PatternData();

		void Randomize();

		int Steps;	//Steps per Beat
		int Beats;	//Beats per Pattern

		bool Data[NumSounds][MaxSteps*MaxBeats];

};





class SequencerData
{
	public:

		SequencerData();

		bool HostSync;

		float BPM;	
};





class EnvelopeData
{
	public:

		//no constructor as different envelopes need different defaults!

		void Randomize();

		int NumPoints;

		float PointX[MaxEnvelopePoints];		//range 0..1
		float PointY[MaxEnvelopePoints];		//range 0..1
};






class SoundData_v1
{
	public:

		SoundData_v1();

		void Randomize();

		char Name[MaxSoundNameLength];
	
		int Level;			//range 0..100
		int Pan;			//range -100..100
		int CoarsePitch;	//range 0..MaxPitch
		int FinePitch;		//range -50..50
		int FilterWidth;	//range 0..100
		int Length;			//1/1000ths of a beat, range 0..MaxLength
		int Oscillator;		//range 0..100
		int FeedBack;		//range 0..99
		int Compression;    //range 0..100

		bool Solo, Mute;

		OscillatorType Osc[2];

		bool Osc2PhaseInvert;

		char WavFileName[2][MAX_PATH];	//never displayed, here to load/save programs


		EnvelopeData envelopedata[NumEnvelopes];

		int PanEnvelopeRange;	//range 0..100%
		int PitchEnvelopeRange;	//range 0..MaxPitchEnvelopeRange

		//etc
};






class ExternalStateData_v1
{
	public:

		ExternalStateData_v1();

		void Randomize();

		char Name[MaxProgramNameLength];

		PatternData Pattern;

		SequencerData Sequencer;

		SoundData_v1 Sound[NumSounds];

		ExternalMIDIMap_v1 MIDIMap;

		WindowSizeEnum WindowSize;

		int SelectedSound;

		float Mix;	//0 = dry, 1 = wet

		float InLevel, OutLevel;	//range 0..2
};







#endif//EXTERNALSTATEDATAHEADERGATE