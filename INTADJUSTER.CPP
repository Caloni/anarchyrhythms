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


//IntAdjuster.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2



#include "IntAdjuster.h"
#include "MyHelpers.h"
#include "MyConsts.h"
#include "ContextMenu.h"




IntAdjuster::IntAdjuster(CRect size, CControlListener *Listener, int tag, int _min, int _max, char *_Caption1, char *_Caption2, char *_Label, char *_UnitLabel, int _ValDisplayWidth, IntAdjusterType _AdjusterType, ContextMenu *_contextmenu, int _HelpID, int _MIDIParam, bool _IsPartOfSound)
	:	CControl(size, Listener, tag),
		contextmenu(_contextmenu),
		HelpID(_HelpID),
		IsPartOfSound(_IsPartOfSound),
		MIDIParam(_MIDIParam),
		HiLight(false),
		ValDisplayWidth(_ValDisplayWidth),
		AdjusterType(_AdjusterType)
{
	SetColor(ControlBk, ControlBk, kWhiteCColor);
		
	strcpy(Label, _Label);
	strcpy(UnitLabel, _UnitLabel);
	strcpy(Caption1, _Caption1);

	if(_Caption2)
	{	
		strcpy(Caption2, _Caption2);
	}
	else
	{
		Caption2[0]=0;
	}

	strcpy(ValStr, "");

	max = _max;
	min = _min;

	Val = 0;

	Resize(size);
}



void IntAdjuster::draw(CDrawContext *context)
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
	context->drawRect(ValDisplay);

	context->setFont(kNormalFontSmall);
	context->setFontColor(Foreground);

	if(Caption2[0]==0)
	{
		context->drawString(Caption1, Caption1Display);
	}
	else
	{
		context->drawString(Caption1, Caption1Display);
		context->drawString(Caption2, Caption2Display);
	}

	context->drawString(ValStr, ValDisplay);
}



void IntAdjuster::update(CDrawContext *context)
{
	CPoint where;

	context->getMouseLocation(where);

	bool NewHiLight = where.isInside(ValDisplay);

	if(NewHiLight != HiLight)
	{
		HiLight= NewHiLight;
		redraw();
	}
}



void IntAdjuster::mouse(CDrawContext *context, CPoint &where)
{
	int buttons = context->getMouseButtons();

	if(where.isInside(ValDisplay))
	{
		int dir = (buttons & kLButton) ? -1 : 1; 
		int amount = (buttons&kShift) || (buttons&kControl) || (buttons&kAlt) ? (((AdjusterType==IATPitch)||(AdjusterType==IATPitchEnvelopeRange)) ?12:10) : 1;
		
		if(AdjusterType==IATLength)
		{
			if(MIN(Val, Val + dir*amount*100) >= 1000)
			{
				amount*=100;
			}
			else
			{
				if(MIN(Val, Val + dir*amount*10) >= 100)
				{
					amount*=10;
				}
			}
		}

		MySetValue(MAX(min, MIN(max, Val + dir*amount)));
		listener->valueChanged(context, this);
	}
	else
	{
		if(buttons&kRButton)
		{
			contextmenu->SetContext((HelpIDEnum)HelpID, MIDIParam, IsPartOfSound);
			contextmenu->Popup(context);
		}
	}
}




void IntAdjuster::MySetValue(int val)
{
	Val = val;

	switch(AdjusterType)
	{
		case IATNormal:
		case IATPitchEnvelopeRange:

			itoa(val, ValStr);

			if(MyGetValue() == 1)
			{
				strcat(ValStr, UnitLabel);
			}
			else
			{
				strcat(ValStr, Label);
			}

			break;

		case IATLength:

			ValStr[0] = val/1000 + '0';
			ValStr[1] = '.';
			ValStr[2] = (val/100)%10 + '0';
			ValStr[3] = (val/10)%10 + '0';
			ValStr[4] = val%10 + '0';
			ValStr[5] = 0;

			if(MyGetValue() == 1000)
			{
				strcat(ValStr, UnitLabel);
			}
			else
			{
				strcat(ValStr, Label);
			}

			break;

		case IATPitch:

			switch(val%12)
			{	
				case 0:		strcpy(ValStr, "C");	break;
				case 1:		strcpy(ValStr, "C#");	break;
				case 2:		strcpy(ValStr, "D");	break;
				case 3:		strcpy(ValStr, "D#");	break;
				case 4:		strcpy(ValStr, "E");	break;
				case 5:		strcpy(ValStr, "F");	break;
				case 6:		strcpy(ValStr, "F#");	break;
				case 7:		strcpy(ValStr, "G");	break;
				case 8:		strcpy(ValStr, "G#");	break;
				case 9:		strcpy(ValStr, "A");	break;
				case 10:	strcpy(ValStr, "A#");	break;
				case 11:	strcpy(ValStr, "B");	break;
			}

			itoa(val/12, &(ValStr[strlen(ValStr)]));

			strcat(ValStr, Label);

			break;
	}
}



int IntAdjuster::MyGetValue()
{
	return Val;
	
}



void IntAdjuster::Resize(CRect &newsize)
{
	size = newsize;

	if(Caption2[0]==0)
	{
		ValDisplay = CRect(size.right-ValDisplayWidth, size.top, size.right, size.bottom);
		Caption1Display = CRect(size.left, size.top, ValDisplay.left, size.bottom);
	}
	else
	{
		ValDisplay = CRect(size.right-ValDisplayWidth, size.top+size.height()/4, size.right, size.bottom-size.height()/4);
		Caption1Display = CRect(size.left, size.top, ValDisplay.left, size.top+size.height()/2);
		Caption2Display = CRect(size.left, size.top+size.height()/2, ValDisplay.left, size.bottom);
	}
}








void IntAdjuster::SetColor(CColor _Background, CColor _Nearground, CColor _Foreground)
{
	Background = _Background;
	Nearground = _Nearground;
	Foreground = _Foreground;
}
