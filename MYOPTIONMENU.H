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


//MyOptionMenu.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 2-2-2



#ifndef MYOPTIONMENUGATE
#define MYOPTIONMENUGATE



#include "vstgui.h"
#include "help.h"

class ContextMenu;




class MyOptionMenu : public COptionMenu
{
	public:

		MyOptionMenu(CRect &size, CControlListener *listener, int tag, char *_MenuText, bool _bSmallText, int _ActiveWidth, ContextMenu *_contextmenu = NULL, HelpIDEnum _helpID = HIDContents, int _MIDIParam = 0, bool _sound = false);

		virtual void draw(CDrawContext *context);
		virtual void update(CDrawContext *context);
		virtual void mouse(CDrawContext *context, CPoint &where);

		void SetColor(CColor _Background, CColor _Nearground, CColor _Foreground);

		void Resize(CRect &newsize);

		void SetText(char *_MenuText);


	private:

		ContextMenu *contextmenu;
		HelpIDEnum helpID;
		int MIDIParam;
		bool sound;
		
		int ActiveWidth;

		CRect ActiveArea;

		char MenuText[24];

		bool bSmallText;

		CColor Background, Nearground, Foreground;

		bool HiLight;
};









#endif	//MYOPTIONMENUGATE