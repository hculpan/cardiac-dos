#ifndef _CARDIAC_H_
#define _CARDIAC_H_

#include "util.h"

#define MEM_SIZE 100

typedef int (*input_func)(int); // address
typedef void (*output_func)(int, int); // value, address
typedef void (*accum_updated)(int); // value
typedef void (*pc_updated)(int); //value
typedef void (*memory_updated)(int, int); // value, address

void initCardiac();

bool evaluateOp(int op);
bool nextOp();

void setEvents(
     input_func,
     output_func,
     accum_updated,
     pc_updated,
     memory_updated);

int getMemory(int addr);  // doesn't trigger event
int readMemory(int addr); // does trigger event
void setMemory(int addr, int value);

int clearAccumulator();
int getAccumulator();
int setAccumulator(int value);
int addToAccumulator(int value);
int subFromAccumulator(int value);

int getProgramCounter();
void setProgramCounter(int newAddr);
void incrementProgramCounter();

#endif