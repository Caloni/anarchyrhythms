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


//ControlPositioners.cpp	-	AnarchyRhythms

//mini-fns for positioning controls

//leighton hargreaves 2-2-2


#include "MyConsts.h"
#include "ControlPositioners.h"
#include "ExternalStateData_v2.h"

#include <assert.h>



const int NameLabelHeight = 15;

const int MonoSoloHeight = 15;

const int HorizEnvelopeMargin = 8;
const int VertEnvelopeMargin = 8;

const int IntAdjusterHeight = 12;

const int MixerOutlineWidth = 2;



//main editor controls


void GetPatternEditorPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(54, 25, hCurrentBackground->getWidth() - 54, hCurrentBackground->getHeight()/3-19);
}




void GetMixerChannelPos(CRect &size, CBitmap *hCurrentBackground, int chn)
{
	size((NumSounds-chn-1)*hCurrentBackground->getWidth()/10 + 8, hCurrentBackground->getHeight()/3+16, (NumSounds-chn)*hCurrentBackground->getWidth()/10, hCurrentBackground->getHeight() - 32);
}


void GetSoundEditPanePos(CRect &size, CBitmap *hCurrentBackground)
{
	size(NumSounds*hCurrentBackground->getWidth()/10 + 16, hCurrentBackground->getHeight()*36/100, hCurrentBackground->getWidth()-8, hCurrentBackground->getHeight() - 32);
}



void GetMixSliderPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(50, hCurrentBackground->getHeight() - 24, hCurrentBackground->getWidth() - 50, hCurrentBackground->getHeight() - 4);
}




void GetStepsAdjusterPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth()-124, 5, hCurrentBackground->getWidth()-55, 21);
}


void GetBeatsAdjusterPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth()-178, 5, hCurrentBackground->getWidth()-125, 21);
}



void GetBPMAdjusterPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth()-232, 5, hCurrentBackground->getWidth()-179, 21);
}



void GetHostSyncSelectorPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth()-233, 5, hCurrentBackground->getWidth()-303, 21);
}


void GetInLevelSliderPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(11, 25, 42, hCurrentBackground->getHeight()/3-16);
}



void GetOutLevelSliderPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth() - 42, 8, hCurrentBackground->getWidth() - 11, hCurrentBackground->getHeight()/3-16);
}



void GetBannerPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(0, hCurrentBackground->getHeight(), hCurrentBackground->getWidth(), hCurrentBackground->getHeight()+nBannerHeight-4);
}


void GetShuffleMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(58, hCurrentBackground->getHeight()/3-16, 100, hCurrentBackground->getHeight()/3-6);
}

void GetShuffleSliderPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(100, hCurrentBackground->getHeight()/3-18, 250, hCurrentBackground->getHeight()/3-4);
}

void GetPatternMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth() - 230, hCurrentBackground->getHeight()/3-17, hCurrentBackground->getWidth() - 78, hCurrentBackground->getHeight()/3-4);
}

void GetPatternUpPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth() - 252, hCurrentBackground->getHeight()/3-17, hCurrentBackground->getWidth() - 231, hCurrentBackground->getHeight()/3-4);
}

void GetPatternDownPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(hCurrentBackground->getWidth() - 77, hCurrentBackground->getHeight()/3-17, hCurrentBackground->getWidth() - 56, hCurrentBackground->getHeight()/3-4);
}

//main menu positions


void GetPresetMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(0, 0, 49, 12);
}	



void GetSoundMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(50, 0, 99, 12);
}


void GetMidiMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(100, 0, 159, 12);
}


void GetWindowSizeMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(160, 0, 219, 12);
}


void GetHelpMenuPos(CRect &size, CBitmap *hCurrentBackground)
{
	size(220, 0, 260, 12);
}



//Mixer channel positions



void GetMixerLevelPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left-1, mixerchannelsize.top + mixerchannelsize.height()*2/3-1, mixerchannelsize.right, mixerchannelsize.bottom - NameLabelHeight - MonoSoloHeight);
}

void GetMixerCombPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left, mixerchannelsize.top + mixerchannelsize.height()/6, mixerchannelsize.left + mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()/3); 
}

void GetMixerCompressionPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left+mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()/6, mixerchannelsize.right, mixerchannelsize.top + mixerchannelsize.height()/3); 
}

void GetMixerPanPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left, mixerchannelsize.top + mixerchannelsize.height()/2, mixerchannelsize.left + mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()*2/3); 
}

void GetMixerPitchPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left+mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()/2, mixerchannelsize.right, mixerchannelsize.top + mixerchannelsize.height()*2/3); 
}

void GetMixerFilterWidthPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left, mixerchannelsize.top + mixerchannelsize.height()/3, mixerchannelsize.left + mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()/2); 
}

void GetMixerLengthPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left, mixerchannelsize.top, mixerchannelsize.left + mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()/6); 
}

void GetMixerOscillatorPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left+mixerchannelsize.width()/2, mixerchannelsize.top + mixerchannelsize.height()/3, mixerchannelsize.right, mixerchannelsize.top + mixerchannelsize.height()/2); 
}

void GetMixerFeedBackPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left+mixerchannelsize.width()/2, mixerchannelsize.top, mixerchannelsize.right, mixerchannelsize.top + mixerchannelsize.height()/6); 
}

void GetMixerNameEditPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left, mixerchannelsize.bottom - NameLabelHeight, mixerchannelsize.right-1, mixerchannelsize.bottom-1); 
}

void GetSoloButtonPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left, mixerchannelsize.bottom - NameLabelHeight - MonoSoloHeight, mixerchannelsize.left+mixerchannelsize.width()/2-1, mixerchannelsize.bottom-NameLabelHeight-1); 
}

void GetMuteButtonPos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left+mixerchannelsize.width()/2, mixerchannelsize.bottom - NameLabelHeight - MonoSoloHeight, mixerchannelsize.right-1, mixerchannelsize.bottom-NameLabelHeight-1); 
}

void GetMixerTopOutlinePos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left-MixerOutlineWidth-1, mixerchannelsize.top-MixerOutlineWidth-1, mixerchannelsize.right+MixerOutlineWidth, mixerchannelsize.top-1); 
}

void GetMixerLeftOutlinePos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left-MixerOutlineWidth-1, mixerchannelsize.top, mixerchannelsize.left-1, mixerchannelsize.bottom); 
}

void GetMixerBottomOutlinePos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.left-MixerOutlineWidth-1, mixerchannelsize.bottom, mixerchannelsize.right+MixerOutlineWidth, mixerchannelsize.bottom+MixerOutlineWidth); 
}

void GetMixerRightOutlinePos(CRect &size, CRect &mixerchannelsize)
{
	size(mixerchannelsize.right, mixerchannelsize.top, mixerchannelsize.right+MixerOutlineWidth, mixerchannelsize.bottom); 
}



//sound edit pane control positions
void GetSoundPaneNameEditPos(CRect &size, CRect &soundeditpanesize, int i)
{
	size(soundeditpanesize.left + (NumSounds-i-1)*soundeditpanesize.width()*15/100+1, soundeditpanesize.top, soundeditpanesize.left + (NumSounds-i)*soundeditpanesize.width()*15/100, soundeditpanesize.top + NameLabelHeight);
}





void GetSoundPaneEnvelopePos(CRect &size, CRect &soundeditpanesize, int i)
{
	switch(i)
	{
		case envLevel:			size(soundeditpanesize.left + HorizEnvelopeMargin, soundeditpanesize.top + VertEnvelopeMargin + NameLabelHeight + IntAdjusterHeight, soundeditpanesize.left + soundeditpanesize.width()/3-HorizEnvelopeMargin/2, soundeditpanesize.top+soundeditpanesize.height()/2);	break;
		case envPan:			size(soundeditpanesize.left + HorizEnvelopeMargin, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + IntAdjusterHeight, soundeditpanesize.left + soundeditpanesize.width()/3-HorizEnvelopeMargin/2, soundeditpanesize.bottom-VertEnvelopeMargin - IntAdjusterHeight);	break;
		case envPitch:			size(soundeditpanesize.left + soundeditpanesize.width()/3 + HorizEnvelopeMargin/2, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + IntAdjusterHeight*2, soundeditpanesize.left + soundeditpanesize.width()*2/3-HorizEnvelopeMargin/2, soundeditpanesize.bottom-VertEnvelopeMargin-IntAdjusterHeight);	break;
		case envFilterWidth:	size(soundeditpanesize.left + soundeditpanesize.width()/3 + HorizEnvelopeMargin/2, soundeditpanesize.top + VertEnvelopeMargin + NameLabelHeight + IntAdjusterHeight*2, soundeditpanesize.left + soundeditpanesize.width()*2/3-HorizEnvelopeMargin/2, soundeditpanesize.top+soundeditpanesize.height()/2);	break;
		case envOscillator:		size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2, soundeditpanesize.top + VertEnvelopeMargin*5 + IntAdjusterHeight*6, soundeditpanesize.right - HorizEnvelopeMargin, soundeditpanesize.top+soundeditpanesize.height()*3/4);	break;
	}
}


void GetSEPLevelAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + HorizEnvelopeMargin+1, soundeditpanesize.top + VertEnvelopeMargin + NameLabelHeight+1, soundeditpanesize.left + soundeditpanesize.width()/3-HorizEnvelopeMargin/2-1, soundeditpanesize.top + VertEnvelopeMargin + NameLabelHeight + IntAdjusterHeight);
}

void GetSEPPanAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + HorizEnvelopeMargin+1, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin+1, soundeditpanesize.left + soundeditpanesize.width()/3-HorizEnvelopeMargin/2-1, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + IntAdjusterHeight);
}

void GetSEPFilterWidthAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin + NameLabelHeight+1, soundeditpanesize.left + soundeditpanesize.width()*2/3-HorizEnvelopeMargin/2-1, soundeditpanesize.top + VertEnvelopeMargin + NameLabelHeight + IntAdjusterHeight*2);
}

void GetSEPOscillatorLevelAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin*5 + IntAdjusterHeight*4, soundeditpanesize.right - HorizEnvelopeMargin-1, soundeditpanesize.top + VertEnvelopeMargin*5 + IntAdjusterHeight*5);
}

void GetSEPLengthAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin, soundeditpanesize.right - HorizEnvelopeMargin-1, soundeditpanesize.top + VertEnvelopeMargin + IntAdjusterHeight);
}

void GetSEPFeedBackAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin*2+IntAdjusterHeight, soundeditpanesize.right - HorizEnvelopeMargin-1, soundeditpanesize.top + VertEnvelopeMargin*2 + IntAdjusterHeight*2);
}

void GetSEPCombAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin*3+IntAdjusterHeight*2, soundeditpanesize.right - HorizEnvelopeMargin-1, soundeditpanesize.top + VertEnvelopeMargin*3 + IntAdjusterHeight*3);
}


void GetSEPCompressionAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin*4+IntAdjusterHeight*3, soundeditpanesize.right - HorizEnvelopeMargin-1, soundeditpanesize.top + VertEnvelopeMargin*4 + IntAdjusterHeight*4);
}

void GetSEPCoarsePitchAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + 1, soundeditpanesize.left + soundeditpanesize.width()*2/3-HorizEnvelopeMargin/2-1, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + IntAdjusterHeight);	
}

void GetSEPFinePitchAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + IntAdjusterHeight+1, soundeditpanesize.left + soundeditpanesize.width()*2/3-HorizEnvelopeMargin/2-1, soundeditpanesize.top + soundeditpanesize.height()/2 + VertEnvelopeMargin + IntAdjusterHeight*2);	
}


//SEP envelope range adjusters

void GetSEPPanEnvelopeRangeAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + HorizEnvelopeMargin+1, soundeditpanesize.bottom-VertEnvelopeMargin - IntAdjusterHeight, soundeditpanesize.left + soundeditpanesize.width()/3-HorizEnvelopeMargin/2-1, soundeditpanesize.bottom-VertEnvelopeMargin);
}


void GetSEPPitchEnvelopeRangeAdjusterPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.bottom-VertEnvelopeMargin-IntAdjusterHeight, soundeditpanesize.left + soundeditpanesize.width()*2/3-HorizEnvelopeMargin/2-1, soundeditpanesize.bottom-VertEnvelopeMargin-1);
}


//oscillator menus


void GetSEPOscMenuPos(CRect &size, CRect &soundeditpanesize, int i)
{
	switch(i)
	{
		case 0:

			size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top + VertEnvelopeMargin*5 + IntAdjusterHeight*5, soundeditpanesize.right - HorizEnvelopeMargin-1, soundeditpanesize.top + VertEnvelopeMargin*5 + IntAdjusterHeight*6);
			break;

		case 1:

			size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+1, soundeditpanesize.top+soundeditpanesize.height()*3/4, soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+49, soundeditpanesize.top+soundeditpanesize.height()*3/4+IntAdjusterHeight);	
			break;
	}
}



void GetSepOsc2PhaseInvertPos(CRect &size, CRect &soundeditpanesize)
{
	size(soundeditpanesize.left + soundeditpanesize.width()*2/3 + HorizEnvelopeMargin/2+49, soundeditpanesize.top+soundeditpanesize.height()*3/4, soundeditpanesize.right - HorizEnvelopeMargin - 1, soundeditpanesize.top+soundeditpanesize.height()*3/4+IntAdjusterHeight);	
}
