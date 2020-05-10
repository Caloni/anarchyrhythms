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


//MidiMap.cpp - AnarchyRhythms

//6-3-02 - LeightonHargreaves

#include <stdio.h>

#include "MidiMap_v2.h"
#include "MyHelpers.h"
#include "resource\resource.h"


extern void* hInstance;



char *StandardControllerDesc_v2[] = {
	
	"Bank", "Mod", "Breath", "", "Pedal", "Porta", "Data", "Vol", "Bal", "", "Pan", "Exp", 
	"Eff1", "Eff2", "", "", "Gen1", "Gen2", "Gen3", "Gen4", "", "", "", "", "", "", "", "", "",
	"", "", "", "BSel", "Mod", "Breath", "", "Pedal", "Porta", "Data", "Vol", "Bal", "", "Pan",
	"Exp", "Eff1", "Eff2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "Var", "Tmbre", "Rel", "Att", "Bright", "Snd6", "Snd7", "Snd8",
	"Snd9", "Snd10", "", "", "", "", "", "", "", "", "", "", "", "Effs", "Trem", "Chor", "Clst",
	"Phase", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", ""
													};
HWND MidiMapDialog;

HWND ChannelCombo;
HWND ParamCombo[ExternalMIDIMap_v2::numMIDIParams];
HWND SoundParamCombo[NumSounds][ExternalMIDIMap_v2::numMIDISoundParams];


ExternalMIDIMap_v2::ExternalMIDIMap_v2()
{
	MidiMapDialog = NULL;

	Channel = -1;	

	bChanged = false;

	for(int i=0; i<NumSounds; i++)
	{
		for(int j=0; j<numMIDISoundParams; j++)
		{
			SoundParam[i][j] = -1;	
		}
	}

	for(i=0; i<numMIDIParams; i++)
	{
		Param[i] = -1;	
	}
}


ExternalMIDIMap_v2::ExternalMIDIMap_v2(const ExternalMIDIMap_v1 &copy)
{
	MidiMapDialog = NULL;

	Channel = copy.Channel;	

	bChanged = copy.bChanged;

	for(int i=0; i<NumSounds; i++)
	{
		for(int j=0; j<numMIDISoundParams; j++)
		{
			SoundParam[i][j] = -1;	
		}

		for(j=0; j<ExternalMIDIMap_v1::numMIDISoundParams; j++)
		{
			SoundParam[i][j] = copy.SoundParam[i][j];	
		}
	}

	for(i=0; i<numMIDIParams; i++)
	{
		Param[i] = -1;	
	}

	for(i=0; i<ExternalMIDIMap_v1::numMIDIParams; i++)
	{
		Param[i] = copy.Param[i];	
	}
}


ExternalMIDIMap_v2::~ExternalMIDIMap_v2()
{
}


void ExternalMIDIMap_v2::Load(HWND systemWindow)
{
	CloseDialog();

	char FileName[MAX_PATH];
	FileName[0] = 0;

	OPENFILENAME ofn;

    ofn.lStructSize 		=sizeof(OPENFILENAME);
    ofn.hwndOwner 			=NULL;
    ofn.hInstance 			=NULL;
    ofn.lpstrFilter 		="*.arm";
    ofn.lpstrCustomFilter 	=NULL;     
    ofn.nMaxCustFilter 		=0;
    ofn.nFilterIndex 		=0;
    ofn.lpstrFile 			=FileName;
    ofn.nMaxFile 			=MAX_PATH;
    ofn.lpstrFileTitle 		=NULL;
    ofn.nMaxFileTitle 		=0;
    ofn.lpstrInitialDir 	=NULL;
    ofn.lpstrTitle 			="LOAD MIDI MAP";
    ofn.Flags				=OFN_FILEMUSTEXIST; 
    ofn.nFileOffset			=0; 
    ofn.nFileExtension		=0; 
    ofn.lpstrDefExt			="ARM"; 
    ofn.lCustData			=0; 
    ofn.lpfnHook			=NULL; 
    ofn.lpTemplateName		=0; 

	if(GetOpenFileName(&ofn))
	{
		FILE *f = fopen(FileName, "rb");

		if(f)
		{
			if(fread(this, sizeof(ExternalMIDIMap_v2), 1, f) == 1)
			{
				bChanged = true;
				Edit(systemWindow);
			}
			else
			{
				MessageBox(NULL, "Error reading MIDI MAP FILE", "AnarchyRhythms", MB_OK);
			}

			fclose(f);
		}
		else
		{
			MessageBox(NULL, "Error opening MIDI MAP FILE", "AnarchyRhythms", MB_OK);
		}
	}
}



void ExternalMIDIMap_v2::Save()
{
	CloseDialog();

	char FileName[MAX_PATH];
	FileName[0] = 0;

	OPENFILENAME ofn;

    ofn.lStructSize 		=sizeof(OPENFILENAME);
    ofn.hwndOwner 			=NULL;
    ofn.hInstance 			=NULL;
    ofn.lpstrFilter 		="*.arm";
    ofn.lpstrCustomFilter 	=NULL;     
    ofn.nMaxCustFilter 		=0;
    ofn.nFilterIndex 		=0;
    ofn.lpstrFile 			=FileName;
    ofn.nMaxFile 			=MAX_PATH;
    ofn.lpstrFileTitle 		=NULL;
    ofn.nMaxFileTitle 		=0;
    ofn.lpstrInitialDir 	=NULL;
    ofn.lpstrTitle 			="SAVE MIDI MAP";
    ofn.Flags				=OFN_OVERWRITEPROMPT; 
    ofn.nFileOffset			=0; 
    ofn.nFileExtension		=0; 
    ofn.lpstrDefExt			="ARM"; 
    ofn.lCustData			=0; 
    ofn.lpfnHook			=NULL; 
    ofn.lpTemplateName		=0; 

	if(GetSaveFileName(&ofn))
	{
		FILE *f = fopen(FileName, "wb");

		if(f)
		{
			if(!fwrite(this, sizeof(ExternalMIDIMap_v2), 1, f) == 1)
			{
				MessageBox(NULL, "Error saving MIDI MAP FILE", "AnarchyRhythms", MB_OK);
			}

			fclose(f);
		}
		else
		{
			MessageBox(NULL, "Error opening MIDI MAP FILE for writing", "AnarchyRhythms", MB_OK);
		}
	}
}



void AddOptionsToControllerCombo_v2(HWND combo)
{
	for(int i=-1; i<=128; i++)
	{
		char str[10];

		switch(i)
		{
			case -1:	strcpy(str, "-");			break;
			case 0:		strcpy(str, "P-BEND");		break;
			default:	
				
				itoa(i-1, str);
				strcat(str, " ");
				strcat(str, StandardControllerDesc_v2[i-1]);
				break;
		}

		SendMessage(combo, CB_ADDSTRING, i+1, (LONG)(LPSTR) str);
	}
}


void AddOptionsToChannelCombo_v2(HWND combo)
{
	SendMessage(combo, CB_ADDSTRING, 0, (LONG)(LPSTR) "<off>");

	for(int i=1; i<=16; i++)
	{
		char str[3];

		itoa(i, str);			

		SendMessage(combo, CB_ADDSTRING, i, (LONG)(LPSTR) str);
	}
}



void ExternalMIDIMap_v2::ClearAllOptions()
{
	SendMessage(ChannelCombo, CB_SETCURSEL, 0, 0);

	for(int sound = 0; sound<NumSounds; sound++)
	{
		for(int param = 0; param < numMIDISoundParams; param++)
		{
			SendMessage(SoundParamCombo[sound][param], CB_SETCURSEL, 0, 0);		//set to 'none'
		}
	}

	for(int i=0; i<numMIDIParams; i++)
	{
		SendMessage(ParamCombo[i], CB_SETCURSEL, 0, 0);		//set to 'none'
	}
}





BOOL CALLBACK MasterDialogProc_v2(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ExternalMIDIMap_v2 *MIDIMap;

	int i, sound, param;

	int ComboHeight = GetSystemMetrics(SM_CYFULLSCREEN)/2;

    switch (uMsg)
    {
	    case WM_INITDIALOG:

			MIDIMap = (ExternalMIDIMap_v2 *)lParam;

			//create dialog contents

			for(sound = 0; sound<NumSounds; sound++)
			{
				for(param = 0; param < ExternalMIDIMap_v2::numMIDISoundParams; param++)
				{
					SoundParamCombo[sound][param] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
											 74 + sound*151,64+param*26,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

					AddOptionsToControllerCombo_v2(SoundParamCombo[sound][param]);
				}
			}

			ParamCombo[ExternalMIDIMap_v2::kMIDIInput] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 90,358,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			ParamCombo[ExternalMIDIMap_v2::kMIDIDryWet] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 300,358,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			ParamCombo[ExternalMIDIMap_v2::kMIDIOutput] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 530,358,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			ParamCombo[ExternalMIDIMap_v2::kShuffleType] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 90,394,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			ParamCombo[ExternalMIDIMap_v2::kShuffleAmount] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 300,394,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			ParamCombo[ExternalMIDIMap_v2::kCurrentPattern] = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 530,394,80,ComboHeight, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			for(i=0; i<ExternalMIDIMap_v2::numMIDIParams; i++)
			{
				AddOptionsToControllerCombo_v2(ParamCombo[i]);
			}

			ChannelCombo = CreateWindow("COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,
									 150,5,80,300, hwndDlg, NULL, (HINSTANCE)hInstance, NULL);

			AddOptionsToChannelCombo_v2(ChannelCombo);


			//Set dialog contents to initial values

			for(sound = 0; sound<NumSounds; sound++)
			{
				for(param = 0; param < ExternalMIDIMap_v2::numMIDISoundParams; param++)
				{
					SendMessage(SoundParamCombo[sound][param], CB_SETCURSEL, MIDIMap->SoundParam[/*NumSounds-1-*/sound][param]+1, 0);		//set to initial value
				}
			}

			for(i=0; i<ExternalMIDIMap_v2::numMIDIParams; i++)
			{
				SendMessage(ParamCombo[i], CB_SETCURSEL, MIDIMap->Param[i]+1, 0);	//set to initial value	
			}

			SendMessage(ChannelCombo, CB_SETCURSEL, MIDIMap->Channel+1, 0);

			//Put address into the SLEEPINGADDRESSHOLDER

			SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_ADDSTRING, 0, (LONG)(LPSTR)"");
			SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_SETITEMDATA, 0, lParam);//sets address of our ExternalMIDIMap_v2 for WM_COMMAND!
			ShowWindow(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), SW_HIDE);
 
			return(TRUE);


		case WM_COMMAND:

			MIDIMap = (ExternalMIDIMap_v2 *) (void *)SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_GETITEMDATA, 0, 0);

			switch(LOWORD(wParam))
			{
				case OKBUTTON:

					//set MIDIMap to new dialog contents
					
					for(sound = 0; sound<NumSounds; sound++)
					{
						for(param = 0; param < ExternalMIDIMap_v2::numMIDISoundParams; param++)
						{
							MIDIMap->SoundParam[/*NumSounds-1-*/sound][param] = SendMessage(SoundParamCombo[sound][param], CB_GETCURSEL, 0, 0)-1;
						}
					}

					for(i=0; i<ExternalMIDIMap_v2::numMIDIParams; i++)
					{
						MIDIMap->Param[i] = SendMessage(ParamCombo[i], CB_GETCURSEL, 0, 0) - 1;	
					}

					MIDIMap->Channel = SendMessage(ChannelCombo, CB_GETCURSEL, 0, 0) -1;

					MIDIMap->bChanged = true;
					
					//drop through to close window

				case CANCELBUTTON:
	
					MIDIMap->CloseDialog();
		            break;

				case CLEAR:

					MIDIMap->ClearAllOptions();
					break;
			}

            return(TRUE);


		case WM_CLOSE:

			MIDIMap = (ExternalMIDIMap_v2 *) (void *)SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_GETITEMDATA, 0, 0);

			MIDIMap->CloseDialog();

			return(TRUE);
    }

    return (FALSE);
}
 



void ExternalMIDIMap_v2::Edit(HWND systemWindow, int PopItUpAlreadyParam)
{
	if(!MidiMapDialog)
	{
		MidiMapDialog = CreateDialogParam((HINSTANCE)hInstance, MAKEINTRESOURCE(127), systemWindow, MasterDialogProc_v2, (LONG)(void *)this);
	}

	HICON icon = LoadIcon((HINSTANCE)hInstance, MAKEINTRESOURCE(IDI_ANARCHY));

	if(icon)
		SendMessage(MidiMapDialog, WM_SETICON, ICON_SMALL, (LPARAM)icon);

	if(PopItUpAlreadyParam >= 0)
	{
		if(PopItUpAlreadyParam < numMIDIParams)
		{
			SendMessage(ParamCombo[PopItUpAlreadyParam], CB_SHOWDROPDOWN, TRUE, 0);		
		}
		else
		{
			PopItUpAlreadyParam -= numMIDIParams;

			SendMessage(SoundParamCombo[/*NumSounds-1-*/(PopItUpAlreadyParam/numMIDISoundParams)][PopItUpAlreadyParam%numMIDISoundParams], CB_SHOWDROPDOWN, TRUE, 0);		
		}
	}

	ShowWindow(MidiMapDialog, SW_SHOW);
}




void ExternalMIDIMap_v2::CloseDialog()
{
	if(MidiMapDialog)
	{
		DestroyWindow(MidiMapDialog);
		MidiMapDialog = NULL;
	}
}




void ExternalMIDIMap_v2::CompileInternalMap(InternalMIDIMap *InternalMap)	//called from BaseDLL when bChanged is flagged
{
	InternalMap->Reset();

	for(int param = 0; param < numMIDIParams; param++)
	{
		InternalMap->Map(param, Param[param]);
	}

	for(int sound = 0; sound < NumSounds; sound++)
	{
		for(param = 0; param < numMIDISoundParams; param++)
		{
			InternalMap->Map(numMIDIParams + sound*numMIDISoundParams + param, SoundParam[sound][param]);
		}
	}

	bChanged = false;
}
