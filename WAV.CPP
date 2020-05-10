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


//Wav.cpp		ANARCHY RHYTHMS

//Leighton Hargreaves	21-2-02




#include "Wav.h"
#include "MyConsts.h"
#include "MyHelpers.h"

#include <stdio.h>



Wav::Wav()
{
	bLoaded = false;

	for(int i=0; i<2; i++)
	{
		data[i]=NULL;
	}
}


Wav::~Wav()
{
	for(int i=0; i<2; i++)		//delete data
	{
		if(data[i])
		{
			delete data[i];
		}
	}
}




bool Wav::PickFile(char *FileName)	//filles FileName with picked file
{
	OPENFILENAME ofn;

    ofn.lStructSize 		=sizeof(OPENFILENAME);
    ofn.hwndOwner 			=NULL;
    ofn.hInstance 			=NULL;
    ofn.lpstrFilter 		="*.wav";
    ofn.lpstrCustomFilter 	=NULL;     
    ofn.nMaxCustFilter 		=0;
    ofn.nFilterIndex 		=0;
    ofn.lpstrFile 			=FileName;
    ofn.nMaxFile 			=MAX_PATH;
    ofn.lpstrFileTitle 		=NULL;
    ofn.nMaxFileTitle 		=0;
    ofn.lpstrInitialDir 	=NULL;
    ofn.lpstrTitle 			="LOAD .WAV";
    ofn.Flags				=0; 
    ofn.nFileOffset			=0; 
    ofn.nFileExtension		=0; 
    ofn.lpstrDefExt			="WAV"; 
    ofn.lCustData			=0; 
    ofn.lpfnHook			=NULL; 
    ofn.lpTemplateName		=0; 

	if(GetOpenFileName(&ofn))
	{
		return true;
	}
	else
	{
		return false;
	}
}




bool Wav::Load(char *FileName)	
{
	FILE *f = fopen(FileName, "rb");

	while(!f)
	{
		char msg[MAX_PATH+100];

		strcpy(msg, "Can't find ");
		strcat(msg, FileName);
		strcat(msg, "\nDo you want to locate it?");

		if(MessageBox(NULL, msg, "AnarchyRhythms: LOAD WAV", MB_YESNO) == IDNO)
		{
			return false;
		}
		else
		{
			if(!PickFile(FileName))
			{
				return false;
			}
			else
			{
				f = fopen(FileName, "rb");
			}
		}
	}

	char buffer[25];
	int i = 0;
	int length;
	int freq;
	int bits=0;
	float BitRateDivisor;

	unsigned int correctsymbols;
	const char *cmpstr = "data";

	char *msg = NULL;

	fread(buffer, 1, 12, f);          //check RIFF header

	bool ret = false;

	if(memcmp(buffer, "RIFF", 4) || memcmp(buffer+8, "WAVE", 4))
	{
		msg = "oo la la!\nInvalid .WAV File!";

		goto getout;
	}

	fread(buffer, 1, 4, f);

	if(memcmp(buffer, "fmt ", 4)) 
	{
		msg = "oo la la!\nInvalid .WAV File!";

		goto getout;
	}

	fread(&length, 1, sizeof(int), f);

	fread(&i, 1, sizeof(short), f);            //should be 1 for PCM data

	length -= 2;
	
	if (i != 1) 
	{
		msg = "oo la la!\nCan't load compressed .WAV Files!";
		goto getout;
	}

	fread(&WavChannels, 1, sizeof(short), f);     //mono or stereo data

	length -= 2;

	if((WavChannels!= 1) && (WavChannels != 2))
	{
		msg = "oo la la!\n.WAV file has an invalid number of channels!";
		goto getout;
	}

	fread(&freq, 1, sizeof(int), f);
	length -= 4;

	fread(buffer, 6, 1, f);		//skip six bytes
	length -= 6;

	fread(&bits, 1, sizeof(short), f);
	length -= 2;

	if ((bits != 8) && (bits != 16))
	{
		msg = "oo la la!\n.WAV file has an invalid number of bits!";
		goto getout;
	}

	//search for data chunk header

	correctsymbols=0;

	for(i=0; (i<10240) && (correctsymbols<strlen(cmpstr)); i++)		//might as well search 10240 times as any other
	{
		char c;

		fread(&c, 1, 1, f);
		length--;

		if(c==cmpstr[correctsymbols])
		{
			correctsymbols++;
		}
		else
		{
			correctsymbols=0;
		}
	}

	if(correctsymbols!=strlen(cmpstr))
	{
		msg = "oo la la!\nCan't find data segment tag!";
		goto getout;
	}

	fread(&length, 1, 4, f);          //check Data chunk header

	Length = MIN(length*8 / (WavChannels*bits), MaxWavLength);

	for(i=0; i<2; i++)		//delete previous data
	{
		if(data[i])
		{
			delete data[i];
			data[i] = NULL;
		}
	}

	for(i=0; i<WavChannels; i++)		//allocate new  data
	{
		data[i] = new float[Length];
	}

	BitRateDivisor = (float) (1 << (bits-1));

	for(i=0; i<Length; i++)
	{
		for(int chn=0; chn < WavChannels; chn++)
		{
			if(bits==8)
			{
				unsigned char val;
				fread(&val, 1, 1, f);
				data[chn][i] = MAX(-1.f, MIN(1.f, (float)val / BitRateDivisor -1.f));
			}
			else
			{
				short val;
				fread(&val, 1, 2, f);
				data[chn][i] = MAX(-1.f, MIN(1.f, (float)val / BitRateDivisor));
			}
		}
	}

	bLoaded = true;

	ret = true;

	getout:		//somink musta fucked up if we jumped here

	if(msg)
	{
		MessageBox(NULL, msg, "AnarchyRhythms: LOAD WAV", MB_OK);
	}

	fclose(f);

	return ret;
}
