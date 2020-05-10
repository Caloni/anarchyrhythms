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


//LightUpBox.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2



#include "LightUpBox.h"
#include "MyConsts.h"
#include "SleepingReporter.h"
#include "ContextMenu.h"



LightUpBox::LightUpBox(CRect size, CControlListener *Listener, int tag, char *_Caption, int _MyIndex, SleepingReporter *_SelSoundChanger, bool InitialHighlight, ContextMenu *_contextmenu, bool _IsSolo)
	:	CControl(size, Listener, tag),
		contextmenu(_contextmenu),
		SelSoundChanger(_SelSoundChanger),
		MyIndex(_MyIndex),
		Highlight(InitialHighlight),
		IsSolo(_IsSolo)
{
	strcpy(Caption, _Caption);

	val = false;
}



void LightUpBox::draw(CDrawContext *context)
{
	CColor DimmedCol = {64, 64, 64, 0};
	CColor LitCol = {148, 148, 148, 0};

	context->setFillColor(val?LitCol:ControlBk);
	context->fillRect(size);

	context->setFrameColor(DimmedCol);
	context->drawRect(size);

	context->setFont(kNormalFontVerySmall);
	context->setFontColor(kWhiteCColor);//val?DarkSoundCol[MyIndex]:Highlight?LightSoundCol[MyIndex]:SoundCol[MyIndex]);

	context->drawString(Caption, size);
}




void LightUpBox::mouse(CDrawContext *context, CPoint &where)
{
	if(context->getMouseButtons()&kLButton)
	{
		int buttons = context->getMouseButtons();

		if(buttons & (kLButton|kRButton))
		{
			val = !val;

			listener->valueChanged(context, this);

			SelSoundChanger->SetInt(MyIndex);
		}
	}
	else
	{
		contextmenu->SetContext(IsSolo ? HIDMixerSoloMute: HIDMixerSoloMute, -1, true);
		contextmenu->Popup(context);
	}
}




void LightUpBox::MySetValue(bool _val)
{
	val = _val;
}



bool LightUpBox::MyGetValue()
{
	return val;
	
}



void LightUpBox::Resize(CRect &newsize)
{
	size = newsize;
}




void LightUpBox::SetHighlight(bool _Highlight)
{
	if(Highlight != _Highlight)
	{
		Highlight = _Highlight;
		redraw();
	}
}
