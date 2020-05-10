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


//Filename:		MYHELPERS.H
//Author:		Leighton Hargreaves
//Date:			30/01/2001
//Description:	Contains an anthology of handy little functions


#ifndef _MYHELPERS_H_ /* GATE. */
#define _MYHELPERS_H_



#include <math.h>
#include "vstgui.h"




const float PI = 3.1415927f;
const float TwoPI = PI*2;

#define NULL 0


//this is now in the project settings
//#define EVALUATIONVERSION





//Name          : fsin/fcos/ftan/fasin/facos/fatan/fsqrt/fpow
//Author        : Leighton Hargreaves
//Description   : floating point casts for maths
inline float fsin(const float v)					{	return (float) sin(v);		}
inline float fcos(const float v)					{	return (float) cos(v);		}
inline float ftan(const float v)					{	return (float) tan(v);		}
inline float fasin(const float v)					{	return (float) asin(v);		}
inline float facos(const float v)					{	return (float) acos(v);		}
inline float fatan(const float v)					{	return (float) atan(v);		}
inline float fsqrt(const float v)					{	return (float) sqrt(v);		}
inline float fpow(float const a, float b)			{	return (float) pow(a, b);	}







inline int MIN(const int a, const int b)			{return (a<b) ? a : b;}
inline int MAX(const int a, const int b)			{return (a>b) ? a : b;}
inline int ABS(const int a)							{return (a>0) ? a : -a;}
inline float MIN(const float a, const float b)		{return (a<b) ? a : b;}
inline float MAX(const float a, const float b)		{return (a>b) ? a : b;}
inline float ABS(const float a)						{return (a>0) ? a : -a;}
inline double ABS(const double a)					{return (a>0) ? a : -a;}

inline bool IsWithin(CRect rect, CPoint point)		{return ((point.h >= rect.left) && (point.v >= rect.top) && (point.h < rect.right) && (point.v < rect.bottom));}

inline void UnDenormalise(float &val)
{
	const int x = *reinterpret_cast <const int *> (&val);
	const int abs_mantissa = x & 0x007FFFFF;
	const int biased_exponent = x & 0x7F800000;

	if( (biased_exponent == 0) && (abs_mantissa != 0) )
	{
		val = 0;
	}
}


//Name          : SafeDiv
//Author        : Leighton Hargreaves
//Description   : zero-protected division
//Parameters    : numerator, denominator
//Return value  : result
inline float SafeDiv(const float num, const float den){return(ABS(den) < 0.0001f)?0:num/den;}




//Name          : RandFloat
//Author        : Leighton Hargreaves
//Description   : generates a random float in range 0..1
//Parameters    : none
//Return value  : the random float
inline float RandFloat()						{return (float)(rand()%0xFFFF) / (float)0xFFFF;}




void itoa(int i, char *a);





void DrawArrow(CDrawContext *context, CPoint &Where, int Xvector, int Yvector, const CColor &Colour, int HeadSize);

void DrawThickLine(CDrawContext *context, CPoint &p1, CPoint &p2, int width, const CColor &Colour);



void LoadOnlineSupport(HWND hWnd);



inline void GetApplicationDirectory(char *FileName)
{
	bool bFound = false;

	HKEY key1, key2, key3, key4, key5; 
	
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM", 0, KEY_QUERY_VALUE, &key1) == ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(key1, "CurrentControlSet", 0, KEY_QUERY_VALUE, &key2) == ERROR_SUCCESS)
		{
			if(RegOpenKeyEx(key2, "Control", 0, KEY_QUERY_VALUE, &key3) == ERROR_SUCCESS)
			{
				if(RegOpenKeyEx(key3, "Session Manager", 0, KEY_QUERY_VALUE, &key4) == ERROR_SUCCESS)
				{
					if(RegOpenKeyEx(key4, "SFC", 0, KEY_QUERY_VALUE, &key5) == ERROR_SUCCESS)
					{
						strcpy(FileName, "");

						DWORD nPathLen = MAX_PATH;
						DWORD nType;

						if(RegQueryValueEx(key5, "ProgramFilesDir", NULL, &nType, (LPBYTE) FileName, &nPathLen) == ERROR_SUCCESS)
						{
							bFound = true;
						}

						RegCloseKey(key5);
					}

					RegCloseKey(key4);
				}

				RegCloseKey(key3);
			}

			RegCloseKey(key2);
		}

		RegCloseKey(key1);
	}

	if(!bFound)
	{
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software", 0, KEY_QUERY_VALUE, &key1) == ERROR_SUCCESS)
		{
			if(RegOpenKeyEx(key1, "Microsoft", 0, KEY_QUERY_VALUE, &key2) == ERROR_SUCCESS)
			{
				if(RegOpenKeyEx(key2, "Windows", 0, KEY_QUERY_VALUE, &key3) == ERROR_SUCCESS)
				{
					if(RegOpenKeyEx(key3, "Currentversion", 0, KEY_QUERY_VALUE, &key4) == ERROR_SUCCESS)
					{
						strcpy(FileName, "");

						DWORD nPathLen = MAX_PATH;
						DWORD nType;

						if(RegQueryValueEx(key4, "ProgramFilesDir", NULL, &nType, (LPBYTE) FileName, &nPathLen) == ERROR_SUCCESS)
						{
							bFound = true;
						}

						RegCloseKey(key4);
					}

					RegCloseKey(key3);
				}

				RegCloseKey(key2);
			}

			RegCloseKey(key1);
		}
	}

	if(!bFound)
	{
		strcpy(FileName, "c:\\Program Files");
	}
}






#endif /* GATE */


//End of file