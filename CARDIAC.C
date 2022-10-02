#include "cardiac.h"

#include <math.h>

#define MEM_SIZE 100

int memory[MEM_SIZE];
int accumulator = 0, PC = 0;

void initCardiac() {
  int i;

  memory[0] = 1;
  for (i = 1; i < MEM_SIZE; i++) {
    setMemory(i, 0);
  }
}

int getMemory(int addr) {
  if (addr < 0 || addr > MEM_SIZE - 1) {
    return 0;
  }

  return memory[addr];
}

void setMemory(int addr, int value) {
  if (addr < 1 || addr > MEM_SIZE - 1) {
    return;
  } else if (addr == 99) {
    value = 800 + (value % 100);
  }

  memory[addr] = value % 1000;
}

int getAccumulator() {
  return accumulator;
}

int setAccumulator(int value) {
  accumulator = value % 10000;

  return accumulator;
}

int clearAccumulator() {
  return setAccumulator(0);
}

int addToAccumulator(int value) {
  return setAccumulator(accumulator + value);
}

int subFromAccumulator(int value) {
  return setAccumulator(accumulator - value);
}

int getProgramCounter() {
  return PC;
}

void setProgramCounter(int newAddr) {
  if (newAddr < 0 || newAddr > MEM_SIZE - 1) {
   return;
  }

  PC = newAddr;
}

void incrementProgramCounter() {
  setProgramCounter(PC + 1);
}

int getCurrentOp() {
  return getMemory(PC++);
}

bool evaluateOp(int op,input_func ifunc, output_func ofunc) {
  int opcode = op / 100;
  int arg = op % 100;
  int value;

  switch (opcode) {
    case 0: // INP
      value = (ifunc)();
      setMemory(arg, value);
      break;
    case 1: // CLA
      clearAccumulator();
      addToAccumulator(getMemory(arg));
      break;
    case 2: // ADD
      addToAccumulator(getMemory(arg));
      break;
    case 3: // TAC
      value = getAccumulator();
      if (value < 0) {
        setProgramCounter(arg);
      }
      break;
    case 4: // SFT
      if (arg/10 > 3) {
        clearAccumulator();
      } else {
        value = pow10(arg/10);
        setAccumulator(getAccumulator() * value);
      }
      value = pow10(arg % 10);
      setAccumulator(getAccumulator()/value);
      break;
    case 5: // OUT
      (ofunc)(getMemory(arg));
      break;
    case 6: // STO
      setMemory(arg, getAccumulator());
      break;
    case 7: // SUB
      subFromAccumulator(getMemory(arg));
      break;
    case 8: // JMP
      memory[99] = 800 + getProgramCounter();
      setProgramCounter(arg);
    case 9: // HRS
      setProgramCounter(arg);
      return false;
    default:
      return false;
  }

  return true;
}

bool nextOp(input_func ifunc, output_func ofunc) {
  return evaluateOp(getCurrentOp(), ifunc, ofunc);
}