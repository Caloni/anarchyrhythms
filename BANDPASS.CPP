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


//BandPass.cpp  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2


#include "BandPass.h"
#include "MyHelpers.h"




BandPass::BandPass()
{
	Reset();
}



void BandPass::Reset()
{
	for(int sound=0; sound<NumSounds; sound++)
	{
		LastHiCut[sound] = LastLoCut[sound] = -1;		//forces coefficients to recalculate

		for(int chn=0; chn<NumOutputs; chn++)
		{
			for(int depth = 0; depth < Depth; depth++)
			{
				IH0[chn][sound][depth] = 0;
				IH1[chn][sound][depth] = 0;
				IH2[chn][sound][depth] = 0;
				OH0[chn][sound][depth] = 0;
				OH1[chn][sound][depth] = 0;
				OH2[chn][sound][depth] = 0;
			}
		}
	}
}




void BandPass::Filter(float *value, int &sound, float &HiCut, float &LoCut, int &NumInputs)
{
	//update the coefficients if needed

	if((HiCut!=LastHiCut[sound]) || (LoCut!=LastLoCut[sound]))
	{
		IC0[sound] =		-HiCut / ( (1+LoCut) * (1+HiCut) );

		IC2[sound] =		-IC0[sound];

		OC1[sound] =	((1+LoCut)*(1-HiCut) + (1-LoCut)*(1+HiCut)) /
						((1+LoCut)*(1+HiCut));

		OC2[sound] =	-((1-LoCut)*(1-HiCut)) / ((1+LoCut)*(1+HiCut));

		LastHiCut[sound] = HiCut;
		LastLoCut[sound] = LoCut;
	}
	
	for(int i=0; i<Depth; i++)
	{
		ApplyFilter(value, sound, i, NumInputs);
	}
}




inline void BandPass::ApplyFilter(float *value, int &sound, int &i, int &NumInputs)
{
	IH2[0][sound][i] = IH1[0][sound][i];
	IH1[0][sound][i] = IH0[0][sound][i];
	IH0[0][sound][i] = value[0];

	OH2[0][sound][i] = OH1[0][sound][i];
	OH1[0][sound][i] = OH0[0][sound][i];
	OH0[0][sound][i] = IH0[0][sound][i]*IC0[sound] + IH2[0][sound][i]*IC2[sound] + OH1[0][sound][i]*OC1[sound] + OH2[0][sound][i]*OC2[sound];

	UnDenormalise(OH0[0][sound][i]);

	value[0] = OH0[0][sound][i];

	if(NumInputs==2)
	{
		IH2[1][sound][i] = IH1[1][sound][i];
		IH1[1][sound][i] = IH0[1][sound][i];
		IH0[1][sound][i] = value[1];

		OH2[1][sound][i] = OH1[1][sound][i];
		OH1[1][sound][i] = OH0[1][sound][i];
		OH0[1][sound][i] = IH0[1][sound][i]*IC0[sound] + IH2[1][sound][i]*IC2[sound] + OH1[1][sound][i]*OC1[sound] + OH2[1][sound][i]*OC2[sound];

		UnDenormalise(OH0[1][sound][i]);

		value[1] = OH0[1][sound][i];
	}
}
