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


//Envelope.cpp	-	Anarchy Rhythms

//Leighton Hargreaves 6-2-2


#include "Envelope.h"
#include "SleepingReporter.h"
#include "MyConsts.h"
#include "MyHelpers.h"
#include "ContextMenu.h"


const int ArrowHeadSize = 3;
const int ControlPointRadius = 5;



Envelope::Envelope(CRect size, CControlListener *listener, int tag, ExternalStateData_v2 *_ExternalState, SleepingReporter **_sleepingreporter, EnvelopeEnum _EnvelopeType, ContextMenu *_contextmenu, int _HelpID)
	:	CControl(size, listener, tag),
		contextmenu(_contextmenu),
		HelpID(_HelpID),
		ExternalState(_ExternalState),
		sleepingreporter(_sleepingreporter),
		EnvelopeType(_EnvelopeType),
		DraggingPoint(-1)		
{
	ActiveArea = CRect(size.left + 26, size.top + ArrowHeadSize + ControlPointRadius, size.right - ArrowHeadSize - ControlPointRadius, size.bottom - ArrowHeadSize-ControlPointRadius);

	data = &(ExternalState->Sound[ExternalState->SelectedSound].envelopedata[EnvelopeType]);
}





void Envelope::Resize(CRect &newsize)
{
	size = newsize;
	ActiveArea = CRect(size.left + 26, size.top + ArrowHeadSize + ControlPointRadius, size.right - ArrowHeadSize - ControlPointRadius, size.bottom - ArrowHeadSize-ControlPointRadius);
	DraggingPoint = -1;
}


void Envelope::draw(CDrawContext *context)
{
	int SelectedIndex = ExternalState->SelectedSound;

	if(SelectedIndex < 0)		//if no sound is selected
	{
		return;
	}

	CColor Col = LightSoundCol[SelectedIndex];
	CColor SaturatedCol = SaturatedSoundCol[SelectedIndex];

	//clear rect

	context->setFillColor(SaturatedCol);
	context->fillRect(size);

	context->setFillColor(DarkSoundCol[SelectedIndex]);
	context->fillRect(ActiveArea);

	//draw axes
	
	switch(EnvelopeType)
	{
		case envLevel:
		case envFilterWidth:

			DrawArrow(context, CPoint(ActiveArea.left, ActiveArea.bottom), ActiveArea.width()+ArrowHeadSize*2, 0, SoundCol[SelectedIndex], ArrowHeadSize);
			DrawArrow(context, CPoint(ActiveArea.left, ActiveArea.bottom), 0, -ActiveArea.height()-ArrowHeadSize-ControlPointRadius, SoundCol[SelectedIndex], ArrowHeadSize);
			break;

		default:

			DrawArrow(context, CPoint(ActiveArea.left, ActiveArea.top + ActiveArea.height()/2), size.right-ActiveArea.left, 0, SoundCol[SelectedIndex], ArrowHeadSize);
			DrawArrow(context, CPoint(ActiveArea.left, ActiveArea.top + ActiveArea.height()/2), 0, ActiveArea.height()/2+ArrowHeadSize+ControlPointRadius, SoundCol[SelectedIndex], ArrowHeadSize);
			DrawArrow(context, CPoint(ActiveArea.left, ActiveArea.top + ActiveArea.height()/2), 0, -ActiveArea.height()/2-ArrowHeadSize-ControlPointRadius, SoundCol[SelectedIndex], ArrowHeadSize);
			break;
	}

	//draw y-axis labels

	context->setFont(kNormalFontVerySmall);
	context->setFontColor(Col);

	static char Str[8];

	switch(EnvelopeType)
	{
		case envLevel:

			itoa(ExternalState->Sound[SelectedIndex].Level, Str);
			strcat(Str, "%");
			context->drawString(Str, CRect(size.left, ActiveArea.top, ActiveArea.left, ActiveArea.top+10));
			context->drawString("0%", CRect(size.left, ActiveArea.bottom-10, ActiveArea.left, ActiveArea.bottom));
			break;

		case envFilterWidth:

			itoa(ExternalState->Sound[SelectedIndex].FilterWidth, Str);
			strcat(Str, "%");
			context->drawString(Str, CRect(size.left, ActiveArea.top, ActiveArea.left, ActiveArea.top+10));
			context->drawString("0%", CRect(size.left, ActiveArea.bottom-10, ActiveArea.left, ActiveArea.bottom));
			break;

		case envPan:

			context->drawString("L", CRect(size.left, ActiveArea.top, ActiveArea.left, ActiveArea.top+10));
			context->drawString("R", CRect(size.left, ActiveArea.bottom-10, ActiveArea.left, ActiveArea.bottom));
			break;

		case envOscillator:

			context->drawString("osc1", CRect(size.left, ActiveArea.top, ActiveArea.left, ActiveArea.top+10));
			context->drawString("osc2", CRect(size.left, ActiveArea.bottom-10, ActiveArea.left, ActiveArea.bottom));
			break;

		case envPitch:

			strcpy(Str, "+");
			itoa(ExternalState->Sound[SelectedIndex].PitchEnvelopeRange, Str+1);

			context->drawString(Str, CRect(size.left, ActiveArea.top, ActiveArea.left, ActiveArea.top+10));
			Str[0]='-';
			context->drawString(Str, CRect(size.left, ActiveArea.bottom-10, ActiveArea.left, ActiveArea.bottom));
			break;
	}

	//draw control points

	context->setFillColor(Col);
	context->setFrameColor(Col);

	for(int i=0; i<data->NumPoints; i++)
	{
		CPoint where;

		GetControlPointPos(where, i);

		CRect pos(where.h-ControlPointRadius, where.v-ControlPointRadius, where.h+ControlPointRadius, where.v+ControlPointRadius);
		
		context->fillEllipse(pos);

		if(i==0)
		{
			context->moveTo(where);
		}
		else
		{
			context->lineTo(where);
		}
	}
}



void Envelope::mouse(CDrawContext *context, CPoint &where)
{
	DraggingPoint = -1;

	for(int i=0; i<data->NumPoints; i++)
	{
		CPoint pointwhere;

		GetControlPointPos(pointwhere, i);

		if((pointwhere.h-where.h)*(pointwhere.h-where.h)+(pointwhere.v-where.v)*(pointwhere.v-where.v) <= ControlPointRadius*ControlPointRadius)
		{
			DraggingPoint = i;
			break;
		}
	}

	if(DraggingPoint >= 0)	//if a previous point was selected
	{
		if(context->getMouseButtons() & kRButton)	//Rclick to delete points
		{
			if(IsPointDeletable())
			{
				for(i=DraggingPoint; i<data->NumPoints-1; i++)
				{
					data->PointX[i] = data->PointX[i+1];
					data->PointY[i] = data->PointY[i+1];
				}

				data->NumPoints--;
				redraw();
			}
		}
		else										//Lclick to move the point
		{
			SetControlPointPos(where);
			redraw();
		}
	}
	else	//a previous point wasn't selected
	{
		if(context->getMouseButtons() & kLButton)	//Lclick a new point is born
		{
			if((data->NumPoints < MaxEnvelopePoints) && where.isInside(ActiveArea))
			{
				data->PointX[data->NumPoints] = data->PointX[data->NumPoints-1];
				data->PointY[data->NumPoints] = data->PointY[data->NumPoints-1];
				DraggingPoint = data->NumPoints-1;
				data->NumPoints++;
				SetControlPointPos(where);
				redraw();
			}
		}
		else										//Rclick brings up "what's this" menu?
		{
			contextmenu->SetContext((HelpIDEnum)HelpID, -1, true);
			contextmenu->Popup(context);
		}
	}
}





void Envelope::update(CDrawContext *context)
{
	int Buttons = context->getMouseButtons();

	if((Buttons & kLButton) && (DraggingPoint >= 0))		//still dragging
	{
		CPoint mousewhere;

		context->getMouseLocation(mousewhere);

		float PrevX = data->PointX[DraggingPoint];
		float PrevY = data->PointX[DraggingPoint];
		
		SetControlPointPos(mousewhere);

		if((data->PointX[DraggingPoint] != PrevX) || (data->PointY[DraggingPoint] != PrevY))
		{
			redraw();
		}
	}
	else
	{
		DraggingPoint = -1;
	}
}






void Envelope::ValueChanged()
{
	data = &(ExternalState->Sound[ExternalState->SelectedSound].envelopedata[EnvelopeType]);

	redraw();
}



void Envelope::GetControlPointPos(CPoint &pos, int CPNum)
{
	pos((int)(data->PointX[CPNum] * (float)ActiveArea.width()) + ActiveArea.left, 
		ActiveArea.bottom - (int)(data->PointY[CPNum] * (float)ActiveArea.height()));
}



void Envelope::SetControlPointPos(CPoint &pos)		//sets point pointed to by DraggingPoint, modifies Dragging point if needs to re-order
{
	const float Threshold = 0.8f;	//Level envelope implicit normalise threshold

	bool LockYtoThreshold = false;

	if(EnvelopeType==envLevel)
	{
		if(data->PointY[DraggingPoint] >= Threshold)	//this might be highest point
		{
			for(int i=0; i<data->NumPoints; i++)
			{
				if((i!=DraggingPoint) && (data->PointY[i] >= Threshold))
				{
					break;
				}
			}

			if(i==data->NumPoints)	
			{
				LockYtoThreshold = true;
			}
		}
	}

	//set new positions			
	if((DraggingPoint>0) && (DraggingPoint<data->NumPoints-1))	//lock end-point X
	{
		data->PointX[DraggingPoint] = MAX(0.f, MIN(1.f, (float)(pos.h-ActiveArea.left) / (float)ActiveArea.width()));
	}

																//lock end-point Y
	if((EnvelopeType != envLevel) || ((DraggingPoint>0) && (DraggingPoint<data->NumPoints-1)))
	{
		data->PointY[DraggingPoint] = MAX(LockYtoThreshold ? Threshold : 0.f, MIN(1.f, (float)(ActiveArea.bottom-pos.v) / (float)ActiveArea.height()));
	}

	//sort into order

	while((DraggingPoint < (data->NumPoints-1)) && (data->PointX[DraggingPoint+1] < data->PointX[DraggingPoint]))
	{
		float temp = data->PointX[DraggingPoint+1];
		data->PointX[DraggingPoint+1] = data->PointX[DraggingPoint];
		data->PointX[DraggingPoint] = temp;
		temp = data->PointY[DraggingPoint+1];
		data->PointY[DraggingPoint+1] = data->PointY[DraggingPoint];
		data->PointY[DraggingPoint] = temp;

		DraggingPoint++;
	}

	while((DraggingPoint > 0) && (data->PointX[DraggingPoint-1] > data->PointX[DraggingPoint]))
	{
		float temp = data->PointX[DraggingPoint-1];
		data->PointX[DraggingPoint-1] = data->PointX[DraggingPoint];
		data->PointX[DraggingPoint] = temp;
		temp = data->PointY[DraggingPoint-1];
		data->PointY[DraggingPoint-1] = data->PointY[DraggingPoint];
		data->PointY[DraggingPoint] = temp;

		DraggingPoint--;
	}
}



bool Envelope::IsPointDeletable()
{
	if((DraggingPoint==0) || (DraggingPoint==data->NumPoints-1))
	{
		return false;
	}

	if((EnvelopeType == envLevel) && (data->NumPoints==3))
	{
		return false;
	}

	return true;
}