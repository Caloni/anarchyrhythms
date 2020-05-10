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


//LevelSlider.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02


#include "MyConsts.h"
#include "LevelSlider.h"
#include "MyHelpers.h"
#include "SleepingReporter.h"
#include "ContextMenu.h"
#include "MidiMap_v2.h"

#include "resource/resource.h"


const int SliderWidth = 40;
const int SliderSize = 16;






LevelSlider::LevelSlider(CRect& size, CControlListener* listener, int tag, int _MyIndex, CFrame &frame, SleepingReporter *_SelSoundChanger, bool InitialHighlight, ContextMenu *_contextmenu)
	:	CControl(size, listener, tag),
		contextmenu(_contextmenu),
		SelSoundChanger(_SelSoundChanger),
		MyIndex(_MyIndex),
		Highlight(InitialHighlight)
{
	value = 0;

	Dragging = false;

	bmpSlider = new CBitmap(IDB_LEVELSLIDER);
}




LevelSlider::~LevelSlider()
{
	if(bmpSlider)
		bmpSlider->forget();

	bmpSlider = NULL;
}



		
void LevelSlider::draw(CDrawContext *context)
{
	CColor BaseCol, DimmedCol;

	if(Highlight)
	{
		BaseCol = LightSoundCol[MyIndex];
		DimmedCol = SoundCol[MyIndex];
	}
	else
	{
		BaseCol = SoundCol[MyIndex];
		DimmedCol = DarkSoundCol[MyIndex];
	}

	context->setFillColor(ControlBk);
	context->fillRect(size);

	context->setFillColor(DimmedCol);
	context->fillRect(CRect(size.left+size.width()/2-6, size.top, size.left+size.width()/2+6, size.bottom));
	context->setFillColor(kBlackCColor);
	context->fillRect(CRect(size.left+size.width()/2-2, size.top+5, size.left+size.width()/2+2, size.bottom-5));

	context->setFillColor(BaseCol);

	int SliderHeight = size.bottom - (int)(value*(float)(size.height()-SliderSize)) - SliderSize + 1;

	bmpSlider->draw(context, CRect(size.left+size.width()/2-SliderWidth/2, SliderHeight, size.left+size.width()/2+SliderWidth/2, SliderHeight + SliderSize));
}




void LevelSlider::mouse(CDrawContext *context, CPoint &where)
{
	SelSoundChanger->SetInt(MyIndex);

	if(context->getMouseButtons() & kLButton)
	{
		value = MAX(0.f, MIN(1.f, 1.f-(float)(where.v - size.top - SliderSize/2)/(float)(size.height()-SliderSize)));	
		
		Dragging = true;

		listener->valueChanged(context, this);
	}
	else
	{
		contextmenu->SetContext(HIDMixerLevel, ExternalMIDIMap_v2::numMIDIParams+MyIndex*ExternalMIDIMap_v2::numMIDISoundParams+ExternalMIDIMap_v2::kMIDILevel, true);
		contextmenu->Popup(context);
	}
}



void LevelSlider::update(CDrawContext *context)
{
	if(Dragging && (context->getMouseButtons()&kLButton))
	{
		CPoint where;

		context->getMouseLocation(where);

		float newval = MAX(0.f, MIN(1.f, 1.f-(float)(where.v - size.top - SliderSize/2)/(float)(size.height()-SliderSize)));	
		
		if(newval != value)
		{
			value = newval;
			listener->valueChanged(context, this);
		}
	}
	else
	{
		Dragging = false;
	}
}



void LevelSlider::SetHighlight(bool _Highlight)
{
	if(Highlight != _Highlight)
	{
		Highlight = _Highlight;
		redraw();
	}
}