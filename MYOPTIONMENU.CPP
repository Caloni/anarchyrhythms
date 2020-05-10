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


//MyOptionMenu.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 2-2-2




#include "MyOptionMenu.h"

#include "MyConsts.h"
#include "ContextMenu.h"



MyOptionMenu::MyOptionMenu(CRect &size, CControlListener *listener, int tag, char *_MenuText, bool _bSmallText, int _ActiveWidth, ContextMenu *_contextmenu, HelpIDEnum _helpID, int _MIDIParam, bool _sound)
	:	bSmallText(_bSmallText),
		contextmenu(_contextmenu),
		helpID(_helpID), 
		MIDIParam(_MIDIParam),
		sound(_sound),
		HiLight(false),
		ActiveWidth((_ActiveWidth>0) ? _ActiveWidth : size.width()),
		COptionMenu(size, listener, tag)
{
	Resize(size);

	SetText(_MenuText);

	SetColor(ControlBk, ControlBk, kWhiteCColor);
}





void MyOptionMenu::draw(CDrawContext *context)
{
	context->setFillColor(Background);
	context->fillRect(size);
	context->setFrameColor(Background);
	context->drawRect(size);

	context->setFillColor(Nearground);
	context->fillRect(ActiveArea);
	context->setFrameColor(HiLight ? Foreground : Background);
	context->drawRect(ActiveArea);

	context->setFont(bSmallText ? kNormalFontVerySmall : kNormalFont);
	context->setFontColor(Foreground);
	context->drawString(MenuText, ActiveArea);
}



void MyOptionMenu::update(CDrawContext *context)
{
	CPoint where;

	context->getMouseLocation(where);

	bool NewHiLight = where.isInside(ActiveArea);

	if(NewHiLight != HiLight)
	{
		HiLight=NewHiLight;
		redraw();
	}
}


void MyOptionMenu::mouse(CDrawContext *context, CPoint &where)
{
	if(context->getMouseButtons() & kLButton)
	{
		if(where.isInside(ActiveArea))
		{
			COptionMenu::mouse(context, where);
		}
	}
	else
	{
		if(contextmenu)
		{
			contextmenu->SetContext(helpID, MIDIParam, sound);
			contextmenu->Popup(context);
		}
	}
}





void MyOptionMenu::SetColor(CColor _Background, CColor _Nearground, CColor _Foreground)
{
	Background = _Background;
	Nearground = _Nearground;
	Foreground = _Foreground;
}




void MyOptionMenu::Resize(CRect &newsize)
{
	size = newsize;

	ActiveArea(size.left, size.top, size.left + ActiveWidth, size.bottom);
}



void MyOptionMenu::SetText(char *_MenuText)
{
	strcpy(MenuText, _MenuText);
}