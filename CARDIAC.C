#include "cardiac.h"

#include <math.h>
#include <limits.h>

int memory[MEM_SIZE];
int accumulator = 0, PC = 0;

input_func inputFunc = NULL;
output_func outputFunc = NULL;
accum_updated accumUpdated = NULL;
pc_updated pcUpdated = NULL;
memory_updated memoryUpdated = NULL;

void setEvents(
     input_func ifunc,
     output_func ofunc,
     accum_updated afunc,
     pc_updated pfunc,
     memory_updated mfunc) {
  inputFunc = ifunc;
  outputFunc = ofunc;
  accumUpdated = afunc;
  pcUpdated = pfunc;
  memoryUpdated = mfunc;
}

void initCardiac() {
  int i;

  memory[0] = 1;
  for (i = 1; i < MEM_SIZE; i++) {
    memory[i] = 0;
  }
  memory[99] = 800;
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

  value %= 1000;

  memory[addr] = value;

  if (memoryUpdated) (memoryUpdated)(value, addr);
}

int getAccumulator() {
  return accumulator;
}

int setAccumulator(int value) {
  accumulator = value % 10000;

  if (accumUpdated) (accumUpdated)(accumulator);

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
  int oldAddr = PC;

  if (newAddr < 0 || newAddr > MEM_SIZE - 1) {
   return;
  }

  PC = newAddr;

  if (memoryUpdated) (memoryUpdated)(getMemory(oldAddr), oldAddr);
  if (memoryUpdated) (memoryUpdated)(getMemory(PC), PC);
  if (pcUpdated) (pcUpdated)(PC);
}

void incrementProgramCounter() {
  setProgramCounter(PC + 1);
}

int getCurrentOp() {
  return memory[PC];
}

bool evaluateOp(int op) {
  int opcode = op / 100;
  int arg = op % 100;
  int value;

  switch (opcode) {
    case 0: // INP
      if (arg != 0) {
        if (inputFunc) {
          value = (inputFunc)(arg);
        } else {
          value = 0;
        }
        if (value == INT_MIN) return false;
        setMemory(arg, value);
      }
      incrementProgramCounter();
      break;
    case 1: // CLA
      clearAccumulator();
      addToAccumulator(getMemory(arg));
      incrementProgramCounter();
      break;
    case 2: // ADD
      addToAccumulator(getMemory(arg));
      incrementProgramCounter();
      break;
    case 3: // TAC
      value = getAccumulator();
      if (value < 0) {
        setProgramCounter(arg);
      } else {
        incrementProgramCounter();
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
      incrementProgramCounter();
      break;
    case 5: // OUT
      if (outputFunc) {
         (outputFunc)(getMemory(arg), arg);
      }
      incrementProgramCounter();
      break;
    case 6: // STO
      setMemory(arg, getAccumulator());
      incrementProgramCounter();
      break;
    case 7: // SUB
      subFromAccumulator(getMemory(arg));
      incrementProgramCounter();
      break;
    case 8: // JMP
      setMemory(99, getProgramCounter());
      setProgramCounter(arg);
      break;
    case 9: // HRS
      setProgramCounter(arg);
      return false;
    default:
      incrementProgramCounter();
      return true;
  }

  return true;
}

bool nextOp() {
  return evaluateOp(getCurrentOp());
}