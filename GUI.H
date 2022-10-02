#ifndef _GUI_H_
#define _GUI_H_

#include "util.h"

bool initGui();
void closeGui();

bool drawGui();

extern int screenWidth, screenHeight, numColors;

#endif