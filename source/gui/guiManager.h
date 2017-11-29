#ifndef SRC_GUIMGR_H_
#define SRC_GUIMGR_H_



#include "guiBase.h"


int rendertask () ;
void setActiveWindows(guiBase* pWin);
void popActiveWindows();
int refresh(void);

#endif
