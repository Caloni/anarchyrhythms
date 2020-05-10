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


//About.cpp	AnarchyRhythms
//L8n





#include "About.h"
#include "resource\resource.h"
#include "MyHelpers.h"

extern void* hInstance;

INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	HDC dc;

	About *about;

	RECT SerialRect;

    switch (uMsg)
    {
	    case WM_INITDIALOG:

			//set the sleeping address holder
			SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_ADDSTRING, 0, (LONG)(LPSTR)"");
			SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_SETITEMDATA, 0, lParam);//sets address of HELP for WM_COMMAND!
			ShowWindow(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), SW_HIDE);

			return(TRUE);


		case WM_COMMAND:

			about = (About *) (void *)SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_GETITEMDATA, 0, 0);

			switch(LOWORD(wParam))
			{
				case CLOSEBUTTON:

					about->Close();

					break;
			}

            return(TRUE);


		case WM_CLOSE:

			about = (About *) (void *)SendMessage(GetDlgItem(hwndDlg, IDC_SLEEPINGADDRESSHOLDER), CB_GETITEMDATA, 0, 0);
			about->Close();

			return TRUE;

		case WM_PAINT:

			dc = BeginPaint(hwndDlg, &ps);

			SetBkColor(dc, GetSysColor( COLOR_BTNFACE) );

			GetClientRect(hwndDlg, &SerialRect);			

			SerialRect.top = 163;
			SerialRect.bottom = SerialRect.top + 16;

			SelectObject(dc, (HFONT)GetStockObject(ANSI_VAR_FONT));

			#ifndef EVALUATIONVERSION

				DrawText(dc, "Registered serial number: xxxx-xxxx-xxxx-xxxx", -1, &SerialRect, DT_CENTER);

			#endif

			EndPaint(hwndDlg, &ps);

			return TRUE;

    }

    return (FALSE);
}







About::About()
{
	AboutDlg = NULL;
}



void About::Open(HWND systemWindow)
{
	if(AboutDlg)
	{
		Close();
	}

	AboutDlg = CreateDialogParam((HINSTANCE)hInstance, MAKEINTRESOURCE(125), systemWindow, AboutDialogProc, (LONG)(void *)this);
	
	HICON icon = LoadIcon((HINSTANCE)hInstance, MAKEINTRESOURCE(IDI_ANARCHY));

	if(icon)
		SendMessage(AboutDlg, WM_SETICON, ICON_SMALL, (LPARAM)icon);

	ShowWindow(AboutDlg, SW_SHOW);
}



void About::Close()
{
	if(AboutDlg)
	{
		DestroyWindow(AboutDlg);
		AboutDlg = NULL;
	}
}