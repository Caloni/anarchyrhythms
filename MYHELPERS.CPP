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


//Filename:		MYHELPERS.CPP
//Author:		Leighton Hargreaves
//Date:			30/01/2001
//Description:	Contains an anthology of handy little functions


#define	_DEVELOPMENT


#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "MyHelpers.h"


#include <direct.h>
#include <stdio.h>
#include <process.h>
#include "vstcontrols.h"

#include <htmlhelp.h>

void itoa(int i, char *a)
{
	if(i<0)
	{
		strcpy(a, "-");

		itoa(-i, &a[1]);
	}
	else
	{
		int count=1;
		int pow = 1;

		while (i >= pow*10)
		{
			count++;
			pow*=10;
		}

		for(int loop = 0; loop < count; loop++)
		{
			a[loop] = ((i/pow)%10) + '0';

			pow/=10;
		}

		a[loop] = 0;
	}
}

void DrawThickLine(CDrawContext *context, CPoint &p1, CPoint &p2, int width, const CColor &Colour)
{
	context->setFillColor(Colour);
	context->setFrameColor(Colour);

	static CPoint poly[4];

	int len = (int) sqrt((p1.h-p2.h)*(p1.h-p2.h) + (p1.v-p2.v)*(p1.v-p2.v));

	int sideh = (p2.v - p1.v) * width;
	int sidev = (p1.h - p2.h) * width;

	if(sideh > 0)	sideh += len-1;		else  sideh -= len-1;
	if(sidev > 0)	sidev += len-1;		else  sidev -= len-1;

	if(len>1)
	{
		sideh /= len;
		sidev /= len;
	}

	poly[0].h = p1.h - sideh;
	poly[0].v = p1.v - sidev;
	poly[1].h = p1.h + sideh;
	poly[1].v = p1.v + sidev;
	poly[2].h = p2.h + sideh;
	poly[2].v = p2.v + sidev;
	poly[3].h = p2.h - sideh;
	poly[3].v = p2.v - sidev;
	
	context->fillPolygon(poly, 4);
	context->polyLine(poly, 4);
}


void DrawArrow(CDrawContext *context, CPoint &Where, int Xvector, int Yvector, const CColor &Colour, int HeadSize)
{
	context->setLineStyle(kLineSolid);
	context->setFrameColor(Colour); 

	context->moveTo(Where);

	CPoint ArrowTip(Where.h + Xvector, Where.v + Yvector);

	context->lineTo(ArrowTip);

	CPoint ArrowHead(	ArrowTip.h - Xvector*HeadSize/MAX(1, ABS(Xvector)) + Yvector*HeadSize/MAX(1, ABS(Yvector)),
						ArrowTip.v - Yvector*HeadSize/MAX(1, ABS(Yvector)) - Xvector*HeadSize/MAX(1, ABS(Xvector)));

	context->lineTo(ArrowHead);

	context->moveTo(ArrowTip);

	ArrowHead(	ArrowTip.h - Xvector*HeadSize/MAX(1, ABS(Xvector)) - Yvector*HeadSize/MAX(1, ABS(Yvector)),
				ArrowTip.v - Yvector*HeadSize/MAX(1, ABS(Yvector)) + Xvector*HeadSize/MAX(1, ABS(Xvector)));

	context->lineTo(ArrowHead);
}

void LoadOnlineSupport(HWND hWnd)
{
	HWND hwndASS = HtmlHelp(hWnd, "http:\\\\www.anarchysoundsoftware.co.uk", HH_DISPLAY_TOPIC, 0);

	if(hwndASS)
	{
		ShowWindow(hwndASS, SW_MAXIMIZE);
		SetWindowText(hwndASS, "AnarchySoundSoftware");
	}

}












//End of file