#ifndef _GUI_H_
#define _GUI_H_

#include "util.h"

bool initGui();
void closeGui();

bool drawGui();

int guiInputFunc();

void guiOutputFunc(int value);

extern int screenWidth, screenHeight, numColors;

#endif