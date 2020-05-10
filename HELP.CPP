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


//Help.cpp	AnarchyRhythms
//L8n



#include "Help.h"
#include "MyHelpers.h"

#include <htmlhelp.h>

#include <process.h>



const char *HelpHtmlFilename[numHelpIDs] = {

	"WhatIsIt.htm",
	"PatternEditor.htm",
	"Levels.htm",

	"PatternShuffle.htm",
	"PatternChanging.htm",
	"PatternSequencing.htm",

	"MixerComb.htm",
	"MixerCompression.htm",
	"MixerPan.htm",
	"MixerPitch.htm",
	"MixerLength.htm",
	"MixerFilter.htm",
	"MixerFeedback.htm",
	"MixerOscillator.htm",
	"MixerVolume.htm",
	"MixerSoloMute.htm",
	"MixerName.htm",

	"EditPaneVolume.htm",
	"EditPanePan.htm",
	"EditPanePitch.htm",
	"EditPaneFilter.htm",
	"EditPaneOscillator.htm",
	"EditPaneLength.htm",
	"EditPaneFeedback.htm",
	"EditPaneComb.htm",
	"EditPaneCompression.htm",
	"EditPaneTabstop.htm"
};



void SpawnHelp(HelpIDEnum HelpID)
{
	char path[ MAX_PATH ];

	HKEY keySoftware = NULL;
	HKEY keyASS = NULL;
	HKEY keyProduct = NULL;

	LONG res = RegCreateKey(HKEY_LOCAL_MACHINE, "Software", &keySoftware);
	if( res != ERROR_SUCCESS) return;

	res = RegCreateKey(keySoftware, "AnarchySoundSoftware", &keyASS);
	if( res != ERROR_SUCCESS) return;

	#ifdef EVALUATIONVERSION
		char *productKey = "AnarchyRhythmsDEMO";
	#else
		char *productKey = "AnarchyRhythms";
	#endif

	res = RegCreateKey(keyASS, productKey, &keyProduct);
	if( res != ERROR_SUCCESS) return;

	DWORD dwType = 0;
	DWORD dwSize = MAX_PATH;

	res = RegQueryValueEx( keyProduct, "applicationDirectory", NULL, &dwType, (LPBYTE)path, &dwSize );
	if(  res != ERROR_SUCCESS ) return;
	if( dwType != REG_SZ ) return;

	res = RegCloseKey(keyProduct);
	res = RegCloseKey(keyASS);	
	res = RegCloseKey(keySoftware);

	strcat( path, "\\AnarchyRhythms HELP.chm" );

	if( HelpID < 0 || HelpID >= numHelpIDs ) HelpID = HIDContents;

	HtmlHelp(NULL, path, HH_DISPLAY_TOPIC, (DWORD) HelpHtmlFilename[ HelpID ] );
}

