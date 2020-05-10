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


//ContextMenu.cpp		-	AnarchyRhythms

//Leighton 3-4-2



#include "ContextMenu.h"






ContextMenu::ContextMenu(HWND _systemWindow, CControlListener *_listener, int _tag)
	:	PopupMenu(_systemWindow, _listener, _tag)
{
	MidiMenuIndex = -1;
	SoundMenuIndex = -1;

	HelpID = (HelpIDEnum)0;
	MIDIParam = -1;
}



void ContextMenu::SetContext(HelpIDEnum _HelpID, int _MIDIParam, bool Sound)
{
	HelpID = _HelpID;
	MIDIParam = _MIDIParam;

	removeAllEntry();

	int i=0;

	addEntry("What's this?", i);
	i++;

	if(MIDIParam >= 0)
	{
		addEntry("-", i);
		i++;

		MidiMenuIndex = i;

		addEntry("Map MIDI control", i);
		i++;
	}
	else
	{
		MidiMenuIndex = -1;
	}

	if(Sound)
	{
		addEntry("-", i);
		i++;

		SoundMenuIndex = i;

		addEntry("Sound...", i);
		i++;
	}
	else
	{
		SoundMenuIndex = -1;
	}
}
