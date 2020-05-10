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


//InternalStateData.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02


#include "MyConsts.h"
#include "MyHelpers.h"
#include "Wav.h"
#include "MidiMap_v2.h"
#include "ExternalStateData_v2.h"


class InternalEnvelopeData
{
	public:

		InternalEnvelopeData();

		void Reset();

		int PrevPoint;
		float EnvelopeVal;

		float fPrevNoteEndVal;
};



class InternalSoundData
{
	public:

		InternalSoundData();
		~InternalSoundData();

		void Reset();

		void StopNote();

		int StepPlaying;	//-1 if no step is playing
		int NoteID;			//set to rand() each new note, to trigger patterneditor redraw

		int LengthRemaining;	//in samples

		float OscillatorPhase;	//range 0..1

		int InitialLength;		//in samples
		int CombPos;			//in samples

		InternalEnvelopeData EnvelopeData[NumEnvelopes];

		float *FeedBackHistory[NumOutputs];//could be NumInputs not NumOutputs if NumInputs weren't floating
		float *CombHistory[NumOutputs];//could be NumInputs not NumOutputs if NumInputs weren't floating


		int AttackStitchCounter;

		Wav wav[2];
};





class InternalStateData
{
	public:

		InternalStateData();

		float CurrentStep;	//position within sequence at start of process

		InternalSoundData Sound[NumSounds];

		InternalMIDIMap MIDIMap;

		int PrevSampleStep;		//the step we were in during previous sample, used to detect 
								//start of new sounds

		PatternData_v2 CopiedPattern;
		bool CopiedPatternSet;

		void Reset();

		void copyPattern(const PatternData_v2 &pattern);

		void GenerateShuffleLookup(const PatternData_v2 &pattern);

		inline float GetFreq(int pitch){return PitchToFreqLookup[MAX(0, MIN(MaxPitch-1, pitch))];}

		inline float GetShuffle(int step) {return ShuffleLookup[MAX(0, MIN(MaxSteps*MaxBeats-1, step))];}

	private:

		float PitchToFreqLookup[MaxPitch];

		float ShuffleLookup[MaxSteps*MaxBeats];
};







