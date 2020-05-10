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


//SpinButton.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 6-4



#include "SpinButton.h"
#include "MyHelpers.h"
#include "MyConsts.h"
#include "ContextMenu.h"




SpinButton::SpinButton(CRect size, CControlListener *Listener, int tag, bool up, ContextMenu *_contextmenu, int _HelpID, int _MIDIParam)
	:	CControl(size, Listener, tag),
		contextmenu(_contextmenu),
		MIDIParam(_MIDIParam),
		HelpID(_HelpID),
		bUp(up)

{
	Resize(size);
	enabled = false;
	highlight = false;
	selected = false;
}



void SpinButton::draw(CDrawContext *context)
{
	CColor bgColor = {95, 95, 95,  0}; 
	CColor enabledColor = {255, 255, 255,  0}; 
	CColor disabledColor = {142, 142, 142,  0}; 
	CColor selectedColor = {128, 128, 128,  0}; 

	context->setFillColor( (enabled && selected) ? selectedColor : bgColor);
	context->setFrameColor( (enabled && highlight) ? enabledColor : bgColor );
	context->fillRect(size);
	context->drawRect(size);

	context->setFrameColor( enabled ? enabledColor : disabledColor );

	int xmiddle = (size.left+size.right)/2;
	int flip = bUp ? 1 : -1;
	const int length = 5;

	CPoint top(xmiddle, bUp ? size.top+2 : size.bottom-2);

	context->moveTo(top);
	context->lineTo(CPoint(top.h - length, top.v+flip*length));

	top.v += flip;
	context->moveTo(top);
	context->lineTo(CPoint(top.h - length, top.v+flip*length));

	top.v += flip*3;
	context->moveTo(top);
	context->lineTo(CPoint(top.h - length, top.v+flip*length));

	top.v += flip;
	context->moveTo(top);
	context->lineTo(CPoint(top.h - length, top.v+flip*length));

	top = CPoint(xmiddle+1, bUp ? size.top+2 : size.bottom-2);

	context->moveTo(top);
	context->lineTo(CPoint(top.h + length, top.v+flip*length));

	top.v += flip;
	context->moveTo(top);
	context->lineTo(CPoint(top.h + length, top.v+flip*length));

	top.v += flip*3;
	context->moveTo(top);
	context->lineTo(CPoint(top.h + length, top.v+flip*length));

	top.v += flip;
	context->moveTo(top);
	context->lineTo(CPoint(top.h + length, top.v+flip*length));
}



void SpinButton::update(CDrawContext *context)
{
	CPoint where;

	context->getMouseLocation(where);

	bool NewHiLight = where.isInside(size);

	if( !(context->getMouseButtons() & kLButton) || !NewHiLight)
	{
		selected = false;
		redraw();
	}

	if(NewHiLight != highlight)
	{
		highlight = NewHiLight;
		redraw();
	}
}

void SpinButton::mouse(CDrawContext *context, CPoint &where)
{
	if(!enabled) return;

	int buttons = context->getMouseButtons();

	if(buttons&kLButton)
	{
		listener->valueChanged(context, this);
		selected = true;
		redraw();
		return;
	}

	if(buttons&kRButton)
	{
		contextmenu->SetContext((HelpIDEnum)HelpID, MIDIParam, false);
		contextmenu->Popup(context);
	}
}

void SpinButton::Resize(CRect &newsize)
{
	size = newsize;
}


void SpinButton::Enable(bool enable)
{
	if(enabled != enable)
	{
		enabled = enable;

		redraw();
	}
}

