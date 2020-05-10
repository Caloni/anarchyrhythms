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


//Envelope.h	-	Anarchy Rhythms

//Leighton Hargreaves 6-2-2


#ifndef ENVELOPEHEADERGATE
#define ENVELOPEHEADERGATE



#include "vstgui.h"
#include "ExternalStateData_v2.h"


class SleepingReporter;
class ContextMenu;





class Envelope	:	public CControl
{
	public:

		Envelope(CRect size, CControlListener *listener, int tag, ExternalStateData_v2 *_ExternalState, SleepingReporter **_sleepingreporter, EnvelopeEnum _EnvelopeType, ContextMenu *_contextmenu, int _HelpID);

		void draw(CDrawContext *context);
		void update(CDrawContext *context);
		void mouse(CDrawContext *context, CPoint &where);

		void Resize(CRect &newsize);

		void ValueChanged();		

	private:

		ContextMenu *contextmenu;
		int HelpID;

		CRect ActiveArea;

		EnvelopeEnum EnvelopeType;

		ExternalStateData_v2 *ExternalState;
		SleepingReporter **sleepingreporter;

		EnvelopeData *data;

		int DraggingPoint;		//-1 if no point being dragged

		void GetControlPointPos(CPoint &pos, int CPNum);
		void SetControlPointPos(CPoint &pos);//sets point pointed to by DraggingPoint, modifies Dragging point if needs to re-order

		bool IsPointDeletable();
};







#endif //ENVELOPEHEADERGATE