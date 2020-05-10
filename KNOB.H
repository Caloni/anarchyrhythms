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


//Filename:		KNOB.H  -  ANARCHY RHYTHMS
//Author:		Leighton Hargreaves
//Date:			25/05/2001
//Description:	


#ifndef _KNOB_H_ /* GATE. */
#define _KNOB_H_



#include "vstcontrols.h"



const int MaxKnobStrLen = 20;

class SleepingReporter;
class ContextMenu;



class Knob : public CControl
{
	public:

		Knob(CRect &size, CControlListener *listener, int tag, CFrame &frame, int _MyIndex, char *_Name, char *_Label, SleepingReporter *_SelSoundChanger, bool InitialHighlight, ContextMenu *_contextmenu, int _HelpID, int _MIDIParam); 
		~Knob();

		void draw(CDrawContext *context);
		void update(CDrawContext *context);
		void mouse(CDrawContext *context, CPoint &where);

		void setValue(float val, char *display);

		void Resize(CRect &newsize);
		
		void SetHighlight(bool _Highlight);


	private:

		ContextMenu *contextmenu;

		int MyIndex;

		int HelpID, MIDIParam;

		CBitmap *OCBitmap;
		COffscreenContext *OC;
		CRect OCSize;

		int LabelHeight;

		char buf[MaxKnobStrLen];
		char Name[MaxKnobStrLen];
		char Label[MaxKnobStrLen];
		char Display[MaxKnobStrLen];

		bool Adjusting;
		int MouseClickHeight;
		float MouseClickVal;

		SleepingReporter *SelSoundChanger;

		bool Highlight;

		CBitmap *bmpKnobSmall, *bmpKnobLarge;
};













#endif /* GATE */


//End of file