#ifndef _GUI_H_
#define _GUI_H_

#include "cardiac.h"
#include "util.h"

typedef int (*update_delay_func)(int);

bool initGui(update_delay_func);
void closeGui();

bool drawGui();

void drawVmStatus(int vmStatus);

int guiInputFunc();
void guiOutputFunc(int value, int loc);
void guiMemoryUpdated(int value, int addr);
void guiAccumUpdated(int value);
void guiPcUpdated(int value);

void drawDelay(int value);

extern int screenWidth, screenHeight, numColors;

#endif