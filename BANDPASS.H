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


//BandPass.h  -  ANARCHY RHYTHMS

//Leighton Hargreaves 18-1-2


#include "MyConsts.h"



const int Depth = 2;


class BandPass
{
	public:

		BandPass();

		void Reset();

		void Filter(float *value, int &sound, float &HiCut, float &LoCut, int &NumInputs);

	private:

		inline void ApplyFilter(float *value, int &sound, int &i, int &NumInputs);

		float LastHiCut[NumSounds];
		float LastLoCut[NumSounds];

		float IC0[NumSounds];
		float IC2[NumSounds];
		float OC1[NumSounds];
		float OC2[NumSounds];

		float IH0[NumOutputs][NumSounds][Depth];
		float IH1[NumOutputs][NumSounds][Depth];
		float IH2[NumOutputs][NumSounds][Depth];
		float OH0[NumOutputs][NumSounds][Depth];//could be NumInputs not NumOutputs if NumInputs weren't floating
		float OH1[NumOutputs][NumSounds][Depth];
		float OH2[NumOutputs][NumSounds][Depth];
};