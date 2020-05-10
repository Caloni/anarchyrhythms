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


/*-----------------------------------------------------------------------------

© 1999, Steinberg Soft und Hardware GmbH, All Rights Reserved
  -  ANARCHY RHYTHMS
  LEIGHTON HARGREAVES

-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "BaseDLL.hpp"
#include "AEffEditor.hpp"
#include "Editor.hpp"
#include "MyHelpers.h"
#include "Presets.h"

#include <assert.h>

FILE *f = NULL;


//-----------------------------------------------------------------------------

BaseDLL::BaseDLL(audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, NumPrograms, ExternalMIDIMap_v2::numMIDIParams + ExternalMIDIMap_v2::numMIDISoundParams*NumSounds)
{
	programsAreChunks();
	canMono();
	NumInputs = 2;//isInputConnected(1) ? 2 : 1;

	setNumInputs(NumInputs);
	setNumOutputs(NumOutputs);
	wantEvents();
	canProcessReplacing();
	setUniqueID(uniqueID);

	suspend();		// flush buffer

	LoadPresetPatternData(0);
}

//------------------------------------------------------------------------
BaseDLL::~BaseDLL()
{
	if(f)
	{
		fclose(f);
	}
}


//------------------------------------------------------------------------
void BaseDLL::setProgramName(char *name)
{
	if(strlen(name) >= MaxProgramNameLength)
	{
		memcpy(ExternalState.Name, name, MaxProgramNameLength-1);

		ExternalState.Name[MaxProgramNameLength-1] = 0;
	}
	else
	{	
		strcpy(ExternalState.Name, name);
	}
}

//------------------------------------------------------------------------
void BaseDLL::getProgramName(char *name)
{
	strcpy(name, ExternalState.Name);
}


long BaseDLL::canDo (char* text)
{
	if (!strcmp(text, "receiveVstEvents"))			return 1;
	if (!strcmp(text, "receiveVstMidiEvent"))		return 1;
	if (!strcmp(text, "receiveVstTimeInfo"))		return 1;
	if (!strcmp(text, "1in2out"))					return 1;
	if (!strcmp(text, "2in2out"))					return 1;
	
	return 0;	// -1 = explicitly can't do; 0 => don't know
}



void BaseDLL::setParameter(long index, float value)
{
	int sound;

	int SoundIndex;

	switch(index)
	{
		case ExternalMIDIMap_v2::kMIDIInput:	ExternalState.InLevel = value*2;		break;
		case ExternalMIDIMap_v2::kMIDIDryWet:	ExternalState.Mix = value;				break;
		case ExternalMIDIMap_v2::kMIDIOutput:	ExternalState.OutLevel = value*2;		break;
		case ExternalMIDIMap_v2::kShuffleType:	
			ExternalState.getPattern().ShuffleType = (ShuffleTypeEnum)(int)(value*1.9);	
			InternalState.GenerateShuffleLookup(ExternalState.getPattern());
			break;
		case ExternalMIDIMap_v2::kShuffleAmount:
			ExternalState.getPattern().ShuffleAmount = value;	
			InternalState.GenerateShuffleLookup(ExternalState.getPattern());
			break;
		case ExternalMIDIMap_v2::kCurrentPattern:
			ExternalState.nCurrentPattern = (int) (value*(float)(nPatterns-1));
			InternalState.GenerateShuffleLookup(ExternalState.getPattern());
			break;

		default:			//must be a sound param

			SoundIndex = index - ExternalMIDIMap_v2::numMIDIParams;
			sound = (NumSounds-1) - SoundIndex / ExternalMIDIMap_v2::numMIDISoundParams;
			SoundIndex %= ExternalMIDIMap_v2::numMIDISoundParams;

			switch(SoundIndex)
			{
				case ExternalMIDIMap_v2::kMIDILevel:			ExternalState.Sound[sound].Level				= (int)(value*100);						break;
				case ExternalMIDIMap_v2::kMIDIPan:				ExternalState.Sound[sound].Pan					= (int)(value*200-100);					break;
				case ExternalMIDIMap_v2::kMIDIPanEnv:			ExternalState.Sound[sound].PanEnvelopeRange		= (int)(value*100);						break;
				case ExternalMIDIMap_v2::kMIDIPitch:			ExternalState.Sound[sound].CoarsePitch			= (int)(value*MaxPitch);				break;	
				case ExternalMIDIMap_v2::kMIDIPitchEnv:			ExternalState.Sound[sound].PitchEnvelopeRange	= (int)(value*MaxPitchEnvelopeRange);	break;
				case ExternalMIDIMap_v2::kMIDIFilterWidth:		ExternalState.Sound[sound].FilterWidth			= (int)(value*100);						break;
				case ExternalMIDIMap_v2::kMIDILength:			ExternalState.Sound[sound].Length				= (int)(value*MaxLength);				break;	
				case ExternalMIDIMap_v2::kMIDIOscillator:		ExternalState.Sound[sound].Oscillator			= (int)(value*100);						break;	
				case ExternalMIDIMap_v2::kMIDIFeedBack:			ExternalState.Sound[sound].FeedBack				= (int)(value*99);						break;	
			}

			break;
	}

	if(editor)
	{
		((Editor *)editor)->NotifyMIDIAutomate(index);
	}
}




void BaseDLL::setParameterAutomated(long index, float value)
{
	AudioEffectX::setParameterAutomated(index, value);

	setParameter(index, value);
}



float BaseDLL::getParameter(long index)
{
	int sound;

	switch (index)
	{
		case ExternalMIDIMap_v2::kMIDIInput:		return ExternalState.InLevel/2;
		case ExternalMIDIMap_v2::kMIDIDryWet:		return ExternalState.Mix;
		case ExternalMIDIMap_v2::kMIDIOutput:		return ExternalState.OutLevel/2;
		case ExternalMIDIMap_v2::kShuffleType:		return (float)ExternalState.getPattern().ShuffleType/1.9f;
		case ExternalMIDIMap_v2::kShuffleAmount:	return ExternalState.getPattern().ShuffleAmount;
		case ExternalMIDIMap_v2::kCurrentPattern:	return (float)ExternalState.nCurrentPattern / (float)(nPatterns-1);

		default:			//must be a sound param

			index -= ExternalMIDIMap_v2::numMIDIParams;
			sound = (NumSounds-1) - index / ExternalMIDIMap_v2::numMIDISoundParams;
			index %= ExternalMIDIMap_v2::numMIDISoundParams;

			switch(index)
			{
				case ExternalMIDIMap_v2::kMIDILevel:			return (float)(ExternalState.Sound[sound].Level) / 100.f;
				case ExternalMIDIMap_v2::kMIDIPan:				return (float)(ExternalState.Sound[sound].Pan + 100) / 200.f;		
				case ExternalMIDIMap_v2::kMIDIPanEnv:			return (float)(ExternalState.Sound[sound].PanEnvelopeRange) / 100.f;
				case ExternalMIDIMap_v2::kMIDIPitch:			return (float)(ExternalState.Sound[sound].CoarsePitch) / (float)MaxPitch;	
				case ExternalMIDIMap_v2::kMIDIPitchEnv:			return (float)(ExternalState.Sound[sound].PitchEnvelopeRange) / (float)MaxPitchEnvelopeRange;
				case ExternalMIDIMap_v2::kMIDIFilterWidth:		return (float)(ExternalState.Sound[sound].FilterWidth) / 100.f;
				case ExternalMIDIMap_v2::kMIDILength:			return (float)(ExternalState.Sound[sound].Length) /	(float)MaxLength;	
				case ExternalMIDIMap_v2::kMIDIOscillator:		return (float)(ExternalState.Sound[sound].Oscillator) / 100.f;	
				case ExternalMIDIMap_v2::kMIDIFeedBack:			return (float)(ExternalState.Sound[sound].FeedBack) / 99.f;	

				default:	return 0;
			}
	}
}



void BaseDLL::getParameterName(long index, char *label)
{
	int sound;

	switch (index)
	{
		case ExternalMIDIMap_v2::kMIDIInput:		strcpy(label, "Input Lvl");		return;
		case ExternalMIDIMap_v2::kMIDIDryWet:		strcpy(label, "dry<->wet");		return;
		case ExternalMIDIMap_v2::kMIDIOutput:		strcpy(label, "Output Lvl");	return;
		case ExternalMIDIMap_v2::kShuffleType:		strcpy(label, "Shuffle typ");	return;
		case ExternalMIDIMap_v2::kShuffleAmount:	strcpy(label, "Shuffle val");	return;	
		case ExternalMIDIMap_v2::kCurrentPattern:	strcpy(label, "Pattern");		return;

		default:			//must be a sound param

			index -= ExternalMIDIMap_v2::numMIDIParams;
			sound = (NumSounds-1) - index / ExternalMIDIMap_v2::numMIDISoundParams;
			index %= ExternalMIDIMap_v2::numMIDISoundParams;

			strcpy(label, "Snd");	
			itoa((NumSounds)-sound, label+3);
			strcat(label, " ");

			switch(index)
			{
				case ExternalMIDIMap_v2::kMIDILevel:			strcat(label, "Level");		return;
				case ExternalMIDIMap_v2::kMIDIPan:				strcat(label, "Pan");		return;
				case ExternalMIDIMap_v2::kMIDIPanEnv:			strcat(label, "Pan Env");	return;
				case ExternalMIDIMap_v2::kMIDIPitch:			strcat(label, "Pitch");		return;
				case ExternalMIDIMap_v2::kMIDIPitchEnv:			strcat(label, "Pitch Env");	return;
				case ExternalMIDIMap_v2::kMIDIFilterWidth:		strcat(label, "Filter");	return;
				case ExternalMIDIMap_v2::kMIDILength:			strcat(label, "Length");	return;
				case ExternalMIDIMap_v2::kMIDIOscillator:		strcat(label, "Oscillator");return;
				case ExternalMIDIMap_v2::kMIDIFeedBack:			strcat(label, "FeedBack");	return;
			}
	}
}

//------------------------------------------------------------------------
void BaseDLL::getParameterDisplay (long index, char *text)
{
	int sound;

	switch (index)
	{
		case ExternalMIDIMap_v2::kMIDIInput:	dB2string(ExternalState.InLevel, text);		return;
		case ExternalMIDIMap_v2::kMIDIDryWet:	text[0] = 0;								return;
		case ExternalMIDIMap_v2::kMIDIOutput:	dB2string(ExternalState.OutLevel, text);	return;
		case ExternalMIDIMap_v2::kShuffleType:  strcpy(text, (ExternalState.getPattern().ShuffleType == stpBeat) ? "Beats" : "Steps");	return;
		case ExternalMIDIMap_v2::kShuffleAmount:itoa((int)(ExternalState.getPattern().ShuffleAmount*100.f), text);	return;
		case ExternalMIDIMap_v2::kCurrentPattern:strcpy(text, ExternalState.getPattern().Name);	return;

		default:			//must be a sound param

			index -= ExternalMIDIMap_v2::numMIDIParams;
			sound = (NumSounds-1) - index / ExternalMIDIMap_v2::numMIDISoundParams;
			index %= ExternalMIDIMap_v2::numMIDISoundParams;

			switch(index)
			{
				case ExternalMIDIMap_v2::kMIDILevel:			dB2string((float)(ExternalState.Sound[sound].Level) / 100.f, text);		return;
				case ExternalMIDIMap_v2::kMIDIPan:				text[0] = 0;															return;
				case ExternalMIDIMap_v2::kMIDIPanEnv:			itoa(ExternalState.Sound[sound].PanEnvelopeRange, text);				return;

				case ExternalMIDIMap_v2::kMIDIPitch:			

					switch(ExternalState.Sound[sound].CoarsePitch%12)
					{	
						case 0:		strcpy(text, "C");	break;
						case 1:		strcpy(text, "C#");	break;
						case 2:		strcpy(text, "D");	break;
						case 3:		strcpy(text, "D#");	break;
						case 4:		strcpy(text, "E");	break;
						case 5:		strcpy(text, "F");	break;
						case 6:		strcpy(text, "F#");	break;
						case 7:		strcpy(text, "G");	break;
						case 8:		strcpy(text, "G#");	break;
						case 9:		strcpy(text, "A");	break;
						case 10:	strcpy(text, "A#");	break;
						case 11:	strcpy(text, "B");	break;
					}

					itoa(ExternalState.Sound[sound].CoarsePitch/12, &(text[strlen(text)]));	
					return;

				case ExternalMIDIMap_v2::kMIDIPitchEnv:			itoa(ExternalState.Sound[sound].PitchEnvelopeRange, text);					return;
				case ExternalMIDIMap_v2::kMIDIFilterWidth:		itoa(ExternalState.Sound[sound].FilterWidth, text);							return;
				case ExternalMIDIMap_v2::kMIDILength:			itoa(ExternalState.Sound[sound].Length, text);								return;
				case ExternalMIDIMap_v2::kMIDIOscillator:		dB2string((float)(ExternalState.Sound[sound].Oscillator)/100.f, text);		return;
				case ExternalMIDIMap_v2::kMIDIFeedBack:			itoa(ExternalState.Sound[sound].FeedBack, text);							return;
			}
	}
}

//------------------------------------------------------------------------
void BaseDLL::getParameterLabel (long index, char *label)
{
	int sound;

	switch (index)
	{
		case ExternalMIDIMap_v2::kMIDIInput:	
		case ExternalMIDIMap_v2::kMIDIOutput:	strcpy(label, "dB");	return;

		case ExternalMIDIMap_v2::kMIDIDryWet:	
		case ExternalMIDIMap_v2::kShuffleType:
		case ExternalMIDIMap_v2::kCurrentPattern:	label[0]=0;			return;

		case ExternalMIDIMap_v2::kShuffleAmount:strcpy(label, "%");		return;

		default:			//must be a sound param

			index -= ExternalMIDIMap_v2::numMIDIParams;
			sound = (NumSounds-1) - index / ExternalMIDIMap_v2::numMIDISoundParams;
			index %= ExternalMIDIMap_v2::numMIDISoundParams;

			switch(index)
			{
				case ExternalMIDIMap_v2::kMIDILevel:			
				case ExternalMIDIMap_v2::kMIDIOscillator:		strcpy(label, "dB");			return;
				case ExternalMIDIMap_v2::kMIDIPanEnv:			
				case ExternalMIDIMap_v2::kMIDIFilterWidth:		
				case ExternalMIDIMap_v2::kMIDIFeedBack:			strcpy(label, "%");				return;
				case ExternalMIDIMap_v2::kMIDIPitch:			
				case ExternalMIDIMap_v2::kMIDIPan:				label[0] = 0;					return;
				case ExternalMIDIMap_v2::kMIDIPitchEnv:			strcpy(label, "semitones");		return;
				case ExternalMIDIMap_v2::kMIDILength:			strcpy(label, "beat/1000s");	return;
			}
	}
}


//------------------------------------------------------------------------
void BaseDLL::suspend()
{
	InternalState.Reset();
	bandpass.Reset();
	compression.Reset();

#ifdef EVALUATIONVERSION

	BleepCounter = 0;
	BleepLevel = 0;

#endif

	Resumed = false;
}



void BaseDLL::resume()
{
	NumInputs = isInputConnected(1) ? 2 : 1;

	//setNumInputs(NumInputs);

	Resumed = true;

	wantEvents();
}




bool BaseDLL::getEffectName (char* name) 
{
	#ifdef EVALUATIONVERSION
	
		strcpy(name, "AnarchyRhythms DEMO"); 

	#else

		strcpy(name, "AnarchyRhythms"); 

	#endif

	return true;
}


bool BaseDLL::getVendorString (char* text) 
{
	strcpy(text, "AnarchySoundSoftware"); 
	return true;
}


bool BaseDLL::getProductString (char* text) 
{
	#ifdef EVALUATIONVERSION
	
		strcpy(text, "AnarchyRhythms v1.1 DEMO"); 

	#else

		strcpy(text, "AnarchyRhythms v1.1"); 

	#endif

	return true;
}



void BaseDLL::LoadPresetPatternData(int i)
{
	CPresets presets;

	char buff[sizeof(ExternalStateData_v2)];
	memcpy(buff, presets.getPresetData(i), sizeof(ExternalStateData_v2));
	TolerateHostNonSupp = true;
	setChunk((void *) buff, sizeof(ExternalStateData_v2), false);
}


void BaseDLL::LoadPresetSoundData(int i)
{
	char buff[sizeof(SoundData_v1)];

	memcpy(buff, GetPresetSoundData(i), sizeof(SoundData_v1));
	
	SoundData_v1 *sound_v1 = (SoundData_v1 *)buff;
	SoundData_v2 sound_v2(*sound_v1);
	
	memcpy(&(ExternalState.Sound[ExternalState.SelectedSound]), &sound_v2, sizeof(SoundData_v2));

	if(editor)
	{
		((Editor *)editor)->AllExternalDataChanged();
	}
}



void BaseDLL::LoadSoundData()
{
	char FileName[MAX_PATH];

	FileName[0] = 0;

	OPENFILENAME ofn;

    ofn.lStructSize 		=sizeof(OPENFILENAME);
    ofn.hwndOwner 			=NULL;
    ofn.hInstance 			=NULL;
    ofn.lpstrFilter 		="*.ars";
    ofn.lpstrCustomFilter 	=NULL;     
    ofn.nMaxCustFilter 		=0;
    ofn.nFilterIndex 		=0;
    ofn.lpstrFile 			=FileName;
    ofn.nMaxFile 			=MAX_PATH;
    ofn.lpstrFileTitle 		=NULL;
    ofn.nMaxFileTitle 		=0;
    ofn.lpstrInitialDir 	=NULL;
    ofn.lpstrTitle 			="LOAD USER SOUND";
    ofn.Flags				=OFN_FILEMUSTEXIST; 
    ofn.nFileOffset			=0; 
    ofn.nFileExtension		=0; 
    ofn.lpstrDefExt			="ARS"; 
    ofn.lCustData			=0; 
    ofn.lpfnHook			=NULL; 
    ofn.lpTemplateName		=0; 

	if(GetOpenFileName(&ofn))
	{
		FILE *f = fopen(FileName, "rb");

		if(f)
		{
			if(fread(&(ExternalState.Sound[ExternalState.SelectedSound]), sizeof(SoundData_v2), 1, f) == 1)
			{
				for(int osc=0; osc<2; osc++)
				{
					if(ExternalState.Sound[ExternalState.SelectedSound].Osc[osc] == oscWAV)
					{
						InternalState.Sound[ExternalState.SelectedSound].wav[osc].Load(ExternalState.Sound[ExternalState.SelectedSound].WavFileName[osc]);
					}
				}
		
				if(editor)
				{
					((Editor *)editor)->AllExternalDataChanged();
				}
			}
			else
			{
				if(editor)
				{
					((Editor *)editor)->PostError("Error loading user sound");
				}
			}

			fclose(f);
		}
		else
		{
			if(editor)
			{
				((Editor *)editor)->PostError("Error opening user sound");
			}
		}
	}
}


void BaseDLL::SaveSoundData()
{
	char FileName[MAX_PATH];

	strcpy(FileName, ExternalState.Sound[ExternalState.SelectedSound].Name);
	
	OPENFILENAME ofn;

    ofn.lStructSize 		=sizeof(OPENFILENAME);
    ofn.hwndOwner 			=NULL;
    ofn.hInstance 			=NULL;
    ofn.lpstrFilter 		="*.ars";
    ofn.lpstrCustomFilter 	=NULL;     
    ofn.nMaxCustFilter 		=0;
    ofn.nFilterIndex 		=0;
    ofn.lpstrFile 			=FileName;
    ofn.nMaxFile 			=MAX_PATH;
    ofn.lpstrFileTitle 		=NULL;
    ofn.nMaxFileTitle 		=0;
    ofn.lpstrInitialDir 	=NULL;
    ofn.lpstrTitle 			="LOAD USER SOUND";
    ofn.Flags				=OFN_OVERWRITEPROMPT; 
    ofn.nFileOffset			=0; 
    ofn.nFileExtension		=0; 
    ofn.lpstrDefExt			="ARS"; 
    ofn.lCustData			=0; 
    ofn.lpfnHook			=NULL; 
    ofn.lpTemplateName		=0; 

	if(GetSaveFileName(&ofn))
	{
		FILE *f = fopen(FileName, "wb");

		if(f)
		{
			if(fwrite(&(ExternalState.Sound[ExternalState.SelectedSound]), sizeof(SoundData_v2), 1, f) != 1)
			{
				if(editor)
				{
					((Editor *)editor)->PostError("Error saving user sound");
				}
			}

			fclose(f);
		}
		else
		{
			if(editor)
			{
				((Editor *)editor)->PostError("Error opening user sound file for writing");
			}
		}
	}
}



void BaseDLL::Randomize()
{
	ExternalState.Randomize();

	if(editor)
	{
		((Editor *)editor)->AllExternalDataChanged();
	}
}




long BaseDLL::getChunk(void** data, bool isPreset)
{
	ExternalState.MIDIMap.CloseDialog();

	(*data) = (void *)(&ExternalState);
		
	return sizeof(ExternalState);	
}



long BaseDLL::setChunk(void* data, long byteSize, bool isPreset)
{
	ExternalState.MIDIMap.CloseDialog();

	if( byteSize==sizeof(ExternalStateData_v1) ) //loading from v1
	{
		ExternalStateData_v2 state_v2( *(ExternalStateData_v1 *) data );	//convert to v2 data

		setChunk((void *) &state_v2, sizeof(ExternalStateData_v2), false);
		return byteSize;
	}

	if(byteSize==sizeof(ExternalState))
	{
		memcpy((void *)(&ExternalState), data, MIN(byteSize, sizeof(ExternalState)));

		for(int sound=0; sound<NumSounds; sound++)	//load the wavs
		{
			for(int osc=0; osc<2; osc++)
			{
				if(ExternalState.Sound[sound].Osc[osc] == oscWAV)
				{
					InternalState.Sound[sound].wav[osc].Load(ExternalState.Sound[sound].WavFileName[osc]);
				}
			}
		}

		ExternalState.MIDIMap.bChanged = true;

		InternalState.GenerateShuffleLookup(ExternalState.getPattern());

		if(editor)
		{
			((Editor *)editor)->AllExternalDataChanged();
		}

		return byteSize;
	}
	else
	{
		//failsafe silently
		/*if(editor)
		{
			((Editor *)editor)->PostError("setChunk() recieved incorrect number of bytes!");
		}*/

		return 0;
	}
}





inline void BaseDLL::PutFeedBackedInputIntoOutputAccum(float *FXOutputAccum, float **inputs, int &i, InternalSoundData &InternalSound, int &sound)
{
	float FeedBack;
	int FeedbackHistoryPos;
	int chn;

	FeedBack = (float)ExternalState.Sound[sound].FeedBack * 0.01f;

	FeedbackHistoryPos = InternalSound.LengthRemaining%FeedBackHistoryLength;

	for(chn=0; chn<NumInputs; chn++)
	{
		float &FeedbackSample = InternalSound.FeedBackHistory[chn][FeedbackHistoryPos];

		FeedbackSample *= FeedBack;
		FeedbackSample += inputs[chn][i] * (1.f - FeedBack);

		UnDenormalise(FeedbackSample);

		FXOutputAccum[chn] = FeedbackSample * ExternalState.InLevel;
	}
}


inline void BaseDLL::AddCombFilterValue(float *OutputAccum, InternalSoundData &InternalSound, const int &sound, const int &combwidth)
{
	float Comb = (float)ExternalState.Sound[sound].Comb * 0.01f;
	float CombOpp = 1.f - Comb;

	int CombHistoryPos = InternalSound.CombPos;
	InternalSound.CombPos++;
	if( InternalSound.CombPos >= min(combwidth, CombHistoryLength) )
	{
		InternalSound.CombPos=0;
	}

	for(int chn=0; chn<NumInputs; chn++)
	{
		float &CombSample = InternalSound.CombHistory[chn][CombHistoryPos];

		CombSample *= Comb;
		CombSample += OutputAccum[chn] * CombOpp;

		OutputAccum[chn] = CombSample;
	}
}





inline float BaseDLL::GetOsc(OscillatorType &osc, float &phase, float &level, int &sound, const int oscnum, const int chn)
{
	float q;
	
	InternalSoundData *InternalSound; 
	Wav *wav;
	int SamplePos;

	switch(osc)
	{
		case oscSine:		return level*fsin(phase*6.28318530717f);//sin lookup?
		case oscSquare:		return (phase<0.5) ? level : -level;
		case oscSaw:		q = phase*4;	return (q<1) ? q*level : q<3 ? (2-q)*level : (q-4)*level;
		case oscNoise:		return ((float)(rand()&1024)/256.f -2.f) * level;
		case oscWAV:		
			
			InternalSound = &(InternalState.Sound[sound]);
			wav = &(InternalSound->wav[oscnum]);
			SamplePos = InternalSound->InitialLength-InternalSound->LengthRemaining;

			return (wav->bLoaded && (SamplePos < wav->Length)) ?
				
				level*wav->data[MIN(chn, wav->WavChannels-1)][SamplePos] : 0;

		default:	return 0;

	}
}




inline void BaseDLL::AddOscillatorValue(float *OutputAccum, int &sound, float &freq, SoundData_v2 &ExternalSound, InternalSoundData &InternalSound)
{
	float interp, level, val;

	InternalSound.OscillatorPhase += freq / sampleRate;

	while(InternalSound.OscillatorPhase >= 1.f)
	{
		InternalSound.OscillatorPhase -= 1.f;
	}

	interp = InternalSound.EnvelopeData[envOscillator].EnvelopeVal;

	level = ExternalSound.Oscillator * 0.01f;

	level *= (level*level);

	val = interp * GetOsc(ExternalSound.Osc[0], InternalSound.OscillatorPhase, level, sound, 0, 0) +
				
			(ExternalSound.Osc2PhaseInvert ? (interp-1.f) : (1.f-interp)) * 
			
			GetOsc(ExternalSound.Osc[1], InternalSound.OscillatorPhase, level, sound, 1, 0);

	OutputAccum[0] += val;

	if(NumInputs==2)
	{
		if((ExternalSound.Osc[0] == oscWAV) || (ExternalSound.Osc[1] == oscWAV))
		{
			val = interp * GetOsc(ExternalSound.Osc[0], InternalSound.OscillatorPhase, level, sound, 0, 1) +
				
			(ExternalSound.Osc2PhaseInvert ? (interp-1.f) : (1.f-interp)) * 
			
			GetOsc(ExternalSound.Osc[1], InternalSound.OscillatorPhase, level, sound, 1, 1);		
		}

		OutputAccum[1] += val;
	}
}





inline void BaseDLL::UpdateEnvelopeStates(int &sound, SoundData_v2 &ExternalSound, InternalSoundData &InternalSound)
{
	EnvelopeData *envelopedata;
	InternalEnvelopeData *InternalEnvelope;
	float EnvelopePos, x1, x2, div, y1interp, y2interp;
	int Prev, i;

	EnvelopePos = 1.f-(float)InternalSound.LengthRemaining / (float)InternalSound.InitialLength;

	for(i=0; i<NumEnvelopes; i++)
	{
		envelopedata = &(ExternalSound.envelopedata[i]);

		InternalEnvelope = &(InternalSound.EnvelopeData[i]);

		Prev = InternalEnvelope->PrevPoint;

		if(Prev < envelopedata->NumPoints-2)
		{
			if(EnvelopePos >= envelopedata->PointX[Prev+1])
			{
				InternalEnvelope->PrevPoint++;
				Prev++;
			}
		}

		x1 = envelopedata->PointX[Prev];
		x2 = envelopedata->PointX[Prev+1];

		div = MAX(0.0001f, x2-x1);

		y2interp = MAX(0.f, MIN(1.f, (EnvelopePos-x1)/div));
		y1interp = 1.f - y2interp;

		InternalEnvelope->EnvelopeVal = envelopedata->PointY[Prev]*y1interp + envelopedata->PointY[Prev+1]*y2interp;

		if(InternalSound.AttackStitchCounter>0)
		{
			float fDiv = sampleRate * 0.01f;

			InternalEnvelope->EnvelopeVal = (InternalEnvelope->fPrevNoteEndVal*(float)InternalSound.AttackStitchCounter+InternalEnvelope->EnvelopeVal*(fDiv-(float)InternalSound.AttackStitchCounter))/fDiv;
		}
	}
}



inline void BaseDLL::PollForMIDIAutomation(long i)
{
	char* midiData;
	float value;
	int ParamRef, ParamLoop, Param, Sound, SoundParam;
	
	InternalMIDIMap &InternalMIDI = InternalState.MIDIMap;

	while((InternalMIDI.CurrentMidiEvent < InternalMIDI.NumMidiEvents) &&
				(InternalMIDI.MidiEvent[InternalMIDI.CurrentMidiEvent].deltaFrames <= i))
	{
		midiData = InternalMIDI.MidiEvent[InternalMIDI.CurrentMidiEvent].midiData;

		if((midiData[0] & 0xF0) == 0xB0)	//if tis a controller message
		{
			value = (float)midiData[2] / 127.f;

			ParamRef = midiData[1] + 1;
		}
		else						//must be a pitch shift (been pre-filtered for desirability)
		{
			value = (float)(midiData[2]*128+midiData[1])/16383.f;

			ParamRef = 0;
		}

		for(ParamLoop=0; ParamLoop<InternalMIDI.ParamsPerController[ParamRef]; ParamLoop++)
		{
			Param = InternalMIDI.ControllerParam[ParamLoop][ParamRef];

			switch(Param)
			{
				case ExternalMIDIMap_v2::kMIDIInput:	ExternalState.InLevel	= value*2;	break;
				case ExternalMIDIMap_v2::kMIDIDryWet:	ExternalState.Mix		= value;	break;
				case ExternalMIDIMap_v2::kMIDIOutput:	ExternalState.OutLevel	= value*2;	break;

				case ExternalMIDIMap_v2::kShuffleType:	
					ExternalState.getPattern().ShuffleType = (ShuffleTypeEnum)(int)(value*1.9f);	
					InternalState.GenerateShuffleLookup(ExternalState.getPattern());
					break;

				case ExternalMIDIMap_v2::kShuffleAmount:
					ExternalState.getPattern().ShuffleAmount = value;	
					InternalState.GenerateShuffleLookup(ExternalState.getPattern());
					break;

				case ExternalMIDIMap_v2::kCurrentPattern:
					ExternalState.nCurrentPattern = (int) (value * (float)(nPatterns-1) );
					InternalState.GenerateShuffleLookup(ExternalState.getPattern());
					break;

				default:	//must be a sound param

					SoundParam = Param - ExternalMIDIMap_v2::numMIDIParams;
					
					Sound = NumSounds - 1 - (SoundParam / ExternalMIDIMap_v2::numMIDISoundParams);
					
					switch(SoundParam % ExternalMIDIMap_v2::numMIDISoundParams)
					{
						case ExternalMIDIMap_v2::kMIDILevel:		ExternalState.Sound[Sound].Level =				(int)(value * 100.f);							break;
						case ExternalMIDIMap_v2::kMIDIPan:			ExternalState.Sound[Sound].Pan =				(int)((value-0.5) * 200.f);						break;
						case ExternalMIDIMap_v2::kMIDIPanEnv:		ExternalState.Sound[Sound].PanEnvelopeRange =	(int)(value * 100.f);							break;
						case ExternalMIDIMap_v2::kMIDIPitch:		ExternalState.Sound[Sound].CoarsePitch =		(int)(value * (float)MaxPitch+0.5);				break;
						case ExternalMIDIMap_v2::kMIDIPitchEnv:		ExternalState.Sound[Sound].PitchEnvelopeRange = (int)(value * (float)MaxPitchEnvelopeRange);	break;
						case ExternalMIDIMap_v2::kMIDIFilterWidth:	ExternalState.Sound[Sound].FilterWidth =		(int)(value * 100.f);							break;
						case ExternalMIDIMap_v2::kMIDILength:		ExternalState.Sound[Sound].Length =				(int)(value * value * (float)MaxLength);		break;
						case ExternalMIDIMap_v2::kMIDIOscillator:	ExternalState.Sound[Sound].Oscillator =			(int)(value * 100.f);							break;
						case ExternalMIDIMap_v2::kMIDIFeedBack:		ExternalState.Sound[Sound].FeedBack	=			(int)(value * 99.f);							break;
					}					
			}

			if(editor)
			{
				((Editor *)editor)->NotifyMIDIAutomate(Param);
			}
		}

		InternalMIDI.CurrentMidiEvent++;
	}
}





void BaseDLL::process(float **inputs, float **outputs, long sampleframes)
{
	//variables

	int NumSteps, step, i, sound, IntPitch, nShuffledStep;

	bool SomeAreSolo, NewStep;

	float bpm, fract, CurrentStep, Pitch, freq, subtone, EnvelopedFilterWidth, HiCut, LoCut;
	float EnvelopedLevel, EnvelopedPan;

	float FXOutputAccum[NumOutputs];//could be NumInputs not NumOutputs if NumInputs weren't floating
	float FXSoundOutput[NumOutputs];
	float FXOutput[NumOutputs];		

	//initial stuff

	if(!Resumed)
	{
		resume();
	}

	if(ExternalState.MIDIMap.bChanged)
	{
		ExternalState.MIDIMap.CompileInternalMap(&(InternalState.MIDIMap));
	}
	
	NumSteps = ExternalState.getPattern().Steps*ExternalState.getPattern().Beats;

	SomeAreSolo = ExternalState.Sound[0].Solo | ExternalState.Sound[1].Solo | ExternalState.Sound[2].Solo | ExternalState.Sound[3].Solo;

	//VstTimeInf sync if switched on

	if(ExternalState.Sequencer.HostSync)
	{
		VstTimeInfo *TimeInf = getTimeInfo(kVstTempoValid | kVstPpqPosValid | kVstTimeSigValid);

		if(TimeInf && (TimeInf->flags & kVstTempoValid) && (TimeInf->flags & kVstPpqPosValid) && (TimeInf->flags & kVstTimeSigValid))
		{
			bpm = MAX(30.f, MIN(250.f, (float)TimeInf->tempo));
		
			if(bpm != ExternalState.Sequencer.BPM)
			{
				ExternalState.Sequencer.BPM = bpm;

				if(editor)
				{
					((Editor *)editor)->BPMChanged();
				}
			}

			step = (int)TimeInf->ppqPos;
			fract = (float)TimeInf->ppqPos - (float)step;
			step %= ExternalState.getPattern().Beats;

			InternalState.CurrentStep = ((float)step + fract) * (float)ExternalState.getPattern().Steps;
		}
		else
		{
			ExternalState.Sequencer.HostSync = false;

			if(editor)
			{
				((Editor *)editor)->HostSyncChanged();

				if(TolerateHostNonSupp)
				{
					TolerateHostNonSupp = false;
				}
				else
				{
					((Editor *)editor)->PostError("Host syncronisation is not supported\nby this host application!");
				}
			}
		}
	}

	CurrentStep = InternalState.CurrentStep;

	//loop through input

	for(i=0; i<sampleframes; i++)
	{
		PollForMIDIAutomation(i);

		FXOutput[0] = FXOutput[1] = 0;

		//shuffle logic
		if(InternalState.PrevSampleStep < NumSteps-1)
		{
			float nNextStep = InternalState.GetShuffle(InternalState.PrevSampleStep+1);

			NewStep = (CurrentStep >= nNextStep);
		}
		else
		{
			NewStep = (CurrentStep < InternalState.PrevSampleStep);
		}

		if(NewStep)
		{
			nShuffledStep = InternalState.PrevSampleStep+1;
			if(nShuffledStep >= NumSteps)
			{
				nShuffledStep = 0;
			}
		}
		else
		{
			nShuffledStep = InternalState.PrevSampleStep;
		}

		//loop through sounds

		for(sound = 0; sound < NumSounds; sound++)
		{
			SoundData_v2 &ExternalSound = ExternalState.Sound[sound];
			InternalSoundData &InternalSound = InternalState.Sound[sound];

			if(NewStep && ExternalState.getPattern().Data[sound][nShuffledStep] && !ExternalSound.Mute && (ExternalSound.Solo || !SomeAreSolo))//new note
			{
				if(InternalSound.LengthRemaining>0)//if a note was playing...
				{
					InternalSound.AttackStitchCounter = (int)((float)sampleRate * 0.01f);

					for(int nEnv = 0; nEnv<NumEnvelopes; nEnv++)
					{
						InternalSound.EnvelopeData[nEnv].fPrevNoteEndVal = InternalSound.EnvelopeData[nEnv].EnvelopeVal;
					}
				}

				InternalSound.StopNote();

				InternalSound.StepPlaying = nShuffledStep;
				InternalSound.NoteID = rand();

				InternalSound.InitialLength = (int)(ExternalSound.Length*0.06f*(float)sampleRate/ExternalState.Sequencer.BPM)+1;
				InternalSound.LengthRemaining = InternalSound.InitialLength;
			}

			//if sound is playing
			if(InternalSound.LengthRemaining>0)
			{
				UpdateEnvelopeStates(sound, ExternalSound, InternalSound);

				Pitch = (float)(ExternalSound.CoarsePitch)+(InternalSound.EnvelopeData[envPitch].EnvelopeVal-0.5f)*(float)(ExternalSound.PitchEnvelopeRange*2)+(float)(ExternalSound.FinePitch)*0.01f;

				IntPitch = (int)Pitch;

				freq = InternalState.GetFreq(IntPitch);

				subtone = Pitch - (float)IntPitch;

				freq += (subtone*freq*0.05946309435929526456182529494634f);

				PutFeedBackedInputIntoOutputAccum(FXOutputAccum, inputs, i, InternalSound, sound);

				AddCombFilterValue(FXOutputAccum, InternalSound, sound, (int)(sampleRate/freq));

				AddOscillatorValue(FXOutputAccum, sound, freq, ExternalSound, InternalSound);

				EnvelopedFilterWidth = ExternalSound.FilterWidth*0.15f*InternalSound.EnvelopeData[envFilterWidth].EnvelopeVal + 1.f;

				HiCut = freq * EnvelopedFilterWidth/sampleRate;
				LoCut = freq / (EnvelopedFilterWidth*sampleRate);

				bandpass.Filter(FXOutputAccum, sound, HiCut, LoCut, NumInputs);

				compression.Compress(FXOutputAccum, ExternalSound.Compression, sound, NumInputs);

				EnvelopedLevel = ExternalSound.Level * InternalSound.EnvelopeData[envLevel].EnvelopeVal * 0.01f;

				EnvelopedPan = MAX(0.f, MIN(1.f, 
					
					((float)(ExternalSound.Pan+100) + 

					(InternalSound.EnvelopeData[envPan].EnvelopeVal-0.5f) *

					(float)(ExternalSound.PanEnvelopeRange*2))/200.f ));

				FXSoundOutput[0] = FXOutputAccum[0] * EnvelopedLevel * (1.f-EnvelopedPan);
				FXSoundOutput[1] = FXOutputAccum[NumInputs-1] * EnvelopedLevel * (EnvelopedPan);

				if(InternalSound.AttackStitchCounter>0)
				{
					InternalSound.AttackStitchCounter--;
				}

				FXOutput[0] += FXSoundOutput[0];
				FXOutput[1] += FXSoundOutput[1];

				InternalSound.LengthRemaining--;

				if(InternalSound.LengthRemaining==0)
				{
					InternalSound.StopNote();
				}
			}
		}

		//mix in the dry

		if(NumInputs == 1)
		{
			outputs[0][i] = FXOutput[0]*ExternalState.Mix*ExternalState.OutLevel + inputs[0][i]*(1.f-ExternalState.Mix);
			outputs[1][i] = FXOutput[1]*ExternalState.Mix*ExternalState.OutLevel + inputs[0][i]*(1.f-ExternalState.Mix);
		}
		else
		{
			outputs[0][i] = FXOutput[0]*ExternalState.Mix*ExternalState.OutLevel + inputs[0][i]*(1.f-ExternalState.Mix);
			outputs[1][i] = FXOutput[1]*ExternalState.Mix*ExternalState.OutLevel + inputs[1][i]*(1.f-ExternalState.Mix);
		}


#ifdef EVALUATIONVERSION
		
		if(BleepCounter == 0)
		{
			BleepCounter += (int)(sampleRate * BleepInterval);
		}
		else
		{
			BleepCounter--;
		}

		if(BleepCounter < (int)(sampleRate * BleepLength))
		{
			outputs[0][i] = outputs[1][i] = BleepLevel * (float)((rand()&255)-128)/128.f;
		}
		else
		{
			BleepLevel = MAX(BleepLevel, (ABS(outputs[0][i])+ABS(outputs[1][i]))/2);

			if((BleepCounter & 0x3FF)==0)
			{
				BleepLevel *= 0.95f;

				UnDenormalise(BleepLevel);
			}
		}

#endif

		//advance CurrentStep counter

		InternalState.PrevSampleStep = nShuffledStep;

		CurrentStep = InternalState.CurrentStep + (float)(i+1)*ExternalState.Sequencer.BPM * (float)ExternalState.getPattern().Steps/(60.f*(float)sampleRate);
		
		if(CurrentStep > (float)NumSteps)
		{
			CurrentStep -= (float)NumSteps;
		}	
	}

	//Advance current position for the next update

	InternalState.CurrentStep = CurrentStep;
}





//---------------------------------------------------------------------------
void BaseDLL::processReplacing (float **inputs, float **outputs, long sampleframes)
{
	process(inputs, outputs, sampleframes);
}





//-----------------------------------------------------------------------------------------
long BaseDLL::processEvents(VstEvents* ev)
{
	InternalState.MIDIMap.NumMidiEvents = 0;
	InternalState.MIDIMap.CurrentMidiEvent = 0;

	for(long i = 0; i < ev->numEvents; i++)
	{
		if((ev->events[i])->type != kVstMidiType)
		{
			continue;
		}

		VstMidiEvent* event = (VstMidiEvent*)ev->events[i];

		char* midiData = event->midiData;

		if((midiData[0] & 0x0F) != ExternalState.MIDIMap.Channel)	//not on our channel mate
		{
			continue;
		}

		switch(midiData[0] & 0xF0)	//filter status byte for messages we're interested in
		{
			case 0xB0:	//control change

				if(InternalState.MIDIMap.ParamsPerController[(midiData[1]&0x7F)+1] > 0)
				{
					if(InternalState.MIDIMap.QueueEvent(event))
					{
						break;
					}
					else
					{
						return 1;
					}
				}

			case 0xE0:	//pitch wheel

				if(InternalState.MIDIMap.ParamsPerController[0] > 0)
				{
					if(InternalState.MIDIMap.QueueEvent(event))
					{
						break;
					}
					else
					{
						return 1;
					}
				}
		}
	}

	return 1;	// want more
}
