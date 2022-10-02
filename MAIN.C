#include <conio.h>
#include "gui.h"
#include "cardiac.h"
#include "cardtest.h"

// comment/uncomment to execute unit tests
// #define _RUN_TESTS_

int main() {

  initCardiac();

#ifndef_RUN_TESTS_

  if (initGui() != true) {
     printf("Unable to initialize GUI\n");
     return 1;
  };

  drawGui();

  getch();

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