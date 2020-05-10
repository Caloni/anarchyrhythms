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

© 1999, Steinberg Soft und Hardware GmbH, All Rights Reserved
  -  ANARCHY RHYTHMS
  LEIGHTON HARGREAVES
-----------------------------------------------------------------------------*/
#ifndef __ADEditor
#define __ADEditor


// include VSTGUI
#ifndef __vstgui__
#include "vstgui.h"
#endif


#include "MyConsts.h"
#include "About.h"


class PatternEditor;
class ExternalStateData_v2;
class InternalStateData;
class MixerChannelEditor;
class MixSlider;
class ShuffleSlider;
class IntAdjuster;
class SleepingReporter;
class MyOptionMenu;
class SoundEditPane;
class CheckBox;
class PopupMenu;
class IOSlider;
class ContextMenu;
class Splash;
class Banner;
class PopupNameEdit;
class SpinButton;


enum EditorControlEnum
{
	kMix = 0,
	kSteps,
	kBeats,
	kBPM,
	kHostSync,
	kInLevel,
	kOutLevel,
	kBanner,
	kPresetMenu,
	kSoundMenu,
	kPopupSoundMenu,
	kMidiMenu,
	kWindowSizeMenu,
	kHelpMenu,

	kPresetSoundMenu,
	kShuffleMenu,
	kShuffleSlider,
	kPatternMenu,
	kPatternNameEdit,
	kPatternUp,
	kPatternDown,
	kContextMenu,
	kFirstSleepingReporter,
	kLastSleepingReporter = kFirstSleepingReporter + kNumSleepingReporters-1
};


enum SoundMenuOptionEnum
{
	kLoadPresetSound = 0,
	kSoundMenuSeperator,
	kLoadUserSound,
	kSaveUserSound,
	numSoundMenuOptions
};


enum MidiMenuOptionEnum
{
	kEditMidiAutomation = 0,
	kMidiMenuSeperator,
	kLoadMidiAutomation,
	kSaveMidiAutomation,
	numMidiMenuOptions
};


enum HelpMenuOptionEnum
{
	kHelpHelp = 0,
	kHelpSeparator1,
	kHelpAbout,
	kHelpSeparator2,
	kHelpOnlineSupport,
	kHelpSeparator3,
	kRandomize,
	numHelpMenuOptions
};


enum PatternMenuOptionEnum
{
	kPatternMenuSeperator = nPatterns,
	kRenamePattern, 
	kClearPattern,
	kCopyPattern,
	kPastePattern,
	numPatternMenuOptions = 5
};



//-----------------------------------------------------------------------------
class Editor : public AEffGUIEditor, public CControlListener
{
public:
	Editor(AudioEffect *effect);
	virtual ~Editor();

	void BPMChanged();
	void HostSyncChanged();
	void AllExternalDataChanged();

	void PostError(char *msg);

	void NotifyMIDIAutomate(int MIDIParam);


protected:
 	virtual long open(void *ptr);
	virtual void close();

	virtual void valueChanged(CDrawContext* context, CControl* control);
	virtual void idle();

	virtual void setParameter(long index, float value);

private:

	ExternalStateData_v2 *ExternalState;
	InternalStateData *InternalState;

	bool isOpen;

	char *ErrorMsg;		//set to NULL when no message needs reporting

	// Controls
	PatternEditor *patterneditor;
	MixerChannelEditor *mixerchannel[NumSounds];
	MixSlider *mixslider;
	IntAdjuster *StepsAdjuster;
	IntAdjuster *BeatsAdjuster;
	IntAdjuster *BPMAdjuster;
	CheckBox *HostSyncSelector;
	MyOptionMenu *ShuffleMenu;
	ShuffleSlider *shuffleSlider;
	MyOptionMenu *PatternMenu;
	PopupNameEdit *PatternNameEdit;
	SpinButton *PatternUpButton;
	SpinButton *PatternDownButton;

	IOSlider *InLevel;
	IOSlider *OutLevel;
	Banner *banner;
	MyOptionMenu *PresetMenu;
	MyOptionMenu *SoundMenu;
	PopupMenu *PopupSoundMenu;
	MyOptionMenu *MidiMenu;
	MyOptionMenu *WindowSizeMenu;
	MyOptionMenu *HelpMenu;

	PopupMenu *PresetSoundMenu;

	ContextMenu *contextmenu;

	SoundEditPane *soundeditpane;

	SleepingReporter *sleepingreporter[kNumSleepingReporters];

	About about;

	// Bitmaps
	CBitmap *hBackgroundSmall, *hBackgroundLarge;

	CBitmap *hCurrentBackground;

	bool bEditingPatternName;

	//private functions

	void ResizeWindow();

	void OnPatternChange();
	void PopulateShuffleMenu();
	void PopulatePatternMenu();
	void SetPatternSpinEnables();

	void importLegacyPatch();
};



#endif
 