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


/*-----------------------------------------------------------------------------

  -  ANARCHY RHYTHMS
  LEIGHTON HARGREAVES

-----------------------------------------------------------------------------*/
#include <stdio.h>

#ifndef __ADEditor
#include "Editor.hpp"
#endif

#ifndef __ADELAY_H
#include "BaseDLL.hpp"
#endif

#include "PatternEditor.h"
#include "MixerChannelEditor.h"
#include "MixSlider.h"
#include "IntAdjuster.h"
#include "SleepingReporter.h"
#include "MyOptionMenu.h"
#include "ControlPositioners.h"
#include "SoundEditPane.h"
#include "CheckBox.h"
#include "Presets.h"
#include "PopupMenu.h"
#include "IOSlider.h"
#include "ContextMenu.h"
#include "Banner.h"
#include "Help.h"
#include "ShuffleSlider.h"
#include "PopupNameEdit.h"
#include "SpinButton.h"
#include "resource\resource.h"

#include <assert.h>
#include <time.h>


//-----------------------------------------------------------------------------
// resource id's
enum {
	// bitmaps
	kBackgroundSmallId = 128,
	kBackgroundLargeId = 129,
};

#if MOTIF
// resource for MOTIF (format XPM)
#include "bmp00128.xpm"
#include "bmp00129.xpm"

CResTable xpmResources = {
  {kBackgroundSmallId,	bmp00128},
  {kBackgroundLargeId,	bmp00129},
  {0, 0}
};
#endif


char *SoundMenuOptionStr[numSoundMenuOptions] = {
	"Preset Sounds...",
	"-",
	"Load user sound",
	"Save As user sound",
};


char *MidiMenuOptionStr[numMidiMenuOptions] = {
	"Edit Midi Automation Map",
	"-",
	"Load Midi Automation Map",
	"Save Midi Automation Map"
};


char *HelpMenuOptionStr[numHelpMenuOptions] = {

	"Help",
	"-",
	"About...",
	"-",
	"Anarchy Sound Software ONLINE",
	"-",
	"anarchy button"
};


char *PatternMenuOptionStr[numPatternMenuOptions] = {

	"-",
	"Rename",
	"Clear",
	"Copy",
	"Paste"
};







//-----------------------------------------------------------------------------
// ADEditor class implementation
//-----------------------------------------------------------------------------
Editor::Editor(AudioEffect *effect)
 : AEffGUIEditor (effect) 
{
	ExternalState = ((BaseDLL *) effect)->GetExternalState();
	InternalState = ((BaseDLL *) effect)->GetInternalState();

	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackgroundSmall = new CBitmap(kBackgroundSmallId);
	hBackgroundLarge  = new CBitmap(kBackgroundLargeId);

	isOpen = false;
	
	bEditingPatternName = false;

	ErrorMsg = NULL;

	frame = NULL;

	banner = NULL;

	hCurrentBackground = hBackgroundSmall;

#ifdef EVALUATIONVERSION
	int heightAdditive = nBannerHeight;
#else
	int heightAdditive = 0;
#endif

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)hCurrentBackground->getWidth();
	rect.bottom = (short)hCurrentBackground->getHeight() + heightAdditive;
}

//-----------------------------------------------------------------------------
Editor::~Editor()
{
	// free background bitmap
	if(hBackgroundSmall)
		hBackgroundSmall->forget();

	if(hBackgroundLarge)
		hBackgroundLarge->forget();

	hBackgroundSmall = NULL;
	hBackgroundLarge = NULL;
}

//-----------------------------------------------------------------------------
long Editor::open(void *ptr)
{
	// !!! always call this !!!
	AEffGUIEditor::open(ptr);

#ifdef EVALUATIONVERSION
	int heightAdditive = nBannerHeight;
#else
	int heightAdditive = 0;
#endif

	//--init background frame-----------------------------------------------
	CRect size(0, 0, hCurrentBackground->getWidth(), hCurrentBackground->getHeight() + heightAdditive);
	frame = new CFrame(size, ptr, this);
	size.bottom -= heightAdditive;

	frame->setSize(hCurrentBackground->getWidth(), hCurrentBackground->getHeight() + heightAdditive);

	frame->setBackground(hCurrentBackground);

	//init sleeping reporters
	int i;
	for(i=0; i<kNumSleepingReporters; i++)
	{
		sleepingreporter[i] = new SleepingReporter(this, kFirstSleepingReporter+i);
	}

	contextmenu = new ContextMenu((HWND)ptr, this, kContextMenu);

	GetPatternEditorPos(size, hCurrentBackground);
	patterneditor = new PatternEditor(size, ExternalState, sleepingreporter[kSelSoundChange], InternalState, *frame, contextmenu);
	patterneditor->SetSelSound(ExternalState->SelectedSound);
	frame->addView(patterneditor);

	for(i=0; i<NumSounds; i++)
	{
		GetMixerChannelPos(size, hCurrentBackground, i);
		mixerchannel[i] = new MixerChannelEditor(size, &(ExternalState->Sound[i]), *frame, i, sleepingreporter, i == ExternalState->SelectedSound, contextmenu);
	}

#ifdef EVALUATIONVERSION
	bool CreateBanner = true;
#else
	bool CreateBanner = false;
#endif

	if(CreateBanner)
	{
		GetBannerPos(size, hCurrentBackground);
		banner = new Banner(size, this, kBanner);
		frame->addView(banner);
	}

	GetMixSliderPos(size, hCurrentBackground);
	mixslider = new MixSlider(size, this, kMix, *frame, contextmenu);
	frame->addView(mixslider);
	mixslider->setValue(ExternalState->Mix);

	GetBeatsAdjusterPos(size, hCurrentBackground);
	BeatsAdjuster = new IntAdjuster(size, this, kBeats, 1, MaxBeats, "", NULL,  " Beats", " Beat", 50, IATNormal, contextmenu, HIDSequencing, -1, false);
	frame->addView(BeatsAdjuster);
	BeatsAdjuster->MySetValue(ExternalState->getPattern().Beats);

	GetStepsAdjusterPos(size, hCurrentBackground);
	StepsAdjuster = new IntAdjuster(size, this, kSteps, 1, MaxSteps, "", NULL,  " Substeps", " Substep", 65, IATNormal, contextmenu, HIDSequencing, -1, false);
	frame->addView(StepsAdjuster);
	StepsAdjuster->MySetValue(ExternalState->getPattern().Steps);

	GetBPMAdjusterPos(size, hCurrentBackground);
	BPMAdjuster = new IntAdjuster(size, this, kBPM, 30, 250, "", NULL,  " BPM", " BPM", 46, IATNormal, contextmenu, HIDSequencing, -1, false);
	frame->addView(BPMAdjuster);
	BPMAdjuster->MySetValue((int)ExternalState->Sequencer.BPM);

	GetHostSyncSelectorPos(size, hCurrentBackground);
	HostSyncSelector = new CheckBox(size, this, kHostSync, "Host Sync", contextmenu, HIDSequencing, false);
	HostSyncSelector->SetColor(ControlBk, ControlBk, kWhiteCColor);
	frame->addView(HostSyncSelector);
	HostSyncSelector->MySetValue(ExternalState->Sequencer.HostSync);

	GetInLevelSliderPos(size, hCurrentBackground);
	InLevel = new IOSlider(size, this, kInLevel, false, contextmenu);
	InLevel->setValue(ExternalState->InLevel);
	frame->addView(InLevel);

	GetOutLevelSliderPos(size, hCurrentBackground);
	OutLevel = new IOSlider(size, this, kOutLevel, true, contextmenu);
	OutLevel->setValue(ExternalState->OutLevel);
	frame->addView(OutLevel);

	CPresets presets;
	GetPresetMenuPos(size, hCurrentBackground);
	PresetMenu = new MyOptionMenu(size, this, kPresetMenu, "Presets", true, 0, NULL);
	for(i=0; i<presets.getNumPresets(); i++)	PresetMenu->addEntry((char *)presets.getPresetName(i), i);	
	PresetMenu->addEntry( "-", i );
	i++;
	PresetMenu->addEntry( "Import patch from old version...", i );
	frame->addView(PresetMenu);

	GetSoundMenuPos(size, hCurrentBackground);
	SoundMenu = new MyOptionMenu(size, this, kSoundMenu, "Sound", true, 0, NULL);
	for(i=0; i<numSoundMenuOptions; i++)	SoundMenu->addEntry(SoundMenuOptionStr[i], i);
	frame->addView(SoundMenu);

	PopupSoundMenu = new PopupMenu((HWND)ptr, this, kPopupSoundMenu);
	for(i=0; i<numSoundMenuOptions; i++)	PopupSoundMenu->addEntry(SoundMenuOptionStr[i], i);

	PresetSoundMenu = new PopupMenu((HWND)ptr, this, kPresetSoundMenu);
	for(i=0; i<GetNumPresetSounds(); i++)	PresetSoundMenu->addEntry((char *)GetPresetSoundName(i), i);

	GetMidiMenuPos(size, hCurrentBackground);
	MidiMenu = new MyOptionMenu(size, this, kMidiMenu, "Midi Map", true, 0, NULL);
	for(i=0; i<numMidiMenuOptions; i++)	MidiMenu->addEntry(MidiMenuOptionStr[i], i);
	frame->addView(MidiMenu);

	GetWindowSizeMenuPos(size, hCurrentBackground);
	WindowSizeMenu = new MyOptionMenu(size, this, kWindowSizeMenu, "Window Size", true, 0, NULL);
	WindowSizeMenu->addEntry("Small <-", WSmall);
	WindowSizeMenu->addEntry("Large", WLarge);
	frame->addView(WindowSizeMenu);

	GetHelpMenuPos(size, hCurrentBackground);
	HelpMenu = new MyOptionMenu(size, this, kHelpMenu, "Help", true, 0, NULL);
	for(i=0; i<numHelpMenuOptions; i++)	HelpMenu->addEntry(HelpMenuOptionStr[i], i);
	frame->addView(HelpMenu);

	GetShuffleMenuPos(size, hCurrentBackground);
	ShuffleMenu = new MyOptionMenu(size, this, kShuffleMenu, "Shuffle", true, 0, contextmenu, HIDShuffle, ExternalMIDIMap_v2::kShuffleType, false);
	PopulateShuffleMenu();
	frame->addView(ShuffleMenu);

	GetShuffleSliderPos(size, hCurrentBackground);
	shuffleSlider = new ShuffleSlider(size, this, kShuffleSlider, *frame, contextmenu);
	frame->addView(shuffleSlider);

	GetPatternMenuPos(size, hCurrentBackground);
	PatternMenu = new MyOptionMenu(size, this, kPatternMenu, "<pattern>", true, 0, contextmenu, HIDPatternChange, ExternalMIDIMap_v2::kCurrentPattern, false);
	PopulatePatternMenu();
	frame->addView(PatternMenu);
	PatternNameEdit = new PopupNameEdit(size, this, kPatternNameEdit, contextmenu);

	GetPatternUpPos(size, hCurrentBackground);
	PatternUpButton = new SpinButton(size, this, kPatternUp, true, contextmenu, HIDPatternChange, ExternalMIDIMap_v2::kCurrentPattern);
	frame->addView(PatternUpButton);

	GetPatternDownPos(size, hCurrentBackground);
	PatternDownButton = new SpinButton(size, this, kPatternDown, false, contextmenu, HIDPatternChange, ExternalMIDIMap_v2::kCurrentPattern);
	frame->addView(PatternDownButton);
	SetPatternSpinEnables();

	GetSoundEditPanePos(size, hCurrentBackground);
	soundeditpane = new SoundEditPane(size, *frame, ExternalState, InternalState, sleepingreporter, contextmenu);

	isOpen = true;

	return true;
}


//-----------------------------------------------------------------------------
void Editor::close()
{
	ExternalState->MIDIMap.CloseDialog();

	about.Close();

	if(isOpen)
	{
		//delete sleeping reporters
		for(int i=0; i<kNumSleepingReporters; i++)
		{
			delete sleepingreporter[i];
		}

		delete contextmenu;

		frame->removeView(patterneditor);
		delete patterneditor;

		int i;
		for(i=0; i<NumSounds; i++)
		{
			delete mixerchannel[i];
		}

		frame->removeView(mixslider);
		delete mixslider;
		frame->removeView(BeatsAdjuster);
		delete BeatsAdjuster;
		frame->removeView(StepsAdjuster);
		delete StepsAdjuster;
		frame->removeView(BPMAdjuster);
		delete BPMAdjuster;
		frame->removeView(HostSyncSelector);
		delete HostSyncSelector;
		frame->removeView(InLevel);
		delete InLevel;
		frame->removeView(OutLevel);
		delete OutLevel;

		if(banner)
		{
			frame->removeView(banner);
			delete banner;
			banner = NULL;
		}

		frame->removeView(PresetMenu);
		delete PresetMenu;
		frame->removeView(SoundMenu);
		delete SoundMenu;

		delete PopupSoundMenu;
		delete PresetSoundMenu;

		frame->removeView(MidiMenu);
		delete MidiMenu;
		frame->removeView(WindowSizeMenu);
		delete WindowSizeMenu;
		frame->removeView(HelpMenu);
		delete HelpMenu;

		frame->removeView(ShuffleMenu);
		delete ShuffleMenu;
		frame->removeView(shuffleSlider);
		delete shuffleSlider;

		frame->removeView(PatternMenu);
		delete PatternMenu;
		frame->removeView(PatternNameEdit);
		delete PatternNameEdit;
		frame->removeView(PatternUpButton);
		delete PatternUpButton;
		frame->removeView(PatternDownButton);
		delete PatternDownButton;

		delete soundeditpane;

		isOpen = false;
	}

	if(frame)
	{
		delete frame;
		frame = NULL;
	}
}




void Editor::PostError(char *msg)
{
	ErrorMsg = msg;
}




void Editor::idle()
{
	AEffGUIEditor::idle();

	if(ErrorMsg)
	{
		char *temp = ErrorMsg;
		ErrorMsg=NULL;
		MessageBox(NULL, temp, "AnarchyRhythms", MB_OK);
	}
}


//-----------------------------------------------------------------------------
void Editor::valueChanged(CDrawContext* context, CControl* control)
{
	int tag = control->getTag();

	CRect size;

	switch(tag)
	{
		case kMix:

			ExternalState->Mix = control->getValue();
			break;

		case kBeats:

			ExternalState->getPattern().Beats = BeatsAdjuster->MyGetValue();
			patterneditor->redraw();
			
			break;

		case kSteps:

			ExternalState->getPattern().Steps = StepsAdjuster->MyGetValue();
			patterneditor->redraw();

			break;

		case kBPM:

			ExternalState->Sequencer.BPM = (float)BPMAdjuster->MyGetValue();

			break;

		case kHostSync:

			ExternalState->Sequencer.HostSync = HostSyncSelector->MyGetValue();
			break;

		case kInLevel:

			ExternalState->InLevel = InLevel->getValue();
			break;

		case kOutLevel:

			ExternalState->OutLevel = OutLevel->getValue();
			break;

		case kBanner:

			break;

		case kPresetMenu:

			{
			CPresets presets;
			if( PresetMenu->getCurrent() < presets.getNumPresets() )
				((BaseDLL *) effect)->LoadPresetPatternData(PresetMenu->getCurrent());
			else
				importLegacyPatch();
			}

			break;

		case kHelpMenu:

			switch(HelpMenu->getCurrent())
			{
				case kHelpHelp:				SpawnHelp(HIDContents);				break;
				case kHelpAbout:			about.Open((HWND)systemWindow);		break;

				case kHelpOnlineSupport:	LoadOnlineSupport(NULL);			break;

				case kRandomize:			((BaseDLL *) effect)->Randomize();	break;

			}
			break;

		case kSoundMenu:

			switch(SoundMenu->getCurrent())
			{
				case kLoadPresetSound:	PresetSoundMenu->Popup(context);			break;
				case kLoadUserSound:	((BaseDLL *) effect)->LoadSoundData();		break;
				case kSaveUserSound:	((BaseDLL *) effect)->SaveSoundData();		break;
			}
			break;

		case kPopupSoundMenu:

			switch(PopupSoundMenu->getCurrent())
			{
				case kLoadPresetSound:	PresetSoundMenu->Popup(context);			break;
				case kLoadUserSound:	((BaseDLL *) effect)->LoadSoundData();		break;
				case kSaveUserSound:	((BaseDLL *) effect)->SaveSoundData();		break;
			}
			return;//don't redraw control!

		case kMidiMenu:
	
			switch(MidiMenu->getCurrent())
			{
				case kEditMidiAutomation:		ExternalState->MIDIMap.Edit((HWND)systemWindow);	break;
				case kLoadMidiAutomation:		ExternalState->MIDIMap.Load((HWND)systemWindow);	break;
				case kSaveMidiAutomation:		ExternalState->MIDIMap.Save();						break;
			}
			break;

		case kPresetSoundMenu:

			((BaseDLL *) effect)->LoadPresetSoundData(PresetSoundMenu->getCurrent());
			return;//don't redraw control!

		case kWindowSizeMenu:

			ExternalState->WindowSize = (WindowSizeEnum)WindowSizeMenu->getCurrent();
			ResizeWindow();
			return;	//bypass control->redraw()!

		case kShuffleMenu:

			ExternalState->getPattern().ShuffleType = (ShuffleTypeEnum) ShuffleMenu->getCurrent();
			InternalState->GenerateShuffleLookup(ExternalState->getPattern());
			patterneditor->redraw();
			PopulateShuffleMenu();
			break;

		case kShuffleSlider:

			ExternalState->getPattern().ShuffleAmount = control->getValue();
			InternalState->GenerateShuffleLookup(ExternalState->getPattern());
			patterneditor->redraw();
			break;

		case kPatternMenu:

			switch( PatternMenu->getCurrent() )
			{
				case kRenamePattern:
					frame->removeView(PatternMenu);
					frame->addView(PatternNameEdit);
					PatternNameEdit->setText(ExternalState->getPattern().Name);
					PatternNameEdit->popup(context);
					bEditingPatternName = true;
					return;
				case kClearPattern:		
					ExternalState->getPattern().Clear();
					InternalState->GenerateShuffleLookup(ExternalState->getPattern());
					PopulateShuffleMenu();
					PopulatePatternMenu();
					patterneditor->redraw();
					return;		//bypass control->redraw()!
				case kCopyPattern:		
					InternalState->copyPattern(ExternalState->getPattern());
					return;		//bypass control->redraw()!
				case kPastePattern:	
					if(InternalState->CopiedPatternSet)
					{
						ExternalState->getPattern() = InternalState->CopiedPattern;
						OnPatternChange();
					}
					else
					{
						PostError("You must copy a pattern before you can paste it!");
					}
					return;
				default:	//assume it's a pattern change
					ExternalState->nCurrentPattern = PatternMenu->getCurrent();
					OnPatternChange();
					return;		//bypass control->redraw()!
			}
			break;

		case kPatternNameEdit:
		{
			frame->removeView(PatternNameEdit);
			frame->addView(PatternMenu);
			bEditingPatternName = false;

			static char buffer[100];
			PatternNameEdit->getText(buffer);
			buffer[MaxPatternNameLength-1] = 0;
			strcpy(ExternalState->getPattern().Name, buffer);

			PopulatePatternMenu();
			break;
		}

		case kPatternUp:
			if(ExternalState->nCurrentPattern <= 0) return;	//failsafe
			if( bEditingPatternName ) return;

			ExternalState->nCurrentPattern --;
			OnPatternChange();

			return;		//bypass control->redraw()!

		case kPatternDown:
			if(ExternalState->nCurrentPattern >= nPatterns-1) return;	//failsafe
			if( bEditingPatternName ) return;

			ExternalState->nCurrentPattern ++;
			OnPatternChange();
			return;		//bypass control->redraw()!

		case kContextMenu:

			if(contextmenu->getCurrent() == 0)
				SpawnHelp(contextmenu->HelpID);		//context help

			if(contextmenu->getCurrent() == contextmenu->MidiMenuIndex)
				ExternalState->MIDIMap.Edit((HWND)systemWindow, contextmenu->MIDIParam);	

			if(contextmenu->getCurrent() == contextmenu->SoundMenuIndex)
				PopupSoundMenu->Popup(context);

			return;//don't redraw control!

		default:	//assume its a sleeping int reporter
			
			tag -= kFirstSleepingReporter;
			assert((tag >= 0) && (tag < kNumSleepingReporters));

			switch(tag)		//switch amongst the sleeping int reporters
			{
				case kSelSoundChange:

					if(((SleepingReporter*)control)->GetInt() != ExternalState->SelectedSound)
					{
						mixerchannel[ExternalState->SelectedSound]->SetHighlight(false);
						ExternalState->SelectedSound = ((SleepingReporter*)control)->GetInt();
						mixerchannel[ExternalState->SelectedSound]->SetHighlight(true);
						patterneditor->SetSelSound(ExternalState->SelectedSound);
						patterneditor->redraw();
						soundeditpane->ValueChanged(tag);

						//RedrawWindow((HWND)frame->getSystemWindow(), NULL, NULL, RDW_INVALIDATE|RDW_ERASE);
					}

					return;	//bypass control->redraw()!

				case kLevelChange:
				case kCombChange:
				case kCompressionChange:
				case kOscillatorLevelChange:
				case kPanChange:
				case kPanEnvChange:
				case kFilterWidthChange:
				case kPitchChange:
				case kPitchEnvChange:
				case kLengthChange:
				case kFeedBackChange:

					mixerchannel[((SleepingReporter*)control)->GetInt()]->ValueChanged(tag);

					if(((SleepingReporter*)control)->GetInt() == ExternalState->SelectedSound)
					{
						soundeditpane->ValueChanged(tag);
					}

					return;	//bypass control->redraw()!


				default:	//assume its a sound name change

					assert(tag >= kFirstSoundNameChange);
					assert(tag <= kLastSoundNameChange);

					mixerchannel[ExternalState->SelectedSound]->ValueChanged(tag);
					soundeditpane->ValueChanged(tag);

					break;
			}
			
			break;
	}

	control->redraw();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



void Editor::BPMChanged()
{
	if(isOpen)
	{
		BPMAdjuster->MySetValue((int)ExternalState->Sequencer.BPM);
		BPMAdjuster->redraw();
	}
}



void Editor::HostSyncChanged()
{
	if(isOpen)
	{
		HostSyncSelector->MySetValue(ExternalState->Sequencer.HostSync);
		HostSyncSelector->redraw();
	}
}




void Editor::AllExternalDataChanged()
{
	if(isOpen)
	{
		patterneditor->SetSelSound(ExternalState->SelectedSound);
		patterneditor->redraw();

		for(int i=0; i<NumSounds; i++)
		{
			mixerchannel[i]->SetHighlight(i == ExternalState->SelectedSound);
			mixerchannel[i]->AllValuesChanged();
		}

		mixslider->setValue(ExternalState->Mix);
		mixslider->redraw();

		BeatsAdjuster->MySetValue(ExternalState->getPattern().Beats);
		BeatsAdjuster->redraw();

		StepsAdjuster->MySetValue(ExternalState->getPattern().Steps);
		StepsAdjuster->redraw();

		BPMAdjuster->MySetValue((int)ExternalState->Sequencer.BPM);
		BPMAdjuster->redraw();

		HostSyncSelector->MySetValue(ExternalState->Sequencer.HostSync);
		HostSyncSelector->redraw();

		InLevel->setValue(ExternalState->InLevel);
		InLevel->redraw();

		OutLevel->setValue(ExternalState->OutLevel);
		OutLevel->redraw();

		soundeditpane->AllValuesChanged();

		bool bLargeWindowNow = (ExternalState->WindowSize == WLarge);
		bool bLargeWindowThen = (hCurrentBackground == hBackgroundLarge);

		if(bLargeWindowNow != bLargeWindowThen)
		{	
			ResizeWindow();
		}

		PopulateShuffleMenu();

		shuffleSlider->setValue(ExternalState->getPattern().ShuffleAmount);
		shuffleSlider->redraw();

		PopulatePatternMenu();
		SetPatternSpinEnables();
	}
}


void Editor::ResizeWindow()
{
	if(isOpen)
	{
		WindowSizeEnum newsize = ExternalState->WindowSize;

		WindowSizeMenu->removeAllEntry();
		WindowSizeMenu->addEntry((newsize==WSmall) ? "Small <-" : "Small", WSmall);
		WindowSizeMenu->addEntry((newsize==WLarge) ? "Large <-" : "Large", WLarge);

		switch(newsize)
		{
			case WSmall:	hCurrentBackground = hBackgroundSmall;	break;
			case WLarge:	hCurrentBackground = hBackgroundLarge;	break;
			default:		assert(false);							break;
		}

#ifdef EVALUATIONVERSION
	int heightAdditive = nBannerHeight;
#else
	int heightAdditive = 0;
#endif

		frame->setSize(hCurrentBackground->getWidth(), hCurrentBackground->getHeight() + heightAdditive);
		frame->setBackground(hCurrentBackground);

		CRect size;

		GetPatternEditorPos(size, hCurrentBackground);
		patterneditor->Resize(size);

		for(int i=0; i<NumSounds; i++)
		{
			GetMixerChannelPos(size, hCurrentBackground, i);
			mixerchannel[i]->Resize(size);
		}

		GetMixSliderPos(size, hCurrentBackground);
		mixslider->Resize(size);

		GetBeatsAdjusterPos(size, hCurrentBackground);
		BeatsAdjuster->Resize(size);

		GetStepsAdjusterPos(size, hCurrentBackground);
		StepsAdjuster->Resize(size);

		GetBPMAdjusterPos(size, hCurrentBackground);
		BPMAdjuster->Resize(size);

		GetHostSyncSelectorPos(size, hCurrentBackground);
		HostSyncSelector->Resize(size);

		GetInLevelSliderPos(size, hCurrentBackground);
		InLevel->Resize(size);

		GetOutLevelSliderPos(size, hCurrentBackground);
		OutLevel->Resize(size);

		GetShuffleMenuPos(size, hCurrentBackground);
		ShuffleMenu->Resize(size);

		GetShuffleSliderPos(size, hCurrentBackground);
		shuffleSlider->Resize(size);

		GetPatternMenuPos(size, hCurrentBackground);
		PatternMenu->Resize(size);
		PatternNameEdit->Resize(size);

		GetPatternUpPos(size, hCurrentBackground);
		PatternUpButton->Resize(size);

		GetPatternDownPos(size, hCurrentBackground);
		PatternDownButton->Resize(size);

		GetSoundEditPanePos(size, hCurrentBackground);
		soundeditpane->Resize(size);

		if(banner)
		{
			GetBannerPos(size, hCurrentBackground);
			banner->Resize(size);
		}

		ERect *rect;
		getRect(&rect);
		AEffGUIEditor::draw(rect);
	}
}




void Editor::NotifyMIDIAutomate(int MIDIParam)
{
	int MIDISoundParam;

	SleepingReporter *reporter;

	if(isOpen)
	{
		switch(MIDIParam)
		{
			case ExternalMIDIMap_v2::kMIDIInput:	InLevel->setValue(ExternalState->InLevel);		InLevel->redraw();		break;
			case ExternalMIDIMap_v2::kMIDIDryWet:	mixslider->setValue(ExternalState->Mix);		mixslider->redraw();	break;
			case ExternalMIDIMap_v2::kMIDIOutput:	OutLevel->setValue(ExternalState->OutLevel);	OutLevel->redraw();		break;
			
			case ExternalMIDIMap_v2::kShuffleType:	 
				PopulateShuffleMenu();
				patterneditor->redraw();
				break;

			case ExternalMIDIMap_v2::kShuffleAmount:	 
				shuffleSlider->setValue(ExternalState->getPattern().ShuffleAmount);
				patterneditor->redraw();
				break;

			case ExternalMIDIMap_v2::kCurrentPattern:
				PopulateShuffleMenu();
				shuffleSlider->setValue(ExternalState->getPattern().ShuffleAmount);
				shuffleSlider->redraw();
				PopulatePatternMenu();
				patterneditor->redraw();
				SetPatternSpinEnables();
				break;

			default:	//must be a sound param

				MIDISoundParam = MIDIParam - ExternalMIDIMap_v2::numMIDIParams;
				
				switch(MIDISoundParam%ExternalMIDIMap_v2::numMIDISoundParams)
				{
					case ExternalMIDIMap_v2::kMIDILevel:		reporter = sleepingreporter[kLevelChange];				break;
					case ExternalMIDIMap_v2::kMIDIComb:			reporter = sleepingreporter[kCombChange];				break;
					case ExternalMIDIMap_v2::kMIDICompression:	reporter = sleepingreporter[kCompressionChange];				break;
					case ExternalMIDIMap_v2::kMIDIPan:			reporter = sleepingreporter[kPanChange];				break;
					case ExternalMIDIMap_v2::kMIDIPanEnv:		reporter = sleepingreporter[kPanEnvChange];				break;
					case ExternalMIDIMap_v2::kMIDIPitch:		reporter = sleepingreporter[kPitchChange];				break;
					case ExternalMIDIMap_v2::kMIDIPitchEnv:		reporter = sleepingreporter[kPitchEnvChange];			break;
					case ExternalMIDIMap_v2::kMIDIFilterWidth:	reporter = sleepingreporter[kFilterWidthChange];		break;
					case ExternalMIDIMap_v2::kMIDILength:		reporter = sleepingreporter[kLengthChange];				break;
					case ExternalMIDIMap_v2::kMIDIOscillator:	reporter = sleepingreporter[kOscillatorLevelChange];	break;
					case ExternalMIDIMap_v2::kMIDIFeedBack:		reporter = sleepingreporter[kFeedBackChange];			break;
				}

				reporter->SetInt((NumSounds-1) - MIDIParam/ExternalMIDIMap_v2::numMIDISoundParams);
				break;
		}
	}
}


void Editor::OnPatternChange()
{
	InternalState->GenerateShuffleLookup(ExternalState->getPattern());
	PopulateShuffleMenu();
	shuffleSlider->setValue(ExternalState->getPattern().ShuffleAmount);
	shuffleSlider->redraw();
	PopulatePatternMenu();
	patterneditor->redraw();
	StepsAdjuster->MySetValue(ExternalState->getPattern().Steps);
	StepsAdjuster->redraw();
	BeatsAdjuster->MySetValue(ExternalState->getPattern().Beats);
	BeatsAdjuster->redraw();
	SetPatternSpinEnables();
}



void Editor::setParameter(long index, float value)
{
	NotifyMIDIAutomate(index);
}


void Editor::PopulateShuffleMenu()
{
	ShuffleMenu->removeAllEntry();
	ShuffleMenu->addEntry( (ExternalState->getPattern().ShuffleType == stpBeat) ? "Beat <-" : "Beat", stpBeat);
	ShuffleMenu->addEntry( (ExternalState->getPattern().ShuffleType == stpStep ) ? "Step <-" : "Step", stpStep);
}

void Editor::PopulatePatternMenu()
{
	PatternMenu->removeAllEntry();

	int i;
	for(int i=0; i<nPatterns; i++)
	{
		char name[50];
		strcpy(name, ExternalState->Pattern[i].Name);

		if(i == ExternalState->nCurrentPattern)
		{
			strcat(name, " <-");
		}

		PatternMenu->addEntry( name, i );
	}

	for(i=0; i<numPatternMenuOptions; i++)	
		PatternMenu->addEntry(PatternMenuOptionStr[i], i+nPatterns);

	PatternMenu->SetText(ExternalState->getPattern().Name);
	PatternMenu->redraw();
}


void Editor::SetPatternSpinEnables()
{
	PatternUpButton->Enable(ExternalState->nCurrentPattern > 0);
	PatternDownButton->Enable(ExternalState->nCurrentPattern < nPatterns-1);
}


long InvertLong(long v)
{
	long ret;

	char *pIn = (char *)&v;
	char *pOut = (char *)&ret;

	pOut[0] = pIn[3];
	pOut[1] = pIn[2];
	pOut[2] = pIn[1];
	pOut[3] = pIn[0];

	return ret;
}


void Editor::importLegacyPatch()
{
	char FileName[MAX_PATH];

	strcpy(FileName, "" );
	
	OPENFILENAME ofn;

    ofn.lStructSize 		=sizeof(OPENFILENAME);
    ofn.hwndOwner 			=NULL;
    ofn.hInstance 			=NULL;
    ofn.lpstrFilter 		="Legacy AnarchyRhythms patches (*.FXP)\0*.fxp\0\0";
    ofn.lpstrCustomFilter 	=NULL;     
    ofn.nMaxCustFilter 		=0;
    ofn.nFilterIndex 		=0;
    ofn.lpstrFile 			=FileName;
    ofn.nMaxFile 			=MAX_PATH;
    ofn.lpstrFileTitle 		=NULL;
    ofn.nMaxFileTitle 		=0;
    ofn.lpstrInitialDir 	=NULL;
    ofn.lpstrTitle 			="Import Patch from another version of AnarchyRhythms";
    ofn.Flags				=OFN_OVERWRITEPROMPT; 
    ofn.nFileOffset			=0; 
    ofn.nFileExtension		=0; 
    ofn.lpstrDefExt			="FXP"; 
    ofn.lCustData			=0; 
    ofn.lpfnHook			=NULL; 
    ofn.lpTemplateName		=0; 

	if( !GetOpenFileName(&ofn)) return;

	FILE *f = fopen(FileName, "rb");
	if(!f)		
	{
		PostError("Can't find the specified file");
		return;
	}

	//load the header

	long pHeaderData[15];

	int nHeaderWords = 15;//bFullHeader || (!(effect->flags & effFlagsProgramChunks)) ? (effect->flags & effFlagsProgramChunks) ? 15 : 14 : 1;

	fread(pHeaderData, sizeof(long), nHeaderWords, f);

	//validate the header

	bool bHeaderOK = true;

	if( pHeaderData[0] != InvertLong(0x43636E4B) )
	{
		PostError("This is not a valid VST patch file");
		fclose( f );
		return;
	}

	if( pHeaderData[2] != InvertLong(0x46504368) )
	{
		PostError("This is not a valid VST patch file");
		fclose( f );
		return;
	}

	if( pHeaderData[4] != InvertLong( uniqueID ) )
	{
		PostError("This is not an AnarchyRhythms patch file");
		fclose( f );
		return;
	}

	int numberOfBytes = InvertLong( pHeaderData[14] );
	if( numberOfBytes != sizeof( ExternalStateData_v1 ) && numberOfBytes != sizeof( ExternalStateData_v2 ) )
	{
		PostError("This is not an AnarchyRhythms patch file");
		fclose( f );
		return;
	}

	//read the data

	char buffer[ sizeof( ExternalStateData_v2 ) ];

	fread( buffer, 1, numberOfBytes, f);

	effect->setChunk( buffer, numberOfBytes );

	fclose(f);
}



