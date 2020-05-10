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


//ExternalStateData.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02



#include <stdio.h>
#include <time.h>


#include "ExternalStateData_v2.h"
#include "MyHelpers.h"



PatternData_v2::PatternData_v2()
{
	Steps = 4;	
	Beats = 4;	

	ShuffleType = stpStep;
	ShuffleAmount = 0;

	sprintf(Name, "empty pattern");

	for(int i=0; i<NumSounds; i++)
	{
		for(int j=0; j<MaxSteps*MaxBeats; j++)
		{
			Data[i][j] = false;
		}
	}
}


PatternData_v2::PatternData_v2(const ExternalStateData_v1 &copy)
{
	Steps = copy.Pattern.Steps;	
	Beats = copy.Pattern.Beats;	

	ShuffleType = stpStep;
	ShuffleAmount = 0;

	for(int i=0; i<NumSounds; i++)
	{
		for(int j=0; j<MaxSteps*MaxBeats; j++)
		{
			Data[i][j] = copy.Pattern.Data[i][j];
		}
	}

	sprintf(Name, copy.Name);
}


void PatternData_v2::Clear()
{
	for(int i=0; i<NumSounds; i++)
	{
		for(int j=0; j<MaxSteps*MaxBeats; j++)
		{
			Data[i][j] = false;
		}
	}

	sprintf(Name, "empty pattern");
}


void PatternData_v2::Randomize()
{
	int seed = rand();

	srand(seed);

	sprintf(Name, "rand#%i", seed%100000);

	ShuffleType = ( (rand()%2) == 0 ) ? stpStep : stpBeat;
	ShuffleAmount = (float)(rand()%10)*(rand()%10) * 0.01f;

	for(int i=0; i<NumSounds; i++)
	{
		for(int j=0; j<MaxSteps*MaxBeats; j++)
		{
			Data[i][j] = (rand()%4==0);
		}
	}
}


SoundData_v2::SoundData_v2()
{
	static int CreationIndex = 0;

	strcpy(Name, "Sound ");

	Name[strlen(Name)-1] = '4'-CreationIndex;

	CreationIndex = (CreationIndex+1) % 4;

	Level = 50;
	Pan = 0;

	CoarsePitch = MaxPitch/2;
	FinePitch = 0;

	FilterWidth = 20;
	Length = 100;		//10th of a beat
	Oscillator = 0;
	Compression = 30;
	FeedBack = 30;
	Comb = 20;

	Solo = Mute = false;

	Osc[0] = oscNoise;
	Osc[1] = oscSquare;

	strcpy(WavFileName[0], "c:\\somefileorother.wav");
	strcpy(WavFileName[1], "ping.wav");

	Osc2PhaseInvert = false;

	PanEnvelopeRange = 100;
	PitchEnvelopeRange = 12;

	envelopedata[envLevel].NumPoints = 4;		//level envelope
	envelopedata[envLevel].PointX[0] = 0;
	envelopedata[envLevel].PointY[0] = 0;
	envelopedata[envLevel].PointX[1] = 0.02f;
	envelopedata[envLevel].PointY[1] = 1;
	envelopedata[envLevel].PointX[2] = 0.5;
	envelopedata[envLevel].PointY[2] = 1;
	envelopedata[envLevel].PointX[3] = 1;
	envelopedata[envLevel].PointY[3] = 0;


	envelopedata[envPan].NumPoints = 2;		//pan envelope
	envelopedata[envPan].PointX[0] = 0;
	envelopedata[envPan].PointY[0] = 0.5;
	envelopedata[envPan].PointX[1] = 1;
	envelopedata[envPan].PointY[1] = 0.5;

	envelopedata[envPitch].NumPoints = 2;
	envelopedata[envPitch].PointX[0] = 0;
	envelopedata[envPitch].PointY[0] = 0.5;
	envelopedata[envPitch].PointX[1] = 1;
	envelopedata[envPitch].PointY[1] = 0.5;

	envelopedata[envFilterWidth].NumPoints = 2;
	envelopedata[envFilterWidth].PointX[0] = 0;
	envelopedata[envFilterWidth].PointY[0] = 1;
	envelopedata[envFilterWidth].PointX[1] = 1;
	envelopedata[envFilterWidth].PointY[1] = 1;

	envelopedata[envOscillator].NumPoints = 2;
	envelopedata[envOscillator].PointX[0] = 0;
	envelopedata[envOscillator].PointY[0] = 0.5;
	envelopedata[envOscillator].PointX[1] = 1;
	envelopedata[envOscillator].PointY[1] = 0.5;
}



SoundData_v2::SoundData_v2(const SoundData_v1 &copy)
{
	strcpy(Name, copy.Name);

	Level = copy.Level;
	Pan = copy.Pan;

	CoarsePitch = copy.CoarsePitch;
	FinePitch = copy.FinePitch;

	FilterWidth = copy.FilterWidth;
	Length = copy.Length;
	Oscillator = copy.Oscillator;
	Compression = copy.Compression;
	FeedBack = copy.FeedBack;
	Comb = 0;

	Solo = copy.Solo;
	Mute = copy.Mute;

	Osc[0] = copy.Osc[0];
	Osc[1] = copy.Osc[1];

	strcpy(WavFileName[0], copy.WavFileName[0]);
	strcpy(WavFileName[1], copy.WavFileName[1]);

	Osc2PhaseInvert = copy.Osc2PhaseInvert;

	PanEnvelopeRange = copy.PanEnvelopeRange;
	PitchEnvelopeRange = copy.PitchEnvelopeRange;

	for(int i=0; i<NumEnvelopes; i++)
	{
		envelopedata[i] = copy.envelopedata[i];
	}
}


void SoundData_v2::Randomize()
{
	int seed = rand();

	sprintf(Name, "rand#%i", seed%100000);	
	
	Level = 50 + rand()%50;
	Pan = rand()%200 - 100;

	CoarsePitch = rand()%MaxPitch;

	FilterWidth = rand()%75+25;
	Length = (rand()%11) * (rand()%11) * (rand()%11) * (MaxLength-100) / 1000 + 100;
	Oscillator = (rand()%3) * (rand()%3) * (rand()%3);
	Compression = rand()%10*rand()%10;
	FeedBack = rand()%100;
	Comb = rand()%100;

	Osc[0] = (OscillatorType)(rand()%(int)(oscWAV));

	do{

		Osc[1] = (OscillatorType)(rand()%(int)(oscWAV));

	}
	while(Osc[1]==Osc[0]);

	Osc2PhaseInvert = (rand()%2==0);

	PanEnvelopeRange = rand()%100;
	PitchEnvelopeRange = rand()%MaxPitchEnvelopeRange;

	for(int i=0; i<NumEnvelopes; i++)
	{
		envelopedata[i].Randomize();
	}

	envelopedata[envLevel].PointY[rand()%(envelopedata[envLevel].NumPoints-1)+1] = 1;
	envelopedata[envLevel].PointY[envelopedata[envLevel].NumPoints-1] = 0;
	envelopedata[envLevel].PointY[0] = 0;
	envelopedata[envFilterWidth].PointY[rand()%(envelopedata[envFilterWidth].NumPoints-1)+1] = 1;
}


ExternalStateData_v2::ExternalStateData_v2()
{
	WindowSize = WSmall;
	
	SelectedSound = 0;

	Mix = 0.5f;

	InLevel = OutLevel = 1.6666f;

	strcpy(Name, "init");

	nCurrentPattern = 0;

	memset( unused, 0, bytesUnused ); 
}


ExternalStateData_v2::ExternalStateData_v2(const ExternalStateData_v1 &copy)
	:	MIDIMap(copy.MIDIMap)
{
	nCurrentPattern = 0;

	strcpy(Name, copy.Name);
	
	for(int i=0; i<NumSounds; i++)
	{
		Sound[i] = SoundData_v2(copy.Sound[i]);
	}

	Pattern[0] = PatternData_v2(copy);
	for(i=1; i<nPatterns; i++)
	{
		Pattern[i] = PatternData_v2();
	}

	Sequencer = copy.Sequencer;
	WindowSize = copy.WindowSize;
	SelectedSound = copy.SelectedSound;
	Mix = copy.Mix;
	InLevel = copy.InLevel;
	OutLevel = copy.OutLevel;

	memset( unused, 0, bytesUnused ); 
}



void ExternalStateData_v2::Randomize()
{
	srand(time(NULL));

	int seed = rand();

	srand(seed);

	Mix = 1;

	InLevel = OutLevel = 1.6666f;

	sprintf(Name, "rand#%i", seed%100000);

	for(int i=0; i<nPatterns; i++)
	{
		Pattern[i].Randomize();
	}

	nCurrentPattern = rand()%nPatterns;

	for(i=0; i<NumSounds; i++)
	{
		Sound[i].Randomize();
	}
}



