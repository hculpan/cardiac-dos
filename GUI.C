#include "gui.h"

#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <limits.h>

int screenWidth = 0, screenHeight = 0, numColors = 16;
update_delay_func updateDelayFunc;

void drawDelay(int value) {
  int rect[] = {30, 325, 107, 325, 107, 341, 30, 341, 30, 325};
  char buff[10];
  float num;

  moveto(44, 302);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
  setcolor(BLUE);
  outtext("DELAY");

  moveto(25, 342);
  settextstyle(SMALL_FONT, HORIZ_DIR, 2);
  setcolor(DARKGRAY);
  outtext("Up/Down arrow to change");

  setcolor(RED);
  setfillstyle(SOLID_FILL, WHITE);
  fillpoly(5, rect);

  moveto(44, 325);
  settextstyle(SMALL_FONT, HORIZ_DIR, 5);
  setcolor(BLUE);
  num = value / 1000.0f;
  sprintf(buff, " %.1f s", num);
  outtext(buff);
}

void drawVmStatus(int vmStatus) {
  int rect[] = {30, 425, 107, 425, 107, 441, 30, 441, 30, 425};

  setcolor(RED);
  if (vmStatus) {
    setfillstyle(SOLID_FILL, GREEN);
  } else {
    setfillstyle(SOLID_FILL, LIGHTGRAY);
  }
  fillpoly(5, rect);

  moveto(44, 425);
  settextstyle(SMALL_FONT, HORIZ_DIR, 5);
  if (vmStatus) {
    setcolor(WHITE);
    outtext("RUNNING");
  } else {
    setcolor(DARKGRAY);
    outtext("HALTED");
    moveto(43, 443);
    settextstyle(SMALL_FONT, HORIZ_DIR, 2);
    outtext("F5 to restart");
    moveto(30, 453);
    settextstyle(SMALL_FONT, HORIZ_DIR, 2);
    outtext("F7 to restart from 00");
  }
}

void drawBorder() {
  setcolor(RED);
  rectangle(1, 1, screenWidth - 1, screenHeight - 1);
}

void drawTitle() {
  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
  moveto(5, 0);
  outtext("CARDIAC v1.0");
}

void drawMemoryTitle() {
  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(400, 35);
  outtext("MEMORY");
}

void drawMemory(int loc) {
  int rect[10];
  char txt[8];
  int textColor = BLUE;
  int value = getMemory(loc);

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

  if (value < 0) {
    sprintf(txt, "%02d -%03d\0", loc, abs(value));
  } else {
    sprintf(txt, "%02d  %03d\0", loc, value);
  }


  if (loc == getProgramCounter()) {
    setfillstyle(SOLID_FILL, BLUE);
    textColor = WHITE;
  } else {
    setfillstyle(SOLID_FILL, WHITE);
  }

  setcolor(RED);
  fillpoly(5, rect);

  setcolor(textColor);
  settextstyle(SMALL_FONT, HORIZ_DIR, 4);
  moveto(x, y);
  outtext(txt);
}

void drawAccumulator() {
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
}

void drawInputBox() {
  int rect[] = {170, 365, 267, 365, 267, 401, 170, 401, 170, 365};

  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(200, 335);
  outtext("INP");

  setcolor(RED);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  fillpoly(5, rect);
}

void drawOutputBox() {
  int rect[] = {170, 265, 267, 265, 267, 301, 170, 301, 170, 265};

  setcolor(BLUE);
  settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
  moveto(200, 235);
  outtext("OUT");

  setcolor(RED);
  setfillstyle(SOLID_FILL, WHITE);
  fillpoly(5, rect);
}

void drawProgramCounter() {
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
}

void drawBootloader() {
  setcolor(BLUE);
  settextstyle(SMALL_FONT, HORIZ_DIR, 4);
  moveto(15, 105);
  outtext("TO ENTER PROGRAM");

  moveto(20, 115);
  outtext("01:002");
  moveto(20, 125);
  outtext("02:800");
  moveto(20, 135);
  outtext("01:0XX");
  moveto(25, 145);
  outtext("XX = program start");
  moveto(25, 155);
  outtext("Increment 0XX each time");

  moveto(15, 185);
  outtext("TO RUN PROGRAM");

  moveto(20, 195);
  outtext("01:8XX");
  moveto(25, 205);
  outtext("XX = program start");
}

bool drawGui() {
  int i;

  drawBorder();
  drawTitle();
  drawAccumulator();
  drawProgramCounter();
  drawOutputBox();
  drawInputBox();
  drawBootloader();

  drawMemoryTitle();
  for (i = 0; i < MEM_SIZE; i++) {
    drawMemory(i);
  }

  return true;
}

bool initGui(update_delay_func updatedelayfunc) {
  int driver = VGA, mode = VGAHI, error;
  char *errptr;

  updateDelayFunc = updatedelayfunc;

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

int guiInputFunc(int loc) {
  int rect[] = {170, 365, 267, 365, 267, 401, 170, 401, 170, 365};
  int rect_loc[] = {120, 365, 174, 365, 174, 401, 120, 401, 120, 365};
  char buff[]= {' ', '\0', '\0', '\0', '\0'};
  char c = '\0';
  int hasNeg = FALSE;
  int result = 0;

  setcolor(BLUE);
  moveto(125, 365);
  settextstyle(SMALL_FONT, HORIZ_DIR, 9);
  sprintf(buff, "%02d:", loc);
  outtext(buff);
  buff[0] = ' ';
  buff[1] = '\0';
  buff[2] = '\0';
  buff[3] = '\0';
  buff[4] = '\0';

  while (c != 13) {
    setcolor(RED);
    setfillstyle(SOLID_FILL, WHITE);
    fillpoly(5, rect);

    setcolor(BLUE);
    moveto(177, 365);
    settextstyle(SMALL_FONT, HORIZ_DIR, 9);
    outtext(buff);

    c = getch();
    if (c == '-' && !hasNeg) {
      buff[0] = '-';
      hasNeg = TRUE;
    } else if (c == '-' && hasNeg) {
      buff[0] = ' ';
      hasNeg = FALSE;
    } else if (c > 47 && c < 58 && strlen(buff) < 4) {
      buff[strlen(buff)] = c;
    } else if (c == 8 && strlen(buff) > 1) {
      buff[strlen(buff) - 1] = '\0';
    } else if (c == 8 && strlen(buff) == 1 && hasNeg) {
      buff[0] = ' ';
      hasNeg = FALSE;
    } else if (c == 27) { // ESC
      return INT_MIN;
    } else if (c == 0) {
      c = getch();
      switch (c) {
        case 80: // down arrow
          if (updateDelayFunc) drawDelay((updateDelayFunc(-500)));
          break;
        case 72: // up arrow
          if (updateDelayFunc) drawDelay((updateDelayFunc(500)));
          break;
      }
    }
  }

  if (strlen(buff) > 0 || (strlen(buff) == 1 && buff[0] != '-')) {
    result = atoi(buff);
  }

  setcolor(RED);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  fillpoly(5, rect);

  // blank out address loc
  setcolor(CYAN);
  setfillstyle(SOLID_FILL, CYAN);
  fillpoly(5, rect_loc);

  return result;
}

void guiOutputFunc(int value, int loc) {
  int rect[] = {170, 265, 267, 265, 267, 301, 170, 301, 170, 265};
  char buff[6];

  setcolor(RED);
  setfillstyle(SOLID_FILL, WHITE);
  fillpoly(5, rect);

  setcolor(BLUE);
  moveto(177, 265);
  settextstyle(SMALL_FONT, HORIZ_DIR, 9);
  if (value < 0) {
    sprintf(buff, " -%03d\0", abs(value));
  } else {
  sprintf(buff, "  %03d\0", value);
  }
  outtext(buff);
}

void guiMemoryUpdated(int value, int addr) {
  drawMemory(addr);
}

void guiAccumUpdated(int value) {
  drawAccumulator();
}

void guiPcUpdated(int value) {
  drawProgramCounter();
}


