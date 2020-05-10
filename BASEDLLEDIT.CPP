/*-----------------------------------------------------------------------------

© 1999, Steinberg Soft und Hardware GmbH, All Rights Reserved

-----------------------------------------------------------------------------*/
#include "BaseDLLEdit.hpp"

#ifndef __ADEditor
#include "Editor.hpp"
#endif

#include <string.h>

extern bool oome;

//-----------------------------------------------------------------------------
BaseDLLEdit::BaseDLLEdit(audioMasterCallback audioMaster)
 : BaseDLL(audioMaster)
{
	editor = new Editor(this);
	if (!editor)
		oome = true;
}

//-----------------------------------------------------------------------------
BaseDLLEdit::~BaseDLLEdit()
{
	// the editor gets deleted by the
	// AudioEffect base class
}

//-----------------------------------------------------------------------------
void BaseDLLEdit::setParameter(long index, float value)
{
	BaseDLL::setParameter(index, value);

	if(editor)
		((AEffGUIEditor*)editor)->setParameter(index, value);
}
