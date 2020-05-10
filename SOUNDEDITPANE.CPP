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


//SoundEditPane.cpp	-	Anarchy RHYTHMS

//Leighton Hargreaves 5-2-2



#include "SoundEditPane.h"
#include "SoundNameEdit.h"
#include "ControlPositioners.h"
#include "InternalStateData.h"
#include "SleepingReporter.h"
#include "Envelope.h"
#include "IntAdjuster.h"
#include "MyOptionMenu.h"
#include "CheckBox.h"
#include "ContextMenu.h"


#include <assert.h>



class ContextMenu;




SoundEditPane::SoundEditPane(CRect &_size, CFrame &_frame, ExternalStateData_v2 *_ExternalState, InternalStateData *_InternalState, SleepingReporter **_sleepingreporter, ContextMenu *_contextmenu)
	:	frame(_frame),
		contextmenu(_contextmenu),
		size(_size),
		ExternalState(_ExternalState),
		InternalState(_InternalState),
		sleepingreporter(_sleepingreporter)
{
	CRect pos;

	for(int i=0; i<NumSounds; i++)
	{
		GetSoundPaneNameEditPos(pos, size, i);

		SoundName[i] = new SoundNameEdit(pos, this, kFirstSoundNameEdit + i, sleepingreporter[kSelSoundChange], i, true, i == ExternalState->SelectedSound, contextmenu);
		SoundName[i]->setText(ExternalState->Sound[i].Name);
		frame.addView(SoundName[i]);
	}

	for(i=0; i<NumEnvelopes; i++)
	{
		GetSoundPaneEnvelopePos(pos, size, i);
		envelope[i] = new Envelope(pos, this, kFirstEnvelope + i, ExternalState, sleepingreporter, (EnvelopeEnum) i, contextmenu, HIDEditPaneLevel+i);
		frame.addView(envelope[i]);
	}

	GetSEPLevelAdjusterPos(pos, size);
	LevelAdjuster = new IntAdjuster(pos, this, kSEPLevelAdjuster, 0, 100, "vol", NULL, "%", "%", 60, IATNormal, contextmenu, HIDEditPaneLevel, -1, true);
	LevelAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Level);
	LevelAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(LevelAdjuster);

	GetSEPPanAdjusterPos(pos, size);
	PanAdjuster = new IntAdjuster(pos, this, kSEPPanAdjuster, -100, 100, "pan", NULL, "", "", 60, IATNormal, contextmenu, HIDEditPanePan, -1, true);
	PanAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Pan);
	PanAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(PanAdjuster);

	GetSEPPanEnvelopeRangeAdjusterPos(pos, size);
	PanEnvelopeRangeAdjuster = new IntAdjuster(pos, this, kSEPPanEnvelopeRangeAdjuster, 0, 100, "env range", NULL, "%", "%", 50, IATNormal, contextmenu, HIDEditPanePan, -1, true);
	PanEnvelopeRangeAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].PanEnvelopeRange);
	PanEnvelopeRangeAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(PanEnvelopeRangeAdjuster);
	
	GetSEPCoarsePitchAdjusterPos(pos, size);
	CoarsePitchAdjuster = new IntAdjuster(pos, this, kSEPCoarsePitchAdjuster, 0, MaxPitch, "pitch", NULL, "","",60, IATPitch, contextmenu, HIDEditPanePitch, -1, true);
	CoarsePitchAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].CoarsePitch);
	CoarsePitchAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(CoarsePitchAdjuster);

	GetSEPFinePitchAdjusterPos(pos, size);
	FinePitchAdjuster = new IntAdjuster(pos, this, kSEPFinePitchAdjuster, -50, 50, "fine tune",NULL, " cents","cents",60, IATNormal, contextmenu, HIDEditPanePitch, -1, true);
	FinePitchAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FinePitch);
	FinePitchAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(FinePitchAdjuster);

	GetSEPPitchEnvelopeRangeAdjusterPos(pos, size);
	PitchEnvelopeRangeAdjuster = new IntAdjuster(pos, this, kSEPPitchEnvelopeRangeAdjuster, 0, MaxPitchEnvelopeRange, "env range", NULL, " s/tones", " s/tone", 54, IATPitchEnvelopeRange, contextmenu, HIDEditPanePitch, -1, true);
	frame.addView(PitchEnvelopeRangeAdjuster);
	PitchEnvelopeRangeAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].PitchEnvelopeRange);
	PitchEnvelopeRangeAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);

	GetSEPFilterWidthAdjusterPos(pos, size);
	FilterWidthAdjuster = new IntAdjuster(pos, this, kSEPFilterWidthAdjuster, 0, 100, "filter", "width", "%", "%", 60, IATNormal, contextmenu, HIDEditPaneFilter, -1, true);
	FilterWidthAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FilterWidth);
	FilterWidthAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(FilterWidthAdjuster);

	GetSEPOscillatorLevelAdjusterPos(pos, size);
	OscillatorLevelAdjuster = new IntAdjuster(pos, this, kSEPOscillatorLevelAdjuster, 0, 100, "osc level", NULL, "%", "%", 50, IATNormal, contextmenu, HIDEditPaneOscillator, -1, true);
	OscillatorLevelAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Oscillator);
	OscillatorLevelAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(OscillatorLevelAdjuster);

	GetSEPLengthAdjusterPos(pos, size);
	LengthAdjuster = new IntAdjuster(pos, this, kSEPLengthAdjuster, 0, MaxLength, "length", NULL, " beats", " beat", 70, IATLength, contextmenu, HIDEditPaneLength, -1, true);
	LengthAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Length);
	LengthAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(LengthAdjuster);

	GetSEPFeedBackAdjusterPos(pos, size);
	FeedBackAdjuster = new IntAdjuster(pos, this, kSEPFeedBackAdjuster, 0, 99, "feedback", NULL, "%", "%", 50, IATNormal, contextmenu, HIDEditPaneFeedback, -1, true);
	FeedBackAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FeedBack);
	FeedBackAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(FeedBackAdjuster);

	GetSEPCombAdjusterPos( pos, size );
	CombAdjuster = new IntAdjuster(pos, this, kSEPCombAdjuster, 0, 99, "comb", NULL, "%", "%", 40, IATNormal, contextmenu, HIDEditPaneComb, -1, true);
	CombAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Comb);
	CombAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(CombAdjuster);

	GetSEPCompressionAdjusterPos(pos, size);
	CompressionAdjuster = new IntAdjuster(pos, this, kSEPCompressionAdjuster, 0, 99, "compression", NULL, "%", "%", 40, IATNormal, contextmenu, HIDEditPaneCompression, -1, true);
	CompressionAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Compression);
	CompressionAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(CompressionAdjuster);

	for(i=0; i<2; i++)
	{
		GetSEPOscMenuPos(pos, size, i);
		OscMenu[i] = new MyOptionMenu(pos, this, kSEPOscType1+i, "", true, (1-i)*50, contextmenu, HIDEditPaneOscillator, -1, true);
		OscMenu[i]->addEntry("SINE", oscSine);
		OscMenu[i]->addEntry("SQUARE", oscSquare);
		OscMenu[i]->addEntry("SAW", oscSaw);
		OscMenu[i]->addEntry("NOISE", oscNoise);
		OscMenu[i]->addEntry("LOAD .WAV", oscWAV);
		OscMenu[i]->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
		UpdateOscName(i);
		frame.addView(OscMenu[i]);
	}

	GetSepOsc2PhaseInvertPos(pos, size);
	Osc2PhaseInvert = new CheckBox(pos, this, kSEPOsc2PhaseInvert, "invert ", contextmenu, HIDEditPaneOscillator, true);
	Osc2PhaseInvert->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Osc2PhaseInvert);
	Osc2PhaseInvert->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	frame.addView(Osc2PhaseInvert);
}





SoundEditPane::~SoundEditPane()
{
	for(int i=0; i<NumSounds; i++)
	{
		frame.removeView(SoundName[i]);
		delete SoundName[i];
	}

	for(i=0; i<NumEnvelopes; i++)
	{
		frame.removeView(envelope[i]);
		delete envelope[i];
	}

	frame.removeView(LevelAdjuster);
	delete LevelAdjuster;

	frame.removeView(PanAdjuster);
	delete PanAdjuster;

	frame.removeView(PanEnvelopeRangeAdjuster);
	delete PanEnvelopeRangeAdjuster;
	
	frame.removeView(CoarsePitchAdjuster);
	delete CoarsePitchAdjuster;

	frame.removeView(FinePitchAdjuster);
	delete FinePitchAdjuster;

	frame.removeView(PitchEnvelopeRangeAdjuster);
	delete PitchEnvelopeRangeAdjuster;

	frame.removeView(FilterWidthAdjuster);
	delete FilterWidthAdjuster;

	frame.removeView(OscillatorLevelAdjuster);
	delete OscillatorLevelAdjuster;

	frame.removeView(LengthAdjuster);
	delete LengthAdjuster;

	frame.removeView(FeedBackAdjuster);
	delete FeedBackAdjuster;

	frame.removeView(CombAdjuster);
	delete CombAdjuster;

	frame.removeView(CompressionAdjuster);
	delete CompressionAdjuster;

	for(i=0; i<2; i++)
	{
		frame.removeView(OscMenu[i]);
		delete OscMenu[i];
	}

	frame.removeView(Osc2PhaseInvert);
	delete Osc2PhaseInvert;
}






void SoundEditPane::Resize(CRect &newsize)
{
	size = newsize;

	CRect pos;

	for(int i=0; i<NumSounds; i++)
	{
		GetSoundPaneNameEditPos(pos, size, i);

		SoundName[i]->Resize(pos);
	}

	for(i=0; i<NumEnvelopes; i++)
	{
		GetSoundPaneEnvelopePos(pos, size, i);
		
		envelope[i]->Resize(pos);
	}

	GetSEPLevelAdjusterPos(pos, size);
	LevelAdjuster->Resize(pos);

	GetSEPPanAdjusterPos(pos, size);
	PanAdjuster->Resize(pos);

	GetSEPFilterWidthAdjusterPos(pos, size);
	FilterWidthAdjuster->Resize(pos);

	GetSEPOscillatorLevelAdjusterPos(pos, size);
	OscillatorLevelAdjuster->Resize(pos);

	GetSEPLengthAdjusterPos(pos, size);
	LengthAdjuster->Resize(pos);

	GetSEPFeedBackAdjusterPos(pos, size);
	FeedBackAdjuster->Resize(pos);

	GetSEPCombAdjusterPos( pos, size );
	CombAdjuster->Resize( pos );

	GetSEPCompressionAdjusterPos(pos, size);
	CompressionAdjuster->Resize(pos);

	GetSEPCoarsePitchAdjusterPos(pos, size);
	CoarsePitchAdjuster->Resize(pos);

	GetSEPFinePitchAdjusterPos(pos, size);
	FinePitchAdjuster->Resize(pos);

	GetSEPPanEnvelopeRangeAdjusterPos(pos, size);
	PanEnvelopeRangeAdjuster->Resize(pos);

	GetSEPPitchEnvelopeRangeAdjusterPos(pos, size);
	PitchEnvelopeRangeAdjuster->Resize(pos);

	for(i=0; i<2; i++)
	{
		GetSEPOscMenuPos(pos, size, i);
		OscMenu[i]->Resize(pos);
	}

	GetSepOsc2PhaseInvertPos(pos, size);
	Osc2PhaseInvert->Resize(pos);
}






void SoundEditPane::valueChanged(CDrawContext *context, CControl *control)
{
	static char buf[100];
	
	int tag = control->getTag();

	switch(tag)
	{
		case kSEPLevelAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].Level = LevelAdjuster->MyGetValue();
			sleepingreporter[kLevelChange]->SetInt(ExternalState->SelectedSound);
			envelope[envLevel]->redraw();
			break;

		case kSEPPanAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].Pan = PanAdjuster->MyGetValue();
			sleepingreporter[kPanChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPCoarsePitchAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].CoarsePitch = CoarsePitchAdjuster->MyGetValue();
			sleepingreporter[kPitchChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPFinePitchAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].FinePitch = FinePitchAdjuster->MyGetValue();
			break;

		case kSEPFilterWidthAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].FilterWidth = FilterWidthAdjuster->MyGetValue();
			sleepingreporter[kFilterWidthChange]->SetInt(ExternalState->SelectedSound);
			envelope[envFilterWidth]->redraw();

			break;

		case kSEPOscillatorLevelAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].Oscillator = OscillatorLevelAdjuster->MyGetValue();
			sleepingreporter[kOscillatorLevelChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPLengthAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].Length = LengthAdjuster->MyGetValue();
			sleepingreporter[kLengthChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPFeedBackAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].FeedBack = FeedBackAdjuster->MyGetValue();
			sleepingreporter[kFeedBackChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPCombAdjuster:
			ExternalState->Sound[ExternalState->SelectedSound].Comb = CombAdjuster->MyGetValue();
			sleepingreporter[kCombChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPCompressionAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].Compression = CompressionAdjuster->MyGetValue();
			sleepingreporter[kCompressionChange]->SetInt(ExternalState->SelectedSound);
			break;

		case kSEPPanEnvelopeRangeAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].PanEnvelopeRange = PanEnvelopeRangeAdjuster->MyGetValue();
			break;

		case kSEPPitchEnvelopeRangeAdjuster:

			ExternalState->Sound[ExternalState->SelectedSound].PitchEnvelopeRange = PitchEnvelopeRangeAdjuster->MyGetValue();
			envelope[envPitch]->redraw();
			break;

		case kSEPOscType1:
		case kSEPOscType2:

			if((OscillatorType)OscMenu[tag-kSEPOscType1]->getCurrent() == oscWAV)
			{
				if(InternalState->Sound[ExternalState->SelectedSound].wav[tag-kSEPOscType1].PickFile(ExternalState->Sound[ExternalState->SelectedSound].WavFileName[tag-kSEPOscType1]))
				{
					if(!InternalState->Sound[ExternalState->SelectedSound].wav[tag-kSEPOscType1].Load(ExternalState->Sound[ExternalState->SelectedSound].WavFileName[tag-kSEPOscType1]))
					{
						break;	//bypass updating shit
					}
				}
				else
				{
					break;	//bypass updating shit
				}
			}

			ExternalState->Sound[ExternalState->SelectedSound].Osc[tag-kSEPOscType1] = (OscillatorType)OscMenu[tag-kSEPOscType1]->getCurrent();

			UpdateOscName(tag-kSEPOscType1);

			break;

		case kSEPOsc2PhaseInvert:

			ExternalState->Sound[ExternalState->SelectedSound].Osc2PhaseInvert = Osc2PhaseInvert->MyGetValue();
			break;

		default:	//assume its a sound name edit

			assert((tag >= kFirstSoundNameEdit) && (tag <= kLastSoundNameEdit));
			
			((CTextEdit *)control)->getText(buf);
			buf[MaxSoundNameLength-1] = 0;
			strcpy(ExternalState->Sound[ExternalState->SelectedSound].Name, buf);

			sleepingreporter[kFirstSoundNameChange+ExternalState->SelectedSound]->InformListener();

			return;		//bypass control->redraw()
	}

	control->redraw();
}




void SoundEditPane::AllValuesChanged()
{
	for(int i=0; i<NumSounds; i++)
	{
		SoundName[i]->setText(ExternalState->Sound[i].Name);
		SoundName[i]->SetHighlight(i == ExternalState->SelectedSound);
		SoundName[i]->redraw();
	}

	for(i=0; i<NumEnvelopes; i++)
	{
		envelope[i]->ValueChanged();
	}

	LevelAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Level);
	LevelAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	LevelAdjuster->redraw();

	PanAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Pan);
	PanAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	PanAdjuster->redraw();

	FilterWidthAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FilterWidth);
	FilterWidthAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	FilterWidthAdjuster->redraw();
	
	OscillatorLevelAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Oscillator);
	OscillatorLevelAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	OscillatorLevelAdjuster->redraw();
	
	LengthAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Length);
	LengthAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	LengthAdjuster->redraw();

	FeedBackAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FeedBack);
	FeedBackAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	FeedBackAdjuster->redraw();

	CombAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Comb);
	CombAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	CombAdjuster->redraw();

	CompressionAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Compression);
	CompressionAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	CompressionAdjuster->redraw();

	CoarsePitchAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].CoarsePitch);
	CoarsePitchAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	CoarsePitchAdjuster->redraw();
	
	FinePitchAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FinePitch);
	FinePitchAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	FinePitchAdjuster->redraw();
	
	PanEnvelopeRangeAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].PanEnvelopeRange);
	PanEnvelopeRangeAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	PanEnvelopeRangeAdjuster->redraw();
	
	PitchEnvelopeRangeAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].PitchEnvelopeRange);
	PitchEnvelopeRangeAdjuster->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	PitchEnvelopeRangeAdjuster->redraw();

	for(i=0; i<2; i++)
	{
		UpdateOscName(i);
		OscMenu[i]->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
		OscMenu[i]->redraw();
	}			
	
	Osc2PhaseInvert->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Osc2PhaseInvert);
	Osc2PhaseInvert->SetColor(SaturatedSoundCol[ExternalState->SelectedSound], DarkSoundCol[ExternalState->SelectedSound], LightSoundCol[ExternalState->SelectedSound]);
	Osc2PhaseInvert->redraw();
}





void SoundEditPane::ValueChanged(int SleepingReporterNum)
{
	int i;

	switch(SleepingReporterNum)
	{
		case kSelSoundChange:

			AllValuesChanged(); 
			break;

		case kLevelChange:

			LevelAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Level);
			LevelAdjuster->redraw();
			envelope[envLevel]->redraw();
			break;
			
		case kCombChange:

			CombAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Comb);
			CombAdjuster->redraw();
			break;

		case kCompressionChange:

			CompressionAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Compression);
			CompressionAdjuster->redraw();
			break;	

		case kPanChange:

			PanAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Pan);
			PanAdjuster->redraw();
			break;	

		case kPanEnvChange:

			PanEnvelopeRangeAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].PanEnvelopeRange);
			PanEnvelopeRangeAdjuster->redraw();
			break;

		case kPitchChange:

			CoarsePitchAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].CoarsePitch);
			CoarsePitchAdjuster->redraw();
			break;

		case kPitchEnvChange:

			PitchEnvelopeRangeAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].PitchEnvelopeRange);
			PitchEnvelopeRangeAdjuster->redraw();
			envelope[envPitch]->redraw();
			break;

		case kFilterWidthChange:

			FilterWidthAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FilterWidth);
			FilterWidthAdjuster->redraw();
			envelope[envFilterWidth]->redraw();
			break;	

		case kOscillatorLevelChange:

			OscillatorLevelAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Oscillator);
			OscillatorLevelAdjuster->redraw();
			break;	

		case kLengthChange:

			LengthAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].Length);
			LengthAdjuster->redraw();
			break;

		case kFeedBackChange:

			FeedBackAdjuster->MySetValue(ExternalState->Sound[ExternalState->SelectedSound].FeedBack);
			FeedBackAdjuster->redraw();
			break;

		default:		//assume its a SoundNameEdit change

			i = SleepingReporterNum - kFirstSoundNameChange;

			assert((i >= 0) && (i < NumSounds));

			SoundName[i]->setText(ExternalState->Sound[i].Name);
			SoundName[i]->redraw();

			break;
	}
}




void SoundEditPane::SetMenuFromFileName(MyOptionMenu *menu, char *FileName)
{
	char buffer[MAX_PATH];

	strcpy(buffer, FileName);

	buffer[strlen(FileName)-4] = 0;

	char *c = buffer + strlen(buffer);

	while((c > buffer) && (*(c-1) != '\\'))
	{
		c--;
	}

	if(strlen(c) > MaxOscNameLength)
	{
		c[MaxOscNameLength] = 0;
	}

	menu->SetText(c);
}





void SoundEditPane::UpdateOscName(int i)
{
	MyOptionMenu *menu = OscMenu[i];
	
	SoundData_v2 *Sound = &(ExternalState->Sound[ExternalState->SelectedSound]);

	switch(Sound->Osc[i])
	{
		case oscSine:	menu->SetText("SINE");		break;	
		case oscSquare:	menu->SetText("SQUARE");	break;	
		case oscSaw:	menu->SetText("SAW");		break;	
		case oscNoise:	menu->SetText("NOISE");		break;	

		case oscWAV:	
	
			SetMenuFromFileName(menu, Sound->WavFileName[i]);		
			
			break;
	}
}
