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


//IOSlider.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02


#include "MyConsts.h"
#include "IoSlider.h"
#include "MyHelpers.h"
#include "ContextMenu.h"
#include "MidiMap_v2.h"
#include "resource/resource.h"



const int EdgeMargin = 0;
const int TopMargin = 12;

const int sliderheight = 12;
const int sliderwidth = 20;



IOSlider::IOSlider(CRect& size, CControlListener* listener, int tag, bool _isOut, ContextMenu *_contextmenu)
	:	CControl(size, listener, tag),
	isOut(_isOut),
	contextmenu(_contextmenu)
{
	value = 0;

	Dragging = false;

	bmpSlider = new CBitmap(IDB_IOSLIDER);

	Resize(size);
}




IOSlider::~IOSlider()
{
	if(bmpSlider)
		bmpSlider->forget();

	bmpSlider = NULL;
}


		
void IOSlider::draw(CDrawContext *context)
{
	CColor Blue = {9, 5, 60, 0};
	CColor Text = {171, 155, 129, 0};
	CColor MiddleGrey = {127, 127, 127, 0};
	CColor OuterGrey = {62, 60, 91, 0};

	//clear area

	context->setFillColor(Blue);
	context->fillRect(size);

	//draw main bar

	int ActiveAreaMiddle = (ActiveArea.left + ActiveArea.right)/2;

	context->setFillColor(OuterGrey);
	context->fillRect(CRect(size.left+size.width()/2-3, ActiveArea.top, size.left+size.width()/2+3, ActiveArea.bottom));
	context->setFillColor(MiddleGrey);
	context->fillRect(CRect(size.left+size.width()/2-2, ActiveArea.top+1, size.left+size.width()/2+2, ActiveArea.bottom-1));

	int SliderY = ActiveArea.bottom - (int)(value*(float)(ActiveArea.height()-sliderheight)/2.f) - sliderheight;

	CRect Pos(size.left+size.width()/2-sliderwidth/2, SliderY, size.left+size.width()/2+sliderwidth/2, SliderY+sliderheight);

	bmpSlider->draw(context, Pos);

	//caption
	context->setFont(kNormalFontSmall);
	context->setFontColor(Text);

	context->drawString(isOut ? "OUT" : "IN", CaptionArea);
}




void IOSlider::mouse(CDrawContext *context, CPoint &where)
{
	if(context->getMouseButtons()&kLButton)
	{
		if(where.isInside(ActiveArea))
		{
			value = MAX(0.f, MIN(2.f, 2.f*(float)(ActiveArea.bottom - sliderheight/2 - where.v)/(float)(ActiveArea.height()-sliderheight)));	
		
			Dragging = true;

			listener->valueChanged(context, this);
		}
	}
	else
	{
		if(isOut)
		{
			contextmenu->SetContext(HIDLevels, ExternalMIDIMap_v2::kMIDIOutput, false);
		}
		else
		{
			contextmenu->SetContext(HIDLevels, ExternalMIDIMap_v2::kMIDIInput, false);
		}

		contextmenu->Popup(context);
	}
}




void IOSlider::update(CDrawContext *context)
{
	if(Dragging && (context->getMouseButtons()&kLButton))
	{
		CPoint where;

		context->getMouseLocation(where);

		float newval = MAX(0.f, MIN(2.f, 2.f*(float)(ActiveArea.bottom - sliderheight/2 - where.v)/(float)(ActiveArea.height()-sliderheight)));	
			
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




void IOSlider::Resize(CRect &newsize)
{
	size = newsize;

	if(isOut)
	{
		CaptionArea(size.left+EdgeMargin, size.top, size.right, size.top+TopMargin);
		ActiveArea(size.left+EdgeMargin, size.top+TopMargin, size.right, size.bottom);
		LabelArea(size.left, size.top+TopMargin, size.left+EdgeMargin, size.bottom);
	}
	else
	{
		CaptionArea(size.left, size.top, size.right-EdgeMargin, size.top+TopMargin);
		ActiveArea(size.left, size.top+TopMargin, size.right-EdgeMargin, size.bottom);
		LabelArea(size.right-EdgeMargin, size.top+TopMargin, size.right, size.bottom);
	}
}