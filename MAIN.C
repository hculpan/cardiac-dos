#include "gui.h"
#include "cardiac.h"
#include "cardtest.h"

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


// comment/uncomment to execute unit tests
// #define _RUN_TESTS_

int pause = 1500;

int updateDelay(int change) {
  pause += change;
  if (pause > 10000) pause = 10000;
  else if (pause < 0) pause = 0;

  return pause;
}


int main(int argc, char *argv[]) {
  int running, loop = TRUE;
  clock_t prevClock, currClock;

  initCardiac();

#ifndef_RUN_TESTS_
  if (argc > 1 && strcmpi(argv[1], "assemble") == 0) {
    if (argc != 3) {
      printf("Invalid argument count for ASSEMBLE\n");
      return 1;
    }

    clrscr();
    assembleFile(argv[2]);
    return 0;
  } else if (argc > 1 && strcmpi(argv[1], "run") == 0) {
    int addr, value;
    FILE *f;
    char filename[256];

    if (argc != 3) {
      printf("Invalid argument count for RUN\n");
      return 1;
    }

    strcpy(filename, argv[2]);
    strcat(filename, ".CBF");

    f = fopen(filename, "r");
    if (!f) {
      printf("No such file: %s\n", filename);
      return 1;
    }
    while (fscanf(f, "%d:%d", &addr, &value) == 2) {
      setMemory(addr, value);
    }
    fclose(f);
  } else if (argc != 1) {
    printf("Invalid command line\n");
    return 1;
  }

  if (initGui(&updateDelay) != true) {
     printf("Unable to initialize GUI\n");
     return 1;
  };

  setEvents(
            guiInputFunc,
            guiOutputFunc,
            guiAccumUpdated,
            guiPcUpdated,
            guiMemoryUpdated);

  drawGui();

  drawVmStatus(running);

  drawDelay(pause);

  prevClock = currClock = clock();
  while (loop) {
    char c;

    if (running && ((currClock - prevClock) / CLK_TCK) * 1000  > pause) {
      prevClock = currClock;
      if (nextOp() != true) {
        running = FALSE;
        drawVmStatus(running);
      }
    }

    currClock = clock();

    if (kbhit()) {
      c = getch();

      if (c == 0) c = getch();

      switch (c) {
        case 27: // ESC
          loop = FALSE;
          break;
        case 63: // F5
          running = TRUE;
          drawVmStatus(running);
          break;
        case 65: // F7
          setProgramCounter(0);
          running = TRUE;
          drawVmStatus(running);
          break;
        case 72: // increase delay
          updateDelay(500);
          drawDelay(pause);
          break;
        case 80: // decrease delay
          updateDelay(-500);
          drawDelay(pause);
          break;
      }
    }
  }

  closeGui();

  return 0;

#else

  clrscr();

  cardiacUnitTests();

  printf("\nPress any key to continue\n");

  getch();

  return 0;
#endif
}