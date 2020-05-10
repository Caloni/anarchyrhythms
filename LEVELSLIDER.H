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


//LevelSlider.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02



#ifndef LEVELSLIDERHEADER
#define LEVELSLIDERHEADER



#include "vstgui.h"


class SleepingReporter;
class ContextMenu;




class LevelSlider : public CControl
{
	public:

		LevelSlider(CRect& size, CControlListener* listener, int tag, int _MyIndex, CFrame &frame, SleepingReporter *_SelSoundChanger, bool InitialHighlight, ContextMenu *_contextmenu); 
		~LevelSlider();
		
		virtual void draw(CDrawContext *context);
		virtual void update(CDrawContext *context);
		virtual void mouse(CDrawContext *context, CPoint &where);

		void SetHighlight(bool _Highlight);

		void Resize(CRect &newsize)		{	size = newsize;		}

	private:

		ContextMenu *contextmenu;

		int MyIndex;

		bool Dragging;

		SleepingReporter *SelSoundChanger;

		bool Highlight;

		CBitmap *bmpSlider;
};





#endif //LEVELSLIDERHEADER