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


//PatternEditor.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 15-1-02





#include "vstcontrols.h"

#include "MyConsts.h"



class ExternalStateData_v2;
class SequencerData_v2;
class SleepingReporter;
class InternalStateData;
class ContextMenu;



class PatternEditor : public CControl
{
	public:

		PatternEditor(CRect &size, ExternalStateData_v2 *_ExternalState, SleepingReporter *_SelSoundChanger, InternalStateData *_InternalState, CFrame &_frame, ContextMenu *_contextmenu);
		~PatternEditor();

		virtual void draw(CDrawContext *context);
		virtual void update(CDrawContext *context);
		virtual void mouse(CDrawContext *context, CPoint &where);
		virtual void Resize(CRect &newsize);

		void SetSelSound(int _SelSound);

	private:

		CFrame &frame;
		CBitmap *OCBitmap;
		COffscreenContext *OC;
		CRect OCSize;

		int SelSound;

		ExternalStateData_v2 *ExternalState;

		InternalStateData *InternalState;

		ContextMenu *contextmenu;

		int SoundClickedOn;		//set to the sound index last clicked on, -1 = no click at moment
		int StepClickedOn;		//set to the step index last clicked on
		bool NewClickState;		//set to the new state of the sound last clicked on

		int PrevNoteID[NumSounds];
		int PrevStepPlaying[NumSounds];

		SleepingReporter *SelSoundChanger;

		int m_soundHeight;
		inline void updateSoundHeight();

		inline void drawelement(CDrawContext *context, CRect &where, const int &step, const int &sound);

		inline void getCell(const int &step, const int &sound, CRect &where);

		inline int stepFromPoint(const CPoint &point);//point is relative to this control
};
