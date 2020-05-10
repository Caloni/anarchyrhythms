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


//ControlPositioners.h	-	AnarchyRhythms

//mini-fns for positioning controls

//leighton hargreaves 2-2-2

#ifndef CONTROLPOSITIONERGATE
#define CONTROLPOSITIONERGATE


#include "vstgui.h"
#include "MyConsts.h"

const int nBannerHeight = 18;

//editor controls
void GetPatternEditorPos(CRect &size, CBitmap *hCurrentBackground);
void GetMixerChannelPos(CRect &size, CBitmap *hCurrentBackground, int chn);
void GetMixSliderPos(CRect &size, CBitmap *hCurrentBackground);
void GetBeatsAdjusterPos(CRect &size, CBitmap *hCurrentBackground);
void GetStepsAdjusterPos(CRect &size, CBitmap *hCurrentBackground);
void GetBPMAdjusterPos(CRect &size, CBitmap *hCurrentBackground);
void GetHostSyncSelectorPos(CRect &size, CBitmap *hCurrentBackground);
void GetInLevelSliderPos(CRect &size, CBitmap *hCurrentBackground);
void GetOutLevelSliderPos(CRect &size, CBitmap *hCurrentBackground);
void GetBannerPos(CRect &size, CBitmap *hCurrentBackground);
void GetShuffleMenuPos(CRect &size, CBitmap *hCurrentBackground);
void GetShuffleSliderPos(CRect &size, CBitmap *hCurrentBackground);
void GetPatternMenuPos(CRect &size, CBitmap *hCurrentBackground);
void GetPatternUpPos(CRect &size, CBitmap *hCurrentBackground);
void GetPatternDownPos(CRect &size, CBitmap *hCurrentBackground);


//menus
void GetPresetMenuPos(CRect &size, CBitmap *hCurrentBackground);
void GetSoundMenuPos(CRect &size, CBitmap *hCurrentBackground);
void GetMidiMenuPos(CRect &size, CBitmap *hCurrentBackground);
void GetWindowSizeMenuPos(CRect &size, CBitmap *hCurrentBackground);
void GetHelpMenuPos(CRect &size, CBitmap *hCurrentBackground);

void GetSoundEditPanePos(CRect &size, CBitmap *hCurrentBackground);

//mixer channel controls
void GetMixerLevelPos(CRect &size, CRect &mixerchannelsize);
void GetMixerCompressionPos(CRect &size, CRect &mixerchannelsize);
void GetMixerCombPos(CRect &size, CRect &mixerchannelsize);
void GetMixerPanPos(CRect &size, CRect &mixerchannelsize);
void GetMixerPitchPos(CRect &size, CRect &mixerchannelsize);
void GetMixerFilterWidthPos(CRect &size, CRect &mixerchannelsize);
void GetMixerLengthPos(CRect &size, CRect &mixerchannelsize);
void GetMixerOscillatorPos(CRect &size, CRect &mixerchannelsize);
void GetMixerFeedBackPos(CRect &size, CRect &mixerchannelsize);
void GetSoloButtonPos(CRect &size, CRect &mixerchannelsize);
void GetMuteButtonPos(CRect &size, CRect &mixerchannelsize);
void GetMixerNameEditPos(CRect &size, CRect &mixerchannelsize);

void GetMixerTopOutlinePos(CRect &size, CRect &mixerchannelsize);
void GetMixerLeftOutlinePos(CRect &size, CRect &mixerchannelsize);
void GetMixerBottomOutlinePos(CRect &size, CRect &mixerchannelsize);
void GetMixerRightOutlinePos(CRect &size, CRect &mixerchannelsize);

//sound edit pane controls
void GetSoundPaneNameEditPos(CRect &size, CRect &soundeditpanesize, int i);
void GetSoundPaneEnvelopePos(CRect &size, CRect &soundeditpanesize, int i);

void GetSEPLevelAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPPanAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPCoarsePitchAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPFinePitchAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPFilterWidthAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPOscillatorLevelAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPLengthAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPFeedBackAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPCombAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPCompressionAdjusterPos(CRect &size, CRect &soundeditpanesize);

void GetSEPPanEnvelopeRangeAdjusterPos(CRect &size, CRect &soundeditpanesize);
void GetSEPPitchEnvelopeRangeAdjusterPos(CRect &size, CRect &soundeditpanesize);

void GetSEPOscMenuPos(CRect &size, CRect &soundeditpanesize, int i);

void GetSepOsc2PhaseInvertPos(CRect &size, CRect &soundeditpanesize);

#endif //CONTROLPOSITIONERGATE