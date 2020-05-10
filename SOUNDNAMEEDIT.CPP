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


//SoundNameEdit.cpp	-	ANARCHY RHYTHMS

//LEIGHTON HARGREAVES 5-2-2



#include "SleepingReporter.h"
#include "SoundNameEdit.h"
#include "MyConsts.h"
#include "ContextMenu.h"




SoundNameEdit::SoundNameEdit(CRect &size, CControlListener *listener, int tag, SleepingReporter *_SelSoundChanger, int _MyIndex, const bool _TabstopStylee, bool InitialHighlight, ContextMenu *_contextmenu)
	:	CTextEdit(size, listener, tag),
		contextmenu(_contextmenu),
		SelSoundChanger(_SelSoundChanger),
		MyIndex(_MyIndex),
		Highlight(InitialHighlight),
		TabstopStylee(_TabstopStylee)
{
	setFont(kNormalFontVerySmall);

	SetColors();
}



void SoundNameEdit::draw(CDrawContext *context)
{
	CTextEdit::draw(context);
}



void SoundNameEdit::update(CDrawContext *context)
{
	CTextEdit::update(context);
}



void SoundNameEdit::mouse(CDrawContext* context, CPoint& where)
{
	if(context->getMouseButtons() & kLButton)
	{
		if(Highlight)
		{
			//setText("");
			CTextEdit::mouse(context, where);
		}
		else
		{
			SelSoundChanger->SetInt(MyIndex);
		}
	}
	else
	{
		SelSoundChanger->SetInt(MyIndex);

		contextmenu->SetContext(HIDEditPaneName, -1, true);
		contextmenu->Popup(context);
	}
}




void SoundNameEdit::Resize(CRect &newsize)
{
	size = newsize;
}



void SoundNameEdit::SetHighlight(bool _Highlight)
{
	if(Highlight != _Highlight)
	{
		Highlight = _Highlight;

		SetColors();

		redraw();
	}
}




void SoundNameEdit::SetColors()
{
	CColor DimmedCol = {64, 64, 64, 0};
	CColor LitCol = {148, 148, 148, 0};
	
	if(Highlight)
	{
		if(TabstopStylee)
		{
			setFrameColor(DimmedCol);
			setFontColor(LightSoundCol[MyIndex]);
			setBackColor(DimmedCol);
		}
		else
		{
			setFrameColor(DimmedCol);
			setFontColor(kWhiteCColor);
			setBackColor(ControlBk);
		}
	}
	else
	{
		if(TabstopStylee)
		{
			setFrameColor(DimmedCol);
			setFontColor(LightSoundCol[MyIndex]);
			setBackColor(ControlBk);
		}
		else
		{
			setFontColor(kWhiteCColor);
			setFrameColor(DimmedCol);
			setBackColor(ControlBk);
		}
	}
}





