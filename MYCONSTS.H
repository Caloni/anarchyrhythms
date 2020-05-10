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
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


//MyConsts.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 15-1-02

#ifndef MYCONSTSHEADERGATE
#define MYCONSTSHEADERGATE


#include "vstgui.h"



const int NumOutputs = 2;	//hard-wired to never change



const int NumSounds = 4;	//hard-wired to never change

const int MaxBeats = 16;
const int MaxSteps = 8;

const int MaxLength = 4000;	//1/1000ths of a beat

const int MaxSoundNameLength = 12;
const int MaxPatternNameLength = 24;
const int MaxProgramNameLength = 24;


const double StitchDieThreshold = 0.01f;		//level in 0..1 range

const int MaxEnvelopePoints = 16;


const int MaxPitch = 119;	//c0 ->b8, fractional part represents subintervals
const int MaxPitchEnvelopeRange = 48;	//4 8va either direction


const int MaxOscNameLength = 8;

const int MaxWavLength = 176400; //samples (4sec at 44100!) extreme case means 11.2Mg! 




const int FeedBackHistoryLength = 88200;	//2 sec, means allocating 1.4Mg mono or 2.8Mg stereo!
const int CombHistoryLength = 8192;			//can cope with 5Hz at 44100


const int nPatterns = 32;



enum SleepingReporterEnum
{
	kSelSoundChange = 0,
	kFirstSoundNameChange,
	kLastSoundNameChange = kFirstSoundNameChange + NumSounds-1,
	kLevelChange,
	kCombChange,
	kCompressionChange,
	kPanChange,
	kPanEnvChange,
	kPitchChange,
	kPitchEnvChange,
	kFilterWidthChange,
	kOscillatorLevelChange,
	kLengthChange,
	kFeedBackChange,

	kNumSleepingReporters
};



enum EnvelopeEnum
{
	envLevel,
	envPan,
	envPitch,
	envFilterWidth,
	envOscillator,
	NumEnvelopes
};


const CColor ControlBk = {95, 95, 95, 0};



const CColor SoundCol[NumSounds] = {
	
	{	0,		220,	0,		0	},
	{	200,	200,	0,		0	},
	{	255,	0,		0,		0	},
	{	0,		0,		255,	0	}
};


const CColor LightSoundCol[NumSounds] = {
	
	{	196,	235,	196,	0	},
	{	255,	255,	160,	0	},
	{	255,	196,	196,	0	},
	{	196,	196,	255,	0	}
};


const CColor DarkSoundCol[NumSounds] = {
	
	{	0,		128,	0,		0	},
	{	104,	104,		0,		0	},
	{	128,	0,		0,		0	},
	{	0,		0,		128,	0	}
};



const CColor SaturatedSoundCol[NumSounds] = {

	{	64,		96,		64,		0	},
	{	84,		84,		64,		0	},
	{	96,		64,		64,		0	},
	{	64,		64,		96,		0	}
};


const CColor VeryLightSoundCol[NumSounds] = {
	
	{	220,	255,	220,	0	},
	{	255,	255,	200,	0	},
	{	255,	220,	220,	0	},
	{	220,	220,	255,	0	}
};


const CColor White = {255, 255, 255, 0};
const CColor Black = {0, 0, 0, 0};





#endif //MYCONSTSHEADERGATE