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


//Compression.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2





#include "Compression.h"
#include "MyHelpers.h"



const float MinScaler = 0.05f;



Compression::Compression()
{
	Reset();
}






void Compression::Reset()
{
	for(int i=0; i<NumSounds; i++)
	{
		AvgLevel[i] = 0;
	}
}
	



void Compression::Compress(float *value, int &amountpercent, int &sound, int &NumInputs)
{
	float val;
	float Scaler;

	if(NumInputs==1)
	{
		val = ABS(value[0]);

	}
	else
	{
		val = ABS((value[0]+value[1])*0.5f);
	}

	if(val > AvgLevel[sound])
	{
		AvgLevel[sound] = AvgLevel[sound]*0.99f + val*0.01f;
	}
	else
	{
		AvgLevel[sound] = AvgLevel[sound]*0.999f + val*0.001f;
	}

	Scaler = 1.0f/MAX(MinScaler, fpow(AvgLevel[sound], (float)amountpercent/100.f));

	if(NumInputs==1)
	{
		value[0]*=Scaler;
	}
	else
	{
		value[0]*=Scaler;
		value[1]*=Scaler;
	}
} 