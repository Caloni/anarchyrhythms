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


//CheckBox.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2



#include "CheckBox.h"
#include "ContextMenu.h"





CheckBox::CheckBox(CRect size, CControlListener *Listener, int tag, char *_Caption, ContextMenu *_contextmenu, int _HelpID, bool _IsPartOfSound)
	:	CControl(size, Listener, tag),
		HelpID(_HelpID),
		IsPartOfSound(_IsPartOfSound),
		contextmenu(_contextmenu),
		HiLight(false)
{
	SetColor(kBlackCColor, kGreyCColor, kWhiteCColor);
		
	strcpy(Caption, _Caption);

	val = false;

	Resize(size);
}



void CheckBox::draw(CDrawContext *context)
{
	context->setFillColor(Background);
	context->fillRect(size);

	context->setFrameColor(Background);
	context->drawRect(size);

	if(Nearground!=Background)
	{
		context->setFillColor(Nearground);
		context->fillRect(ValDisplay);
	}

	context->setFrameColor(HiLight ? Foreground : Background);
	context->drawRect(size);
	//context->drawRect(ValDisplay);

	context->setFont(kNormalFontSmall);
	context->setFontColor(Foreground);

	context->drawString(Caption, CaptionDisplay, false, kRightText);

	if(val)
	{
		context->setFrameColor(Foreground);

		context->moveTo(CPoint(ValDisplay.left+ValDisplay.width()/4, ValDisplay.top+ValDisplay.height()/2));
		context->lineTo(CPoint(ValDisplay.left+ValDisplay.width()/2, ValDisplay.bottom-ValDisplay.height()/4));
		context->lineTo(CPoint(ValDisplay.right+1-ValDisplay.width()/4, ValDisplay.top+ValDisplay.height()/4-1));
	}
}



void CheckBox::update(CDrawContext *context)
{
	CPoint where;

	context->getMouseLocation(where);

	bool NewHiLight = where.isInside(size);

	if(NewHiLight != HiLight)
	{
		HiLight= NewHiLight;
		redraw();
	}
}



void CheckBox::mouse(CDrawContext *context, CPoint &where)
{
	if(context->getMouseButtons() & kLButton)
	{
		int buttons = context->getMouseButtons();

		if(buttons & (kLButton|kRButton))
		{
			val = !val;
		}

		listener->valueChanged(context, this);
	}
	else
	{
		contextmenu->SetContext((HelpIDEnum)HelpID, -1, IsPartOfSound);
		contextmenu->Popup(context);
	}
}




void CheckBox::MySetValue(bool _val)
{
	val = _val;
}



bool CheckBox::MyGetValue()
{
	return val;
	
}



void CheckBox::Resize(CRect &newsize)
{
	size = newsize;

	ValDisplay = CRect(size.right-size.height(), size.top, size.right, size.bottom);
	CaptionDisplay = CRect(size.left, size.top, ValDisplay.left, size.bottom);
}





void CheckBox::SetColor(CColor _Background, CColor _Nearground, CColor _Foreground)
{
	Background = _Background;
	Nearground = _Nearground;
	Foreground = _Foreground;
}
