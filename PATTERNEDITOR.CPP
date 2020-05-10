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


//PatternEditor.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 15-1-02


#include "PatternEditor.h"
#include "MyHelpers.h"
#include "InternalStateData.h"
#include "ExternalStateData_v2.h"
#include "SleepingReporter.h"
#include "ContextMenu.h"

const int LabelMargin = 16;

PatternEditor::PatternEditor(CRect &size, ExternalStateData_v2 *_ExternalState, SleepingReporter *_SelSoundChanger, InternalStateData *_InternalState, CFrame &_frame, ContextMenu *_contextmenu)
	:	CControl(size, NULL, 0), 
		ExternalState(_ExternalState),
		SoundClickedOn(-1),
		StepClickedOn(-1),
		NewClickState(false),
		SelSoundChanger(_SelSoundChanger),
		SelSound(-1),
		InternalState(_InternalState),
		frame(_frame),
		contextmenu(_contextmenu)
{
	OCBitmap = new CBitmap(frame, 900, 268);	//hard-coded resize-maximum not very portable!
	OC = NULL;

	OCSize = CRect(0, 0, size.width(), size.height());

	for(int i=0; i<NumSounds; i++)
	{
		PrevStepPlaying[i] = -1;
	}
}




PatternEditor::~PatternEditor()
{
	if(OC)
	{
		delete OC;
	}

	OCBitmap->forget();
}


inline void PatternEditor::getCell(const int &step, const int &sound, CRect &where)
{
	PatternData_v2 &Pattern = ExternalState->getPattern();

	int steps = Pattern.Beats*Pattern.Steps;
	float stepsInv = 1.f / steps;

	int left = OCSize.left + (int)(InternalState->GetShuffle(step) * OCSize.width() * stepsInv);
	int right;
	if((step + 1) < steps)
	{
		right = OCSize.left + (int)(InternalState->GetShuffle(step+1) * OCSize.width() * stepsInv);
	}
	else
	{
		right=OCSize.right;
	}

	int top = OCSize.top + sound*m_soundHeight+LabelMargin;
	int bottom = top+m_soundHeight;

	where(left, top, right, bottom);
}


inline int PatternEditor::stepFromPoint(const CPoint &point)
{
	PatternData_v2 &Pattern = ExternalState->getPattern();

	CRect where;
	for(int i=0; i<Pattern.Beats*Pattern.Steps; i++)
	{
		getCell(i, 0, where);

		if( (where.left <= point.h) && (where.right > point.h))
		{
			return i;
		}
	}

	//if we get here, its off the left or right
	if(point.h < (OCSize.left+OCSize.right)/2)
		return 0;	
	else
		return i-1;
}



inline void PatternEditor::drawelement(CDrawContext *context, CRect &where, const int &step, const int &sound)
{
	PatternData_v2 &Pattern = ExternalState->getPattern();
	
	//establish where
	getCell(step, sound, where);

	//clear background

	OC->setFillColor(ControlBk);
	OC->fillRect(where);

	//draw vert lines

	if((step%Pattern.Steps)==0)
	{
		OC->setFrameColor(kWhiteCColor);
	}
	else
	{
		const CColor DarkGrey = {64, 64, 64, 0};
		OC->setFrameColor(DarkGrey);
	}

	OC->moveTo(CPoint(where.left, where.top));
	OC->lineTo(CPoint(where.left, where.bottom));

	//draw horiz lines

	if(sound == SelSound)
	{
		OC->setFrameColor(SoundCol[sound]);
		OC->setFillColor(LightSoundCol[sound]);
	}
	else
	{
		OC->setFrameColor(DarkSoundCol[sound]);
		OC->setFillColor(SoundCol[sound]);
	}

	OC->moveTo(CPoint((step==0) ? where.left+3 : where.left, (where.top+where.bottom)/2));
	OC->lineTo(CPoint(where.right, (where.top+where.bottom)/2));

	//draw the note if...
	if(Pattern.Data[sound][step])
	{
		if(InternalState->Sound[sound].StepPlaying == step)
		{
			OC->setFillColor( (sound == SelSound) ? White : VeryLightSoundCol[sound]);
			//OC->setFillColor( Black );

			PrevNoteID[sound] = InternalState->Sound[sound].NoteID;
			PrevStepPlaying[sound] = InternalState->Sound[sound].StepPlaying;
		}
		int width = MAX(where.width()/2, 4);

		OC->fillRect(CRect(where.left, where.top + 3, where.left + width, where.bottom - 3));
	}
}



void PatternEditor::draw(CDrawContext *context)
{
	PatternData_v2 &Pattern = ExternalState->getPattern();

    if(!OC)
	{
		OC = new COffscreenContext (context, OCBitmap);
	}
	
	updateSoundHeight();

	CRect where;

	//clear background

	OC->setFillColor(ControlBk);
	OC->setFrameColor(ControlBk);
	OC->fillRect(OCSize);
	OC->drawRect(OCSize);

	OC->setFrameColor(kWhiteCColor);

	getCell(0, 0, where);
	OC->moveTo(CPoint(where.left, OCSize.top+LabelMargin-1));
	OC->lineTo(CPoint(OCSize.right, OCSize.top+LabelMargin-1));

	//draw labels

	char str[3];

	OC->setFont(kNormalFont);
	OC->setFontColor(kWhiteCColor);

	for(int beat = 0; beat<Pattern.Beats; beat++)
	{
		itoa(beat+1, str, 10);
		
		getCell(beat*Pattern.Steps, 0, where);

		OC->drawString(str, CRect(where.left, OCSize.top, where.right+20, OCSize.top+LabelMargin), false, kLeftText);
	}

	//draw pattern

	for(int sound=0; sound<NumSounds; sound++)
	{
		for(int step = 0; step<Pattern.Beats*Pattern.Steps; step++)
		{
			drawelement(context, where, step, sound);
		}
	}

	OC->transfert(context, size);
}







void PatternEditor::mouse(CDrawContext *context, CPoint &where)
{
	PatternData_v2 &Pattern = ExternalState->getPattern();

	updateSoundHeight();

	CPoint MyCoords(where.h - size.left, where.v - (size.top + LabelMargin));

	int step = stepFromPoint(MyCoords.h);
	int sound = MyCoords.v / m_soundHeight;

	if(sound >= NumSounds) sound = NumSounds-1;
	if(sound < 0) sound = 0;
	SelSoundChanger->SetInt(sound);

	if(context->getMouseButtons() & kLButton)
	{
		if((step >= 0) && (step < (Pattern.Beats*Pattern.Steps)) && (sound >= 0) && (sound < NumSounds))
		{
			NewClickState = !Pattern.Data[sound][step];
			
			Pattern.Data[sound][step] = NewClickState;

			StepClickedOn = step;
			SoundClickedOn = sound;

			redraw();
		}		
	}
	else
	{
		contextmenu->SetContext(HIDPatternEditor, -1, true);
		contextmenu->Popup(context);
	}
}



inline void PatternEditor::updateSoundHeight()
{
	m_soundHeight = (size.height()-LabelMargin) / NumSounds;
}



void PatternEditor::update(CDrawContext *context)
{
	PatternData_v2 &Pattern = ExternalState->getPattern();

	if((context->getMouseButtons() & kLButton) && (SoundClickedOn >=0))
	{
		CPoint where;

		context->getMouseLocation(where);

		where.h -= size.left;

		int step = stepFromPoint(where);

		bool Changed = false;

		for(int i=MIN(step, StepClickedOn); i<=MAX(step, StepClickedOn); i++)
		{
			if(Pattern.Data[SoundClickedOn][i] != NewClickState)
			{
				Pattern.Data[SoundClickedOn][i] = NewClickState;

				Changed = true;
			}
		}

		StepClickedOn = step;

		if(Changed)
		{
			redraw();
		}
	}
	else
	{
		SoundClickedOn = -1;
	}

	//check if a different step is playing on any sound

	int StepWidth = size.width() / (Pattern.Beats * Pattern.Steps);
	updateSoundHeight();
	
	CRect where;

	for(int sound=0; sound<NumSounds; sound++)
	{
		int stepnow = InternalState->Sound[sound].StepPlaying;
		int stepthen = PrevStepPlaying[sound];
		int IDnow = InternalState->Sound[sound].NoteID;
		int IDthen = PrevNoteID[sound];

		if(IDnow != IDthen)
		{
			if((stepnow>=0) && (stepnow < Pattern.Steps*Pattern.Beats))
			{
				drawelement(context, where, stepnow, sound);
				OC->transfert(context, CRect(where.left+size.left, where.top+size.top, where.right+size.left-1, where.bottom+size.top-1),  CPoint(where.left, where.top));
			}

			if((stepthen>=0) && (stepthen < Pattern.Steps*Pattern.Beats))
			{
				drawelement(context, where, stepthen, sound);
				OC->transfert(context, CRect(where.left+size.left, where.top+size.top, where.right+size.left-1, where.bottom+size.top-1),  CPoint(where.left, where.top));
			}
		}
	}
}




void PatternEditor::SetSelSound(int _SelSound)
{
	SelSound = _SelSound;
}





void PatternEditor::Resize(CRect &newsize)
{
	size = newsize;

	OCSize = CRect(0, 0, size.width(), size.height());
}