#include "cardtest.h"

#include <stdio.h>

#include "util.h"

int outputValue = 0;

bool accumulatorUnitTests(bool err) {
  bool isError = err;

  if (getAccumulator() != 0) {
    printf("** Accumulator init: Expected 0, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(100);
  if (getAccumulator() != 100) {
    printf("** Accumulator set: Expected 100, got %d\n", getAccumulator());
    isError = true;
  }

  clearAccumulator(0);
  if (getAccumulator() != 0) {
    printf("** Accumulator clear: Expected 0, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(9999);
  if (getAccumulator() != 9999) {
    printf("** Accumulator set 4-digit: Expected 9999, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(-9999);
  if (getAccumulator() != -9999) {
    printf("** Accumulator set negative 4-digit: Expected -9999, got %d\n", getAccumulator());
    isError = true;
  }

  clearAccumulator();
  setAccumulator(10000);
  if (getAccumulator() != 0) {
    printf("** Accumulator set 5-digit: Expected 0, got %d\n", getAccumulator());
    isError = true;
  }

  clearAccumulator();
  setAccumulator(-10000);
  if (getAccumulator() != 0) {
    printf("** Accumulator set negative 5-digit: Expected 0, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(999);
  addToAccumulator(100);
  if (getAccumulator() != 1099) {
    printf("** Accumulator add: Expected 1099, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(9999);
  addToAccumulator(100);
  if (getAccumulator() != 99) {
    printf("** Accumulator add over: Expected 99, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(-999);
  subFromAccumulator(100);
  if (getAccumulator() != -1099) {
    printf("** Accumulator sub: Expected -1099, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(-9999);
  subFromAccumulator(100);
  if (getAccumulator() != -99) {
    printf("** Accumulator sub over: Expected -99, got %d\n", getAccumulator());
    isError = true;
  }

  if (isError == false) {
    printf("All accumulator tests have passed\n");
  }

  return isError;
}

bool memoryUnitTests(bool err) {
  bool isError = err;
  int i;

  setMemory(0, 100);
  if (getMemory(0) != 1) {
    printf("ROM 00: Expected 1, got %d\n", getMemory(0));
    isError = true;
  }

  setMemory(99, 199);
  if (getMemory(99) != 899) {
    printf("ROM 99: Expected 899, got %d\n", getMemory(99));
    isError = true;
  }

  for (i = 1; i < 99; i++) {
    setMemory(i, i+100);
    if (getMemory(i) != i + 100) {
      printf("Error: Expected %d, got %d at addres %d\n", i+100, getMemory(i), i);
      isError = true;
    }
  }

  if (isError == false) {
    printf("All memory tests have passed\n");
  }

  return isError;
}

int inputFunc() {
  return 10;
}

void outputFunc(int value) {
  outputValue = value;
}


bool instructionTests(bool err) {
  bool isError = err;

  initCardiac();

  evaluateOp(10, &inputFunc, &outputFunc);
  if (getMemory(10) != 10) {
    printf("** Instruction INP: Expected 10, got %d\n", getMemory(10));
    isError = true;
  }

  evaluateOp(110, &inputFunc, &outputFunc);
  if (getAccumulator() != 10) {
    printf("** Instruction CLA: Expected 10, got %d\n", getAccumulator());
    isError = true;
  }

  evaluateOp(210, &inputFunc, &outputFunc);
  if (getAccumulator() != 20) {
    printf("** Instruction ADD: Expected 20, got %d\n", getAccumulator());
    isError = true;
  }

  evaluateOp(399, &inputFunc, &outputFunc);
  if (getProgramCounter() != 0) {
    printf("** Instruction TAC no jump: Expected 0, got %d\n", getProgramCounter());
    isError = true;
  }

  setAccumulator(-10);
  evaluateOp(399, &inputFunc, &outputFunc);
  if (getProgramCounter() != 99) {
    printf("** Instruction TAC jump: Expected 99, got %d\n", getProgramCounter());
    isError = true;
  }

  setAccumulator(10);
  evaluateOp(410, &inputFunc, &outputFunc);
  if (getAccumulator() != 100) {
    printf("** Instruction SFT left 1, right 0: Expected 100, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(10);
  evaluateOp(422, &inputFunc, &outputFunc);
  if (getAccumulator() != 10) {
    printf("** Instruction SFT left 2, right 2: Expected 10, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(10);
  evaluateOp(401, &inputFunc, &outputFunc);
  if (getAccumulator() != 1) {
    printf("** Instruction SFT left 0, right 1: Expected 1, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(10);
  evaluateOp(480, &inputFunc, &outputFunc);
  if (getAccumulator() != 0) {
    printf("** Instruction SFT left 8, right 0: Expected 0, got %d\n", getAccumulator());
    isError = true;
  }

  setAccumulator(10);
  evaluateOp(408, &inputFunc, &outputFunc);
  if (getAccumulator() != 0) {
    printf("** Instruction SFT left 0, right 8: Expected 0, got %d\n", getAccumulator());
    isError = true;
  }

  evaluateOp(510, &inputFunc, &outputFunc);
  if (outputValue != 10) {
    printf("** Instruction OUT: Expected 10, got %d\n", outputValue);
    isError = true;
  }

  setAccumulator(-10);
  evaluateOp(611, &inputFunc, &outputFunc);
  if (getMemory(11) != -10) {
    printf("** Instruction STO: Expected -10, got %d\n", getMemory(11));
    isError = true;
  }

  setAccumulator(1010);
  evaluateOp(611, &inputFunc, &outputFunc);
  if (getMemory(11) != 10) {
    printf("** Instruction STO oversized: Expected 10, got %d\n", getMemory(11));
    isError = true;
  }

  setAccumulator(0);
  evaluateOp(711, &inputFunc, &outputFunc);
  if (getAccumulator() != -10) {
    printf("** Instruction SUB: Expected -10, got %d\n", getAccumulator());
    isError = true;
  }

  setProgramCounter(50);
  evaluateOp(870, &inputFunc, &outputFunc);
  if (getProgramCounter() != 70) {
    printf("** Instruction JMP: Expected 70, got %d\n", getProgramCounter());
    isError = true;
  }
  if (getMemory(99) != 850) {
    printf("** Instruction JMP: Expected 850 at memory 99, got %d\n", getMemory(99));
    isError = true;
  }

  setProgramCounter(50);
  evaluateOp(970, &inputFunc, &outputFunc);
  if (getProgramCounter() != 70) {
    printf("** Instruction HRS: Expected 70, got %d\n", getProgramCounter());
    isError = true;
  }

  if (isError == false) {
    printf("All instruction tests passed\n");
  }

  return isError;
}

void cardiacUnitTests() {
  bool isError = false;

  printf("Executing unit tests...\n\n");


  isError = memoryUnitTests(isError);
  isError = accumulatorUnitTests(isError);
  isError = instructionTests(isError);

  if (isError == true) {
    printf("\nUnit tests failed!\n");
  } else {
    printf("\nAll unit tests have passed!\n");
  }
}