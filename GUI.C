#include "gui.h"

#include <graphics.h>

#include "cardiac.h"

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

bool drawMemoryTitle() {
  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(400, 35);
  outtext("MEMORY");

  return true;
}

bool drawMemory(int loc, memstatus ms) {
  int rect[10];
  char txt[8];
  int textColor = BLUE;

  int x = (loc/25) * 70 + 320;
  int y = (loc % 25) * 15 + 70;

  rect[0] = x + 15;
  rect[1] = y;
  rect[2] = x + 42;
  rect[3] = y;
  rect[4] = x + 42;
  rect[5] = y + 13;
  rect[6] = x + 15;
  rect[7] = y + 13;
  rect[8] = x + 15;
  rect[9] = y;

  if (getMemory(loc) < 0) {
    sprintf(txt, "%02d -%03d\0", loc, abs(getMemory(loc)));
  } else {
    sprintf(txt, "%02d  %03d\0", loc, getMemory(loc));
  }


  setcolor(RED);
  if (loc == getProgramCounter()) {
    setfillstyle(SOLID_FILL, BLUE);
    textColor = WHITE;
  } else {
    switch (ms) {
      case reading:
        setfillstyle(SOLID_FILL, GREEN);
        break;
      case writing:
        setfillstyle(SOLID_FILL, RED);
        textColor = WHITE;
        break;
      default:
        setfillstyle(SOLID_FILL, WHITE);
    }
  }

  fillpoly(5, rect);

  setcolor(textColor);
  settextstyle(SMALL_FONT, HORIZ_DIR, 4);
  moveto(x, y);
  outtext(txt);

  return true;
}

bool drawAccumulator() {
  char txt[6];
  int rect[] = {170, 65, 267, 65, 267, 101, 170, 101, 170, 65};

  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(200, 35);
  outtext("ACC");

  setcolor(RED);
  setfillstyle(SOLID_FILL, WHITE);
  fillpoly(5, rect);

  if (getAccumulator() < 0) {
    sprintf(txt, "-%04d\0", abs(getAccumulator()));
  } else {
    sprintf(txt, " %04d\0", getAccumulator());
  }

  setcolor(BLUE);
  moveto(177, 65);
  settextstyle(SMALL_FONT, HORIZ_DIR, 9);
  outtext(txt);

  return true;
}

bool drawInputBox() {
  int rect[] = {170, 365, 267, 365, 267, 401, 170, 401, 170, 365};

  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(200, 335);
  outtext("INP");

  setcolor(RED);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  fillpoly(5, rect);

  return true;
}

bool drawOutputBox() {
  int rect[] = {170, 265, 267, 265, 267, 301, 170, 301, 170, 265};

  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(200, 235);
  outtext("OUT");

  setcolor(RED);
  setfillstyle(SOLID_FILL, WHITE);
  fillpoly(5, rect);

  return true;
}

bool drawProgramCounter() {
  char txt[3];
  int rect[] = {194, 165, 243, 165, 243, 201, 194, 201, 194, 165};

  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(206, 135);
  outtext("PC");

  setcolor(RED);
  setfillstyle(SOLID_FILL, WHITE);
  fillpoly(5, rect);

  sprintf(txt, "%02d\0", getProgramCounter());

  setcolor(BLUE);
  moveto(204, 165);
  settextstyle(SMALL_FONT, HORIZ_DIR, 9);
  outtext(txt);

  return true;
}

bool drawGui() {
  int i;

  drawBorder();
  drawTitle();
  drawAccumulator();
  drawProgramCounter();
  drawOutputBox();
  drawInputBox();

  drawMemoryTitle();
  for (i = 0; i < MEM_SIZE; i++) {
    drawMemory(i, none);
  }

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

int guiInputFunc() {
  return 0;
}

void guiOutputFunc(int value) {
  // outputValue = value;
}

