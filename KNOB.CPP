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


//Filename:		KNOB.CPP  -  ANARCHY RHYTHMS
//Author:		Leighton Hargreaves
//Date:			25/05/2001
//Description:	


#include "MyConsts.h"
#include "MyHelpers.h"
#include "Knob.h"
#include "SleepingReporter.h"
#include "ContextMenu.h"
#include "resource/resource.h"




const float KnobDragRange = 160;



Knob::Knob(CRect &size, CControlListener *listener, int tag, CFrame &frame, int _MyIndex, char *_Name, char *_Label, SleepingReporter *_SelSoundChanger, bool InitialHighlight, ContextMenu *_contextmenu, int _HelpID, int _MIDIParam)
	:	CControl(size, listener, tag),
		contextmenu(_contextmenu),
		HelpID(_HelpID),
		MIDIParam(_MIDIParam),
		SelSoundChanger(_SelSoundChanger),
		MyIndex(_MyIndex),
		Highlight(InitialHighlight)
{
	strcpy(Name, _Name);
	strcpy(Label, _Label);
	strcpy(Display, "");

	OCBitmap = new CBitmap(frame, 80, 80);	//hard-coded resize-maximum not very portable!
	OC = NULL;

	OCSize = CRect(0, 0, size.width(), size.height());

	LabelHeight = 10;

	Adjusting = false;

	bmpKnobSmall = new CBitmap(IDB_KNOBSMALL);
	bmpKnobLarge = new CBitmap(IDB_KNOBLARGE);
}





Knob::~Knob()
{
	if(OC)
	{
		delete OC;
	}

	OCBitmap->forget(); 

	if(bmpKnobSmall)
		bmpKnobSmall->forget();

	bmpKnobSmall = NULL;

	if(bmpKnobLarge)
		bmpKnobLarge->forget();

	bmpKnobLarge = NULL;
}




void Knob::draw(CDrawContext *context)
{
	CColor BaseCol = {255, 255, 255, 0};
	CColor DimmedCol = {42, 42, 42, 0};

	const int ArrowSize = 2;

    if(!OC)
	{
		OC = new COffscreenContext (context, OCBitmap);
	}

	OC->setFillColor(ControlBk);
	OC->fillRect(CRect(-1, -1, OCSize.right, OCSize.bottom));

	OC->setFont(kNormalFontSmall);
	OC->setFontColor(BaseCol);
	OC->drawString(Name, CRect(OCSize.left, OCSize.top, OCSize.right, LabelHeight));

	CBitmap *bmpKnob = (size.width() > 30) ? bmpKnobLarge : bmpKnobSmall;

	bmpKnob->draw(OC, CRect(OCSize.left+OCSize.width()/2-bmpKnob->getWidth()/2, OCSize.top+LabelHeight, OCSize.left+OCSize.width()/2+bmpKnob->getWidth()/2, OCSize.top+LabelHeight+bmpKnob->getHeight()));

	float bearing = ((value*2)-1)*PI*3/4;

	CPoint Centre(OCSize.width()/2, bmpKnob->getHeight()*11/20 + LabelHeight);
	CPoint Arrow(	Centre.h + (int)(fsin(bearing) * (float)(bmpKnob->getWidth()/2 - ArrowSize-1)), 
					Centre.v - (int)(fcos(bearing) * (float)(bmpKnob->getWidth()/2 - ArrowSize-1)));

	DrawThickLine(OC, Centre, Arrow, 1, DimmedCol);

	if(Display[0] != 0)
	{
		strcpy(buf, Display);

		if(Label[0] != 0)
		{
			strcat(buf, " ");
			strcat(buf, Label);
		}

		OC->setFontColor(DimmedCol);
		OC->drawString(buf, CRect(OCSize.left, OCSize.bottom-LabelHeight, OCSize.right, OCSize.bottom));
	}

	OC->transfert(context, size);
}





void Knob::update(CDrawContext *context)
{
	if(Adjusting)
	{
		if(context->getMouseButtons() & kLButton)
		{
			CPoint where;

			context->getMouseLocation(where);

			if(MouseClickHeight != where.v)
			{
				value = MAX(0.0f, MIN(1.0f, MouseClickVal + (float)(MouseClickHeight-where.v)/KnobDragRange));

				listener->valueChanged(context, this);

				CControl::setValue(value);
			}
		}
		else
		{
			Adjusting = false;
		}
	}
}






void Knob::mouse(CDrawContext *context, CPoint &where)
{
	SelSoundChanger->SetInt(MyIndex);

	if(context->getMouseButtons()&kLButton)
	{
		MouseClickHeight = where.v;

		MouseClickVal = value;

		Adjusting = true;
	}
	else
	{
		contextmenu->SetContext((HelpIDEnum)HelpID, MIDIParam, true);
		contextmenu->Popup(context);
	}
}






void Knob::setValue(float val, char *display)
{
    CControl::setValue(val);

	strcpy(Display, display);
}





void Knob::Resize(CRect &newsize)
{
	size = newsize;

	OCSize = CRect(0, 0, size.width(), size.height());
}




void Knob::SetHighlight(bool _Highlight)
{
	if(Highlight != _Highlight)
	{
		Highlight = _Highlight;
		redraw();
	}
}


//End of file