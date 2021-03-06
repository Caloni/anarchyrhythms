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


//LightUpBox.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2



#ifndef LIGHTUPBOXHEADERGATE
#define LIGHTUPBOXHEADERGATE


#include "vstgui.h"



const int MaxLightUpBoxLabelLen = 12;


class SleepingReporter;
class ContextMenu;



class LightUpBox : public CControl
{
	public:

		LightUpBox(CRect size, CControlListener *Listener, int tag, char *_Caption, int _MyIndex, SleepingReporter *_SelSoundChanger, bool InitialHighlight, ContextMenu *_contextmenu, bool _IsSolo);
	
		virtual void draw(CDrawContext *context);
		virtual void mouse(CDrawContext *context, CPoint &where);

		void Resize(CRect &newsize);

		void MySetValue(bool _val);
		bool MyGetValue();

		void SetHighlight(bool _Highlight);

	private:

		ContextMenu *contextmenu;

		SleepingReporter *SelSoundChanger;

		bool IsSolo;

		int MyIndex;

		bool Highlight;

		bool val;

		char Caption[MaxLightUpBoxLabelLen];
};









#endif //LIGHTUPBOXHEADERGATE