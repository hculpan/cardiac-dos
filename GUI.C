#include "gui.h"

#include <graphics.h>

int screenWidth = 0, screenHeight = 0, numColors = 16;

bool drawBorder() {
  setcolor(RED);
  rectangle(1, 1, screenWidth - 1, screenHeight - 1);
  return true;
}

bool drawTitle() {
  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
  moveto(5, 0);
  outtext("CARDIAC v0.1");

  return true;
}

bool drawGui() {
  drawBorder();
  drawTitle();
  return true;
}

bool initGui() {
  int driver = VGA, mode = VGAHI, error;
  char *errptr;

  initgraph(&driver, &mode, ".");
  setbkcolor(CYAN);
  error = graphresult();
  if (error != grOk) {
    errptr = grapherrormsg(error);
    printf("\nError %d\n  %s\n", error, errptr);
    return false;
  }

  screenWidth = getmaxx();
  screenHeight = getmaxy();

  return true;
}

void closeGui() {
  closegraph();
}