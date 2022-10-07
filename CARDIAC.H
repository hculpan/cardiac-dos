#ifndef _CARDIAC_H_
#define _CARDIAC_H_

#include "util.h"

#define MEM_SIZE 100

enum memstatustype {
     none,
     reading,
     writing
};

typedef enum memstatustype memstatus;

typedef int (*input_func)();
typedef void (*output_func)(int);

void initCardiac();

bool evaluateOp(int op, input_func ifunc, output_func ofunc);
bool nextOp(input_func ifunc, output_func ofunc);

int getMemory(int addr);
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