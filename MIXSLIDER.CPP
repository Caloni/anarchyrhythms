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


//MixSlider.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02


#include "MyConsts.h"
#include "MixSlider.h"
#include "MyHelpers.h"
#include "ContextMenu.h"
#include "MIDIMap_v2.h"

#include "resource/resource.h"


const int SliderSize = 40;
const int SliderHeight = 20;
const int EdgeMargin = 50;

const int PupilRadius = 3;





MixSlider::MixSlider(CRect& size, CControlListener* listener, int tag, CFrame &frame, ContextMenu *_contextmenu)
	:	CControl(size, listener, tag),
		contextmenu(_contextmenu)
{
	value = 0;

	Dragging = false;

	bmpSlider = new CBitmap(IDB_MIXSLIDER);
}




MixSlider::~MixSlider()
{
	if(bmpSlider)
		bmpSlider->forget();

	bmpSlider = NULL;
}




		
void MixSlider::draw(CDrawContext *context)
{
	CColor Blue = {9, 5, 60, 0};
	CColor MiddleGrey = {127, 127, 127, 0};
	CColor OuterGrey = {62, 60, 91, 0};
	CColor Text = {208, 190, 145, 0};

	context->setFillColor(Blue);
	context->fillRect(size);

	context->setFillColor(OuterGrey);
	context->fillRect(CRect(size.left+EdgeMargin, size.top+size.height()/2-3, size.right - EdgeMargin, size.top+size.height()/2+3));

	context->setFillColor(MiddleGrey);
	context->fillRect(CRect(size.left+EdgeMargin+1, size.top+size.height()/2-2, size.right - EdgeMargin-1, size.top+size.height()/2+2));

	int SliderX = size.left + EdgeMargin + (int)(value*(float)(size.width()-EdgeMargin*2-SliderSize));

	CRect Pos(SliderX, size.top+MAX(1, size.height()/2-SliderHeight/2), SliderX+SliderSize, MIN(size.bottom-1, size.top+size.height()/2+SliderHeight/2));
	bmpSlider->draw(context, Pos); 

	//Wet-Dry labels
	context->setFont(kNormalFontVeryBig);
	context->setFontColor(Text);
	context->drawString("DRY", CRect(size.left, size.top, size.left+EdgeMargin, size.bottom));
	context->drawString("WET", CRect(size.right-EdgeMargin, size.top, size.right, size.bottom));
}




void MixSlider::mouse(CDrawContext *context, CPoint &where)
{
	if(context->getMouseButtons()&kLButton)
	{
		value = MAX(0.f, MIN(1.f, (float)(where.h - size.left - SliderSize/2 - EdgeMargin)/(float)(size.width()-SliderSize-EdgeMargin*2)));	
		
		Dragging = true;

		listener->valueChanged(context, this);
	}
	else
	{
		contextmenu->SetContext(HIDLevels, ExternalMIDIMap_v2::kMIDIDryWet, false);
		contextmenu->Popup(context);
	}
}




void MixSlider::update(CDrawContext *context)
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




