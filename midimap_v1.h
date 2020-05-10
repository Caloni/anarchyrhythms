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


//MidiMap.h - AnarchyRhythms

//6-3-02 - LeightonHargreaves

//This defines state data for v1.1, and is included for backwards compatibility
//with v1.1 fxp files


#ifndef MIDIMAPHEADERGATE_v1
#define MIDIMAPHEADERGATE_v1


#include "aeffectx.h"
#include "MyConsts.h"


const int MaxParamsPerController = 20;

const int MidiEventBufferSize = 200;




class InternalMIDIMap
{
	public:
	
		InternalMIDIMap();

		void Reset();
		void Map(int ParamNum, int ControlNum);

		bool QueueEvent(VstMidiEvent *event);

		int ControllerParam[MaxParamsPerController][129];	//0 means pitch-shift, 1..128 encodes controller num+1
		int ParamsPerController[129];

		int NumMidiEvents;
		int CurrentMidiEvent;
		VstMidiEvent MidiEvent[MidiEventBufferSize];
};




class ExternalMIDIMap_v1
{
	friend BOOL CALLBACK MasterDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:

		enum MIDIControlableParam
		{
			kMIDIInput = 0,
			kMIDIDryWet,
			kMIDIOutput,
			numMIDIParams,
		};

		enum MIDIControlableSoundParam
		{
			kMIDILevel = 0,
			kMIDIPan,
			kMIDIPanEnv,
			kMIDIPitch,
			kMIDIPitchEnv,
			kMIDIFilterWidth,
			kMIDILength,
			kMIDIOscillator,
			kMIDIFeedBack,
			numMIDISoundParams
		};

		ExternalMIDIMap_v1();
		~ExternalMIDIMap_v1();

		void Save();
		void Load(HWND systemWindow);
		void Edit(HWND systemWindow, int PopItUpAlreadyParam = -1);

		void CloseDialog();//the dialog must be closed before setChunk/getChunk!

		void CompileInternalMap(InternalMIDIMap *InternalMap);	//called from BaseDLL when bChanged is flagged

		bool bChanged;

		int Channel;									//<0 means 'off', 0..15 = channel num
		int Param[numMIDIParams];
		int SoundParam[NumSounds][numMIDISoundParams];	//<0 means 'none', 0 means pitch-shift, 1..128 encodes controller num+1

	private:


		void ClearAllOptions();



		//these shouldn't really be here, methinx

		HWND MidiMapDialog;

		HWND ChannelCombo;
		HWND ParamCombo[numMIDIParams];
		HWND SoundParamCombo[NumSounds][numMIDISoundParams];
};







#endif