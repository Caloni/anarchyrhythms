/*-----------------------------------------------------------------------------

© 1999, Steinberg Soft und Hardware GmbH, All Rights Reserved

-----------------------------------------------------------------------------*/
#ifndef __ADELAYEDIT_H
#define __ADELAYEDIT_H

#include "BaseDLL.hpp"
#include <string.h>

class BaseDLLEdit : public BaseDLL
{
public:
	BaseDLLEdit(audioMasterCallback audioMaster);
	~BaseDLLEdit();

	virtual void setParameter(long index, float value);
};

#endif
