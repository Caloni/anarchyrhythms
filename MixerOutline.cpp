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


//MixerOutline.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 7-1-3



#include "MixerOutline.h"
#include "MyConsts.h"
#include "SleepingReporter.h"



MixerOutline::MixerOutline(CRect size, int _MyIndex, SleepingReporter *_SelSoundChanger, bool InitialHighlight, int _nSide)
	:	CControl(size, NULL, 0),
		SelSoundChanger(_SelSoundChanger),
		MyIndex(_MyIndex),
		Highlight(InitialHighlight),
		nSide(_nSide)
{
	SetInnerSize();
}



void MixerOutline::draw(CDrawContext *context)
{
	context->setFillColor(Highlight?SoundCol[MyIndex]:DarkSoundCol[MyIndex]);
	context->setFrameColor(Highlight?SoundCol[MyIndex]:DarkSoundCol[MyIndex]);

	context->fillRect(size);
	context->drawRect(size);

	context->setFrameColor(Highlight?LightSoundCol[MyIndex]:SoundCol[MyIndex]);

	context->moveTo(framestart);
	context->lineTo(frameend);
}




void MixerOutline::mouse(CDrawContext *context, CPoint &where)
{
	SelSoundChanger->SetInt(MyIndex);
}




void MixerOutline::Resize(CRect &newsize)
{
	size = newsize;

	SetInnerSize();
}




void MixerOutline::SetHighlight(bool _Highlight)
{
	if(Highlight != _Highlight)
	{
		Highlight = _Highlight;
		redraw();
	}
}




void MixerOutline::SetInnerSize()
{
	switch(nSide)
	{
		case 0:	

			framestart(size.left+2, size.bottom);
			frameend(size.right-2, size.bottom);
			break;

		case 1:	

			framestart(size.right, size.top);
			frameend(size.right, size.bottom);
			break;

		case 2:	

			framestart(size.left+2, size.top);
			frameend(size.right-2, size.top);
			break;

		case 3:	

			framestart(size.left, size.top);
			frameend(size.left, size.bottom);
			break;
	}
}
