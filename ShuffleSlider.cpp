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


//ShuffleSlider.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02


#include "MyConsts.h"
#include "ShuffleSlider.h"
#include "MyHelpers.h"
#include "ContextMenu.h"
#include "MIDIMap_v2.h"

#include "resource/resource.h"


const int SliderSize = 7;
const int SliderHeight = 14;
const int EdgeMargin = 5;


ShuffleSlider::ShuffleSlider(CRect& size, CControlListener* listener, int tag, CFrame &frame, ContextMenu *_contextmenu)
	:	CControl(size, listener, tag),
		contextmenu(_contextmenu)
{
	value = 0;

	Dragging = false;

	bmpSlider = new CBitmap(IDB_SHUFFLESLIDER);
}




ShuffleSlider::~ShuffleSlider()
{
	if(bmpSlider)
		bmpSlider->forget();

	bmpSlider = NULL;
}


	
void ShuffleSlider::draw(CDrawContext *context)
{
	CColor Bg = {95, 95, 95, 0};
	CColor Middle = {9, 5, 60, 0};
	CColor Outer = {180, 180, 180, 0};

	context->setFillColor(Bg);
	context->fillRect(size);

	context->setFillColor(Outer);
	context->fillRect(CRect(size.left+EdgeMargin, size.top+size.height()/2-3, size.right - EdgeMargin, size.top+size.height()/2+3));

	context->setFillColor(Middle);
	context->fillRect(CRect(size.left+EdgeMargin+1, size.top+size.height()/2-2, size.right - EdgeMargin-1, size.top+size.height()/2+2));

	int SliderX = size.left + EdgeMargin + (int)(value*(float)(size.width()-EdgeMargin*2-SliderSize));

	CRect Pos(SliderX, size.top+1, SliderX+SliderSize, size.bottom);
	bmpSlider->draw(context, Pos); 
}


void ShuffleSlider::mouse(CDrawContext *context, CPoint &where)
{
	if(context->getMouseButtons()&kLButton)
	{
		value = MAX(0.f, MIN(1.f, (float)(where.h - size.left - SliderSize/2 - EdgeMargin)/(float)(size.width()-SliderSize-EdgeMargin*2)));	
		
		Dragging = true;

		listener->valueChanged(context, this);
	}
	else
	{
		contextmenu->SetContext(HIDShuffle, ExternalMIDIMap_v2::kShuffleAmount, false);
		contextmenu->Popup(context);
	}
}




void ShuffleSlider::update(CDrawContext *context)
{
	if(Dragging && (context->getMouseButtons()&kLButton))
	{
		CPoint where;

		context->getMouseLocation(where);

		float newval = MAX(0.f, MIN(1.f, (float)(where.h - size.left - SliderSize/2 - EdgeMargin)/(float)(size.width()-SliderSize-EdgeMargin*2)));	
		
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




