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


//InternalStateData.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02



#include "MyHelpers.h"
#include "InternalStateData.h"

#include "ExternalStateData_v2.h"
#include "stdio.h"




InternalEnvelopeData::InternalEnvelopeData()
{
	Reset();
}





void InternalEnvelopeData::Reset()
{
	PrevPoint = 0;
	EnvelopeVal = 0;
	fPrevNoteEndVal = 0;
}


/////////////////////////////////////////////////


InternalSoundData::InternalSoundData()
{
	for(int i=0; i<NumOutputs; i++)
	{
		FeedBackHistory[i] = new float[FeedBackHistoryLength];
		CombHistory[i] = new float[CombHistoryLength];
	}

	Reset();
}



InternalSoundData::~InternalSoundData()
{
	for(int i=0; i<NumOutputs; i++)
	{
		delete FeedBackHistory[i];
		delete CombHistory[i];
	}
}




void InternalSoundData::Reset()
{
	StepPlaying = -1;

	LengthRemaining = 0;

	CombPos = 0;

	OscillatorPhase = 0;

	InitialLength = 0;

	for(int i=0; i<NumEnvelopes; i++)
	{
		EnvelopeData[i].Reset();
	}

	for(i=0; i<NumOutputs; i++)
	{
		memset((void *)(FeedBackHistory[i]), 0, FeedBackHistoryLength*sizeof(float));
		memset((void *)(CombHistory[i]), 0, CombHistoryLength*sizeof(float));
	}

	AttackStitchCounter = 0;
}



void InternalSoundData::StopNote()
{
	StepPlaying = -1;

	NoteID = rand();

	for(int i=0; i<NumEnvelopes; i++)
	{
		EnvelopeData[i].PrevPoint = 0;
	}
}




InternalStateData::InternalStateData()
{
	Reset();

	//init PitchToFreqLookup

	for(int i=0; i<MaxPitch; i++)
	{
		PitchToFreqLookup[i] = 32.7031956625748293347312f * fpow(2.0f, (float)i/12.f);
	}

	//init ShuffleLookup
	for(i=0; i<MaxBeats*MaxSteps; i++)
	{
		ShuffleLookup[i] = (float)i;
	}

	CopiedPatternSet = false;
};




void InternalStateData::Reset()
{
	CurrentStep = 0;

	PrevSampleStep = -1;

	for(int i=0; i<NumSounds; i++)
	{
		Sound[i].Reset();
	}
}




void InternalStateData::GenerateShuffleLookup(const PatternData_v2 &pattern)
{
	float ProcessedAmount = pattern.ShuffleAmount*pattern.ShuffleAmount*0.5f;

	if(pattern.ShuffleType == stpStep)		//substep shuffle
	{
		for(int i=0; i<pattern.Beats*pattern.Steps; i++)
		{
			ShuffleLookup[i] = (float) i;

			if(i&1)
			{
				ShuffleLookup[i] += ProcessedAmount;
			}
		}
	}
	else									//beat shuffle
	{
		float ProcessedAmountOpp = 1-ProcessedAmount;
		int i=0;
		for(int beat=0; beat<pattern.Beats; beat++)
		{
			ShuffleLookup[i] = (float)i;
			i++;

			for(int stepOpp=pattern.Steps-1; stepOpp>=1; stepOpp--) 
			{
				ShuffleLookup[i] = (beat+1)*pattern.Steps - stepOpp*ProcessedAmountOpp;
				i++;
			}
		}
	}
}


void InternalStateData::copyPattern(const PatternData_v2 &pattern)
{
	CopiedPattern = pattern;
	CopiedPatternSet = true;

	char buffer[MaxPatternNameLength + 10];
	sprintf(buffer, "copy of %s", CopiedPattern.Name);
	buffer[MaxPatternNameLength-1] = 0;
	strcpy(CopiedPattern.Name, buffer);
}