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


//SpinButton.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2



#include "SpinButton.h"
#include "MyHelpers.h"
#include "MyConsts.h"
#include "ContextMenu.h"




SpinButton::SpinButton(CRect size, CControlListener *Listener, int tag, CBitmap *_enabledbitmap, CBitmap *_disabledbitmap, ContextMenu *_contextmenu, int _HelpID, int _MIDIParam)
	:	CControl(size, Listener, tag),
		contextmenu(_contextmenu),
		HelpID(_HelpID),
		enabledBitmap(_enabledbitmap),
		disabledBitmap(_disabledbitmap)
{
	Resize(size);
	enabled = false;
	highlight = false;
}



void SpinButton::draw(CDrawContext *context)
{
	if(enabled)
	{
		enabledBitmap->draw(context, size); 
	}
	else
	{
		disabledBitmap->draw(context, size); 
	}

	if(highlight)
	{
		CColor frameColor = {255, 255, 255,  0}; 
		context->setFrameColor(frameColor);
		context->drawRect(size);
	}
}



void SpinButton::update(CDrawContext *context)
{
	CPoint where;

	context->getMouseLocation(where);

	bool NewHiLight = where.isInside(size);

	if(NewHiLight != highlight)
	{
		highlight = NewHiLight;
		redraw();
	}
}

void SpinButton::mouse(CDrawContext *context, CPoint &where)
{
	if(!enabled) return;

	int SpinButtons = context->getMouseSpinButtons();

	if(SpinButtons&kLSpinButton)
	{
		listener->valueChanged(context, this);
		return;
	}

	if(SpinButtons&kRSpinButton)
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

