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


//Banner.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 8-4-02


#include "Banner.h"

#include <time.h>


#ifdef EVALUATIONVERSION

const int OnlineButtonWidth = 110;
const char *OnlineButtonText = "REGISTER ONLINE ! ! !";
const char *BannerText = "EVALUATION version produces a click every ten seconds.  To remove the clicks  ";

#else

const int OnlineButtonWidth = 90;
const char *OnlineButtonText = "ONLINE Check";
const char *BannerText = "Like to check for updates?   ";

#endif




Banner::Banner(CRect& size, CControlListener* listener, int tag)
	:	CControl(size, listener, tag)
{
	OnlineHighlight = false;

#ifndef EVALUATIONVERSION

	CloseHighlight = false;

#endif

	ColStep = GetColStep();

	Resize(size);
}



long Banner::GetColStep()
{
	return (time(NULL)/ColorChangeInterval);
}




void Banner::Resize(CRect &newsize)
{
	size = newsize;

#ifdef EVALUATIONVERSION

	OnlinePos.left = (size.left+size.right)/2 + 150;
	OnlinePos.right = OnlinePos.left + OnlineButtonWidth;

	TextPos.left = size.left+1;

#else
	
	ClosePos.left = size.left+1;
	ClosePos.top= size.top;
	ClosePos.right = size.left+size.height()+1;
	ClosePos.bottom = size.bottom;

	OnlinePos.left = (size.left+size.right)/2;
	OnlinePos.right = OnlinePos.left + OnlineButtonWidth;

	TextPos.left = ClosePos.right;

#endif

	OnlinePos.top = size.top;
	OnlinePos.bottom = size.bottom-1;

	TextPos.top = size.top;
	TextPos.right = OnlinePos.left;
	TextPos.bottom = size.bottom;
}




void Banner::draw(CDrawContext *context)
{
	context->setFillColor(kBlackCColor);
	context->fillRect(size);
	context->setFrameColor(kBlackCColor);
	context->drawRect(size);

	CColor col;

	switch(ColStep%6)
	{
		case 0:		col = kRedCColor;		break;
		case 1:		col = kGreenCColor;		break;
		case 2:		col = kYellowCColor;	break;
		case 3:		col = kCyanCColor;		break;
		case 4:		col = kMagentaCColor;	break;
		case 5:		col = kWhiteCColor;		break;
	}

	context->setFont(kNormalFontSmall);

	context->setFontColor(col);

	context->drawString(BannerText, TextPos, false, kRightText);
	context->drawString(OnlineButtonText, OnlinePos, false, kCenterText);

	context->setFrameColor(col);

	if(OnlineHighlight)
	{
		context->drawRect(OnlinePos);
	}

#ifndef EVALUATIONVERSION

	context->moveTo(CPoint(ClosePos.left+3, ClosePos.top+3));
	context->lineTo(CPoint(ClosePos.right-2, ClosePos.bottom-2));
	context->moveTo(CPoint(ClosePos.left+3, ClosePos.bottom-3));
	context->lineTo(CPoint(ClosePos.right-2, ClosePos.top+2));

	if(CloseHighlight)
	{
		context->drawRect(ClosePos);
	}

#endif

}



void Banner::update(CDrawContext *context)
{
	CPoint where;

	context->getMouseLocation(where);

	bool NewHighlight = where.isInside(OnlinePos);

	if(OnlineHighlight != NewHighlight)
	{
		OnlineHighlight = NewHighlight;

		redraw();
	}

#ifndef EVALUATIONVERSION

	NewHighlight = where.isInside(ClosePos);

	if(CloseHighlight != NewHighlight)
	{
		CloseHighlight = NewHighlight;

		redraw();
	}

#endif
	
	long NewColStep = GetColStep();

	if(NewColStep != ColStep)
	{
		ColStep = NewColStep;
		redraw();
	}
}




void Banner::mouse(CDrawContext *context, CPoint &where)
{
	if(where.isInside(OnlinePos))
	{
		LoadOnlineSupport(NULL);

		listener->valueChanged(context, this);
	}
}