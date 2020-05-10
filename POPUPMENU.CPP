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


//PopupMenu.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 2-2-2



#include "MyHelpers.h"
#include "PopupMenu.h"



PopupMenu::PopupMenu(HWND _systemWindow, CControlListener *listener, int tag)
	:	CControl(CRect(0, 0, 0, 0), listener, tag),
		systemWindow(_systemWindow)
{
	menu = CreatePopupMenu();
	current = 0;
}



PopupMenu::~PopupMenu()
{
	DestroyMenu(menu);
}



void PopupMenu::addEntry(char *label, int index)
{
	bool sep = (strcmp(label, "-")==0);

	InsertMenu(menu, index, MF_BYPOSITION | (sep ? MF_SEPARATOR : MF_STRING), index+1, sep ? NULL :label);
}



void PopupMenu::Popup(CDrawContext *context)
{
	CPoint mousewhere;

	context->getMouseLocation(mousewhere);

	RECT FrameWhere;

	GetWindowRect(systemWindow, &FrameWhere);

	int x = FrameWhere.left + mousewhere.h;
	int y = FrameWhere.top + mousewhere.v;

	current = TrackPopupMenu(menu, TPM_CENTERALIGN|TPM_TOPALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON, x, y, 0, systemWindow, NULL) - 1;

	if(current>=0)
	{
		listener->valueChanged(context, this);
	}
}





void PopupMenu::removeAllEntry()
{
	int num = GetMenuItemCount(menu);

	for(int i=1; i<=num; i++)
	{
		DeleteMenu(menu, i, MF_BYCOMMAND);
	} 
}