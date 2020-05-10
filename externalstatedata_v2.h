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


//ExternalStateData_v2.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02


#ifndef EXTERNALSTATEDATAHEADERGATE_V2
#define EXTERNALSTATEDATAHEADERGATE_V2


#include "ExternalStateData_v1.h"

#include "midiMap_v2.h"


//bytesUnused = 100,000 - bytes used, to make ExternalStateData be exactly 100,000, 
//so there's room for future features!
const int bytesUnused = 77048;

enum ShuffleTypeEnum
{
	stpBeat = 0,
	stpStep
};



class PatternData_v2
{
	public:

		PatternData_v2();
		PatternData_v2(const ExternalStateData_v1 &copy);

		char Name[MaxPatternNameLength];

		void Clear();
		void Randomize();

		int Steps;	//Steps per Beat
		int Beats;	//Beats per Pattern

		ShuffleTypeEnum ShuffleType;
		float ShuffleAmount;	//range 0..1

		bool Data[NumSounds][MaxSteps*MaxBeats];
};


class SoundData_v2
{
	public:

		SoundData_v2();
		SoundData_v2(const SoundData_v1 &copy);

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
		int Comb;			//range 0..99

		bool Solo, Mute;

		OscillatorType Osc[2];

		bool Osc2PhaseInvert;

		char WavFileName[2][MAX_PATH];	//never displayed, here to load/save programs


		EnvelopeData envelopedata[NumEnvelopes];

		int PanEnvelopeRange;	//range 0..100%
		int PitchEnvelopeRange;	//range 0..MaxPitchEnvelopeRange
};



class ExternalStateData_v2
{
	public:

		ExternalStateData_v2();
		ExternalStateData_v2(const ExternalStateData_v1 &copy);

		void Randomize();

		char Name[MaxProgramNameLength];

		PatternData_v2 &getPattern() {return Pattern[nCurrentPattern];}

		PatternData_v2 Pattern[nPatterns];
		int nCurrentPattern;

		SequencerData Sequencer;

		SoundData_v2 Sound[NumSounds];

		ExternalMIDIMap_v2 MIDIMap;

		WindowSizeEnum WindowSize;

		int SelectedSound;

		float Mix;	//0 = dry, 1 = wet

		float InLevel, OutLevel;	//range 0..2

		char unused[bytesUnused]; 
};







#endif//EXTERNALSTATEDATAHEADERGATE