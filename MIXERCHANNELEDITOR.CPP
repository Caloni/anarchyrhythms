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


//MixerChannelEditor.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 16-1-02



#include "MixerChannelEditor.h"
#include "Knob.h"
#include "LevelSlider.h"
#include "MyConsts.h"
#include "MyHelpers.h"
#include "ExternalStateData_v2.h"
#include "SoundNameEdit.h"
#include "ControlPositioners.h"
#include "SleepingReporter.h"
#include "LightUpBox.h"
#include "MixerOutline.h"
#include "ContextMenu.h"

#include <assert.h>





MixerChannelEditor::MixerChannelEditor(CRect &size, SoundData_v2 *_Sound, CFrame &_frame, int _MyIndex, SleepingReporter **_sleepingreporter, bool InitialHighlight, ContextMenu *_contextmenu)
	:	Sound(_Sound),
		MyIndex(_MyIndex),
		frame(_frame),
		sleepingreporter(_sleepingreporter),
		contextmenu(_contextmenu)
{
	for(int i=0; i<NumMixerChannelControls; i++)
	{
		switch(i)
		{
			case kLevel:	
				
				GetMixerLevelPos(ControlPos[i], size);

				controls[i] = new LevelSlider(ControlPos[i], this, i, MyIndex, frame, sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu);

				controls[i]->setValue((float)Sound->Level/100.f);

				break;

			case kComb:

				GetMixerCombPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Comb", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerComb, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDIComb);

				controls[i]->setValue((float)Sound->Comb/99.f);

				break;

			case kCompression:

				GetMixerCompressionPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Comp", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerCompression, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDICompression);

				controls[i]->setValue((float)Sound->Compression/100.f);

				break;

			case kPan:

				GetMixerPanPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Pan", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerPan, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDIPan);

				controls[i]->setValue((float)(Sound->Pan+100)/200.f);

				break;

			case kPitch:

				GetMixerPitchPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Pitch", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerPitch, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDIPitch);

				controls[i]->setValue((float)Sound->CoarsePitch / (float)MaxPitch);

				break;

			case kLength:

				GetMixerLengthPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Len", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerLength, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDILength);

				controls[i]->setValue(fsqrt((float)Sound->Length/(float)MaxLength));

				break;

			case kFilterWidth:

				GetMixerFilterWidthPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Filter", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerFilterWidth, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDIFilterWidth);

				controls[i]->setValue((float)(Sound->FilterWidth)/100.f);

				break;

			case kFeedBack:

				GetMixerFeedBackPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "Feed", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerFeedback, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDIFeedBack);

				controls[i]->setValue((float)(Sound->FeedBack)/99.f);

				break;

			case kOscillator:

				GetMixerOscillatorPos(ControlPos[i], size);

				controls[i] = new Knob(ControlPos[i], this, i, frame, MyIndex, "OSC", "", sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, HIDMixerOscillator, ExternalMIDIMap_v2::numMIDIParams+ExternalMIDIMap_v2::numMIDISoundParams*MyIndex+ExternalMIDIMap_v2::kMIDIOscillator);

				controls[i]->setValue((float)(Sound->Oscillator)/100.f);

				break;

			case kSolo:

				GetSoloButtonPos(ControlPos[i], size);

				controls[i] = new LightUpBox(ControlPos[i], this, i, "Solo", MyIndex, sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, true);
			
				((LightUpBox *)controls[i])->MySetValue(Sound->Solo);
				break;

			case kMute:

				GetMuteButtonPos(ControlPos[i], size);

				controls[i] = new LightUpBox(ControlPos[i], this, i, "Mute", MyIndex, sleepingreporter[kSelSoundChange], InitialHighlight, contextmenu, false);
			
				((LightUpBox *)controls[i])->MySetValue(Sound->Mute);
				break;

			case kNameEdit:

				GetMixerNameEditPos(ControlPos[i], size);

				controls[i] = new SoundNameEdit(ControlPos[i], this, i, sleepingreporter[kSelSoundChange], MyIndex, false, InitialHighlight, contextmenu);

				((CTextEdit *)controls[i])->setText(Sound->Name);

				break;

			case kTopOutline:

				GetMixerTopOutlinePos(ControlPos[i], size);
				controls[i] = new MixerOutline(ControlPos[i], MyIndex, sleepingreporter[kSelSoundChange], InitialHighlight, 0);
				break;

			case kLeftOutline:

				GetMixerLeftOutlinePos(ControlPos[i], size);
				controls[i] = new MixerOutline(ControlPos[i], MyIndex, sleepingreporter[kSelSoundChange], InitialHighlight, 1);
				break;

			case kBottomOutline:

				GetMixerBottomOutlinePos(ControlPos[i], size);
				controls[i] = new MixerOutline(ControlPos[i], MyIndex, sleepingreporter[kSelSoundChange], InitialHighlight, 2);
				break;

			case kRightOutline:

				GetMixerRightOutlinePos(ControlPos[i], size);
				controls[i] = new MixerOutline(ControlPos[i], MyIndex, sleepingreporter[kSelSoundChange], InitialHighlight, 3);
				break;

			default:	assert(false);		break;
		}

		frame.addView(controls[i]);
	}

}



MixerChannelEditor::~MixerChannelEditor()
{
	for(int i=0; i<NumMixerChannelControls; i++)
	{
		frame.removeView(controls[i]);
		delete controls[i];
	}
}











void MixerChannelEditor::valueChanged(CDrawContext* context, CControl* control)
{
	static char buf[100];

	float value = control->getValue();

	switch(control->getTag())
	{
		case kLevel:	

			Sound->Level = (int)(value * 100.f);
			sleepingreporter[kLevelChange]->SetInt(MyIndex);
			break;

		case kComb:

			Sound->Comb = (int)(value * 99.f);
			sleepingreporter[kCombChange]->SetInt(MyIndex);
			break;

		case kCompression:

			Sound->Compression = (int)(value * 100.f);
			sleepingreporter[kCompressionChange]->SetInt(MyIndex);
			break;
		
		case kPan:

			Sound->Pan = (int)(value * 200.f) - 100;
			sleepingreporter[kPanChange]->SetInt(MyIndex);
			break;

		case kPitch:

			Sound->CoarsePitch = (int)(value * (float)MaxPitch);
			sleepingreporter[kPitchChange]->SetInt(MyIndex);
			break;

		case kFilterWidth:

			Sound->FilterWidth = (int)(value * 100.f);
			sleepingreporter[kFilterWidthChange]->SetInt(MyIndex);
			break;

		case kLength:

			Sound->Length = (int)(value * value * (float)MaxLength);
			sleepingreporter[kLengthChange]->SetInt(MyIndex);
			break;

		case kOscillator:

			Sound->Oscillator = (int)(value * 100.f);
			sleepingreporter[kOscillatorLevelChange]->SetInt(MyIndex);
			break;

		case kFeedBack:

			Sound->FeedBack = (int)(value * 99.f);
			sleepingreporter[kFeedBackChange]->SetInt(MyIndex);
			break;

		case kSolo:

			Sound->Solo = ((LightUpBox *)control)->MyGetValue();
			
			if(Sound->Solo && Sound->Mute)
			{
				Sound->Mute = false;
				((LightUpBox *)controls[kMute])->MySetValue(false);
				controls[kMute]->redraw();
			}
				
			break;

		case kMute:

			Sound->Mute = ((LightUpBox *)control)->MyGetValue();

			if(Sound->Mute && Sound->Solo)
			{
				Sound->Solo = false;
				((LightUpBox *)controls[kSolo])->MySetValue(false);
				controls[kSolo]->redraw();
			}
			
			break;

		case kNameEdit:

			((CTextEdit *)control)->getText(buf);
			buf[MaxSoundNameLength-1] = 0;
			strcpy(Sound->Name, buf);

			sleepingreporter[kFirstSoundNameChange+MyIndex]->InformListener();

			return;		//bypass control->redraw()

		default:

			assert(false);
			break;
	}

	control->redraw();
}





void MixerChannelEditor::SetHighlight(bool _Highlight)
{
	((LevelSlider *)controls[0])->SetHighlight(_Highlight);
	((Knob *)controls[1])->SetHighlight(_Highlight);
	((Knob *)controls[2])->SetHighlight(_Highlight);
	((Knob *)controls[3])->SetHighlight(_Highlight);
	((Knob *)controls[4])->SetHighlight(_Highlight);
	((Knob *)controls[5])->SetHighlight(_Highlight);
	((Knob *)controls[6])->SetHighlight(_Highlight);
	((Knob *)controls[7])->SetHighlight(_Highlight);
	((Knob *)controls[8])->SetHighlight(_Highlight);
	((LightUpBox *)controls[9])->SetHighlight(_Highlight);
	((LightUpBox *)controls[10])->SetHighlight(_Highlight);
	((SoundNameEdit *)controls[11])->SetHighlight(_Highlight);
	((MixerOutline *)controls[12])->SetHighlight(_Highlight);
	((MixerOutline *)controls[13])->SetHighlight(_Highlight);
	((MixerOutline *)controls[14])->SetHighlight(_Highlight);
	((MixerOutline *)controls[15])->SetHighlight(_Highlight);
}




void MixerChannelEditor::Resize(CRect &newsize)
{
	for(int i=0; i<NumMixerChannelControls; i++)
	{
		switch(i)
		{
			case kLevel:		GetMixerLevelPos(ControlPos[i], newsize);			break;
			case kComb:			GetMixerCombPos(ControlPos[i], newsize);			break;
			case kCompression:	GetMixerCompressionPos(ControlPos[i], newsize);		break;
			case kPan:			GetMixerPanPos(ControlPos[i], newsize);				break;
			case kPitch:		GetMixerPitchPos(ControlPos[i], newsize);			break;
			case kLength:		GetMixerLengthPos(ControlPos[i], newsize);			break;
			case kFilterWidth:	GetMixerFilterWidthPos(ControlPos[i], newsize);		break;
			case kFeedBack:		GetMixerFeedBackPos(ControlPos[i], newsize);		break;
			case kOscillator:	GetMixerOscillatorPos(ControlPos[i], newsize);		break;
			case kSolo:			GetSoloButtonPos(ControlPos[i], newsize);			break;
			case kMute:			GetMuteButtonPos(ControlPos[i], newsize);			break;
			case kNameEdit:		GetMixerNameEditPos(ControlPos[i], newsize);		break;
			
			case kTopOutline:	GetMixerTopOutlinePos(ControlPos[i], newsize);		break;
			case kLeftOutline:	GetMixerLeftOutlinePos(ControlPos[i], newsize);		break;
			case kBottomOutline:GetMixerBottomOutlinePos(ControlPos[i], newsize);	break;
			case kRightOutline:	GetMixerRightOutlinePos(ControlPos[i], newsize);	break;

			default:			assert(false);										break;
		}
	}

	((LevelSlider *)controls[0])->Resize(ControlPos[0]);
	((Knob *)controls[1])->Resize(ControlPos[1]);
	((Knob *)controls[2])->Resize(ControlPos[2]);
	((Knob *)controls[3])->Resize(ControlPos[3]);
	((Knob *)controls[4])->Resize(ControlPos[4]);
	((Knob *)controls[5])->Resize(ControlPos[5]);
	((Knob *)controls[6])->Resize(ControlPos[6]);
	((Knob *)controls[7])->Resize(ControlPos[7]);
	((Knob *)controls[8])->Resize(ControlPos[8]);
	((LightUpBox *)controls[9])->Resize(ControlPos[9]);
	((LightUpBox *)controls[10])->Resize(ControlPos[10]);
	((SoundNameEdit *)controls[11])->Resize(ControlPos[11]);
	((MixerOutline *)controls[12])->Resize(ControlPos[12]);
	((MixerOutline *)controls[13])->Resize(ControlPos[13]);
	((MixerOutline *)controls[14])->Resize(ControlPos[14]);
	((MixerOutline *)controls[15])->Resize(ControlPos[15]);
}



void MixerChannelEditor::AllValuesChanged()
{
	controls[kLevel]->setValue((float)Sound->Level/100.f);
	controls[kComb]->setValue((float)Sound->Comb/99);
	controls[kCompression]->setValue((float)Sound->Compression/100.f);
	controls[kPitch]->setValue((float)Sound->CoarsePitch/(float)MaxPitch);
	controls[kPan]->setValue((float)(Sound->Pan+100)/200.f);
	controls[kFilterWidth]->setValue((float)(Sound->FilterWidth)/100.f);
	controls[kOscillator]->setValue((float)(Sound->Oscillator)/100.f);
	controls[kLength]->setValue(fsqrt((float)Sound->Length/(float)MaxLength));
	controls[kFeedBack]->setValue((float)(Sound->FeedBack)/99.f);
	((LightUpBox *)controls[kSolo])->MySetValue(Sound->Solo);
	((LightUpBox *)controls[kMute])->MySetValue(Sound->Mute);
	((SoundNameEdit *)controls[kNameEdit])->setText(Sound->Name);

	for(int i=0; i<NumMixerChannelControls; i++)
	{
		controls[i]->redraw();
	}
}




void MixerChannelEditor::ValueChanged(int SleepingReporterNum)
{
	switch(SleepingReporterNum)
	{
		case kSelSoundChange:	MessageBox(NULL, "MB12367", "error", MB_OK);	break;	//this should never happen!

		case kLevelChange:
			controls[kLevel]->setValue((float)Sound->Level/100.f);
			controls[kLevel]->redraw();
			break;	

		case kCombChange:
			controls[kComb]->setValue((float)Sound->Comb/99.f);
			controls[kComb]->redraw();
			break;
		
		case kCompressionChange:
			controls[kCompression]->setValue((float)Sound->Compression/100.f);
			controls[kCompression]->redraw();
			break;

		case kPitchChange:
			controls[kPitch]->setValue((float)Sound->CoarsePitch/(float)MaxPitch);
			controls[kPitch]->redraw();
			break;

		case kPanChange:
			controls[kPan]->setValue((float)(Sound->Pan+100)/200.f);
			controls[kPan]->redraw();
			break;	

		case kPitchEnvChange:
		case kPanEnvChange:
			break;

		case kFilterWidthChange:
			controls[kFilterWidth]->setValue((float)(Sound->FilterWidth)/100.f);
			controls[kFilterWidth]->redraw();
			break;	

		case kOscillatorLevelChange:
			controls[kOscillator]->setValue((float)(Sound->Oscillator)/100.f);
			controls[kOscillator]->redraw();
			break;	

		case kLengthChange:
			controls[kLength]->setValue(fsqrt((float)Sound->Length/(float)MaxLength));
			controls[kLength]->redraw();
			break;	

		case kFeedBackChange:
			controls[kFeedBack]->setValue((float)(Sound->FeedBack)/99.f);
			controls[kFeedBack]->redraw();
			break;

		default:	//assume its a sound name change

			((SoundNameEdit *)controls[kNameEdit])->setText(Sound->Name);
			controls[kNameEdit]->redraw();

			break;
	}
}



