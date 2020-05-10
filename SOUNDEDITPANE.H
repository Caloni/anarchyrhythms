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


//SoundEditPane.h	-	Anarchy RHYTHMS

//Leighton Hargreaves 5-2-2



#ifndef SOUNDEDITPANEHEADERGATE
#define SOUNDEDITPANEHEADERGATE


#include "vstgui.h"
#include "MyConsts.h"
#include "ExternalStateData_v2.h"



class SleepingReporter;
class SoundNameEdit;
class Envelope;
class IntAdjuster;
class MyOptionMenu;
class CheckBox;
class InternalStateData;
class ContextMenu;



enum SoundEditPaneParam
{
	kFirstSoundNameEdit = 0,
	kLastSoundNameEdit = NumSounds-1,
	kFirstEnvelope,
	kLastEnvelope = kFirstEnvelope+NumEnvelopes-1,
	kSEPLevelAdjuster,	//belated naming convention
	kSEPPanAdjuster,
	kSEPCoarsePitchAdjuster,
	kSEPFinePitchAdjuster,
	kSEPFilterWidthAdjuster,
	kSEPOscillatorLevelAdjuster,
	kSEPLengthAdjuster,
	kSEPFeedBackAdjuster,
	kSEPCombAdjuster,
	kSEPCompressionAdjuster,
	kSEPPanEnvelopeRangeAdjuster,
	kSEPPitchEnvelopeRangeAdjuster,
	kSEPOscType1,
	kSEPOscType2,
	kSEPOsc2PhaseInvert
};




class SoundEditPane : public CControlListener
{
	public:

		SoundEditPane(CRect &_size, CFrame &_frame, ExternalStateData_v2 *_ExternalState, InternalStateData *_InternalState, SleepingReporter **_sleepingreporter, ContextMenu *_contextmenu);
		~SoundEditPane();

		void Resize(CRect &newsize);

		void ValueChanged(int SleepingReporterNum);		//automated 

		void AllValuesChanged();

		void valueChanged(CDrawContext *context, CControl *control);	//set by this pane


	private:

		ContextMenu *contextmenu;

		SleepingReporter **sleepingreporter;

		CFrame &frame;

		CRect size;

		ExternalStateData_v2 *ExternalState;
		InternalStateData *InternalState;

		//controls

		SoundNameEdit *SoundName[NumSounds];

		Envelope *envelope[NumEnvelopes];

		IntAdjuster *LevelAdjuster, *PanAdjuster, *CoarsePitchAdjuster, *FinePitchAdjuster, 
					*FilterWidthAdjuster, *OscillatorLevelAdjuster, *PanEnvelopeRangeAdjuster,
					*PitchEnvelopeRangeAdjuster, *LengthAdjuster, *FeedBackAdjuster, 
					*CombAdjuster, *CompressionAdjuster;

		MyOptionMenu *OscMenu[2];

		CheckBox *Osc2PhaseInvert;

		void UpdateOscName(int i);
		void SetMenuFromFileName(MyOptionMenu *menu, char *FileName);
};





#endif //SOUNDEDITPANEHEADERGATE