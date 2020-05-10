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

#include "audioeffectx.h"
#include <string.h>

#include "ExternalStateData_v2.h"
#include "InternalStateData.h"
#include "BandPass.h"
#include "Compression.h"



const int NumPrograms = 1;

const long uniqueID = 'AnRm';

#ifdef EVALUATIONVERSION

const float BleepLength = 0.06f;		//secs
const float BleepInterval = 10.f;		//secs

#endif




class BaseDLL: public AudioEffectX
{
public:
	BaseDLL(audioMasterCallback audioMaster);
	~BaseDLL();

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);

	virtual void setParameter(long index, float value);
	virtual void setParameterAutomated(long index, float value);
	virtual float getParameter(long index);
	virtual void getParameterName(long index, char *label);
	virtual void getParameterDisplay (long index, char *text);
	virtual void getParameterLabel (long index, char *label);

	virtual void process(float **inputs, float **outputs, long sampleframes);
	virtual void processReplacing(float **inputs, float **outputs, long sampleFrames);
	virtual void suspend();
	virtual void resume();

	virtual long getChunk(void** data, bool isPreset); // returns byteSize
	virtual long setChunk(void* data, long byteSize, bool isPreset);// returns byteSize

	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);

	virtual long canDo (char* text);
	virtual long processEvents(VstEvents* ev);

	
	ExternalStateData_v2 *GetExternalState(){return &ExternalState;}
	InternalStateData *GetInternalState()	{return &InternalState;}

	void LoadPresetPatternData(int i);	//called from editor, these fns don't actually load the 
	void LoadPresetSoundData(int i);	//data, they just flag the task for process()
	
	void LoadSoundData();				//as sbove, but these do the file selection themselves
	void SaveSoundData();

	void Randomize();
	
private:

	ExternalStateData_v2 ExternalState;
	InternalStateData InternalState;
	BandPass bandpass;
	Compression compression;

	int NumInputs;	//1 or 2

	bool Resumed;

	bool TolerateHostNonSupp;

#ifdef EVALUATIONVERSION

	int BleepCounter;
	float BleepLevel;

#endif

	inline void PutFeedBackedInputIntoOutputAccum(float *FXOutputAccum, float **inputs, int &i, InternalSoundData &InternalSound, int &sound);
	inline void AddCombFilterValue(float *OutputAccum, InternalSoundData &InternalSound, const int &sound, const int &combwidth);

	inline void AddOscillatorValue(float *OutputAccum, int &sound, float &freq, SoundData_v2 &ExternalSound, InternalSoundData &InternalSound);
	inline float GetOsc(OscillatorType &osc, float &phase, float &level, int &sound, const int oscnum, const int chn);

	inline void UpdateEnvelopeStates(int &sound, SoundData_v2 &ExternalSound, InternalSoundData &InternalSound);

	inline void PollForMIDIAutomation(long i);
};
