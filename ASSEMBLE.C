#include "assemble.h"

#include "util.h"

#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define IS_LABEL   -1

const char *keywords[] = {
     "inp",
     "cla",
     "add",
     "tac",
     "sft",
     "out",
     "sto",
     "sub",
     "jmp",
     "hrs",
     "org",
     "data",
     "_end"  // must be last element, marks end of array
};

FILE *output;
int address = 3;
int dataAddress = 90;

struct symbol {
  char *name;
  int  value;
};

struct symbol symbolTable[50];
int symbolCount = 0;

void addNewSymbol(char *name, int value) {
  symbolTable[symbolCount].name = malloc(strlen(name) + 1);
  strcpy(symbolTable[symbolCount].name, name);
  symbolTable[symbolCount].value = value;
  symbolCount++;
}

int findSymbolValue(char *name) {
  int i;
  for (i = 0; i < symbolCount; i++) {
    if (strcmpi(name, symbolTable[i].name) == 0) {
      return symbolTable[i].value;
    }
  }

  return INT_MIN;
}

int findOpcode(char *t) {
  int i = 0;
  while (TRUE) {
    if (strcmpi("_end", keywords[i]) == 0) break;
    if (strcmpi(t, keywords[i]) == 0) return i;
    i++;
  }

  return IS_LABEL;
}

int isCharacter(char c) {
  return (c >= 65 && c<= 90) || (c >= 97 && c<=122);
}

int isDigit(char c) {
  return (c >= 48 && c <= 57);
}

int isValidLabel(char *t) {
  return isCharacter(t[0]);
}

int isNumber(char *t) {
  int i;
  for (i = 0; i < strlen(t); i++) {
    if (!isDigit(t[i])) return FALSE;
  }

  return TRUE;
}

int isValidAddress(char *t) {
  int num;

  if (!isNumber(t)) return FALSE;
  num = atoi(t);
  return (num >= 0 && num <= 99);
}

void assembleInstruction(char *buffer, int line, char *opToken, char *argToken) {
  int opcode = findOpcode(opToken);
  if (opcode < 0) {
    printf("Line %d: Invalid instruction '%s'\n", line, opToken);
    buffer[0] = '\0';
  } else if (opcode == 10) {
    if (!isValidAddress(argToken)) {
      printf("Line %d: Invalid address '%s'\n", line, argToken);
    } else {
      address = atoi(argToken);
    }
    buffer[0] = '\0';
  } else if (opcode == 11) {
    if (!isNumber(argToken)) {
      printf("Line %d: Invalid number '%s'\n", line, argToken);
    } else {
      sprintf(buffer, "%02d:%s", dataAddress, argToken);
    }
  } else {
    int value = INT_MIN;
    if (isNumber(argToken)) {
      value = atoi(argToken);
    } else {
      value = findSymbolValue(argToken);
    }

    if (value != INT_MIN) {
      sprintf(buffer, "%02d:%d%02d", address++, opcode, value);
    } else {
      printf("Line %d: Invalid argument '%s'\n", argToken);
    }
  }
}

void trimr(char *s) {
  int i = 0;
  do {
    if (s[i] == ' ' || s[i] == '\n' || s[i] == '\r' || s[i] == '\t') {
      s[i] = '\0';
    } else {
      i++;
    }
  } while (s[i] != '\0');

}

void assembleLine(int line, char *token[4], int tokenCount) {
  char buffer[100];
  if (tokenCount == 4) {
    if (!isValidLabel(token[0])) {
      printf("Line %d: Invalid label '%s'\n", line, token[0]);
    } else if (findSymbolValue(token[0]) != INT_MIN) {
      printf("Line %d: Label '%s' already defined\n", line, token[0]);
    } else if (!isValidAddress(token[1])) {
      printf("Line %d: Invalid address '%s'\n", line, token[1]);
    }

    dataAddress = atoi(token[1]);
    addNewSymbol(token[0], dataAddress);

    assembleInstruction(buffer, line, token[2], token[3]);
    if (strlen(buffer) > 0) {
      fputs(buffer, output);
      fputs("\n\r", output);
    }
  } else if (tokenCount == 3) {
  } else if (tokenCount == 2) {
    assembleInstruction(buffer, line, token[0], token[1]);
    if (strlen(buffer) > 0) {
      fputs(buffer, output);
      fputs("\n\r", output);
    }
  } else {
    printf("Line %d: Invalid token count\n", line);
  }
}

void assembleFile(char *filename) {
  char inputFilename[256];
  char outputFilename[256];
  char buffer[100];
  char *token[4];
  int tokenCount, line = 0;
  FILE *f;

  if (!filename) {
    printf("No filename given\n");
    return;
  }

  strcpy(inputFilename, filename);
  strcat(inputFilename, ".CAF");
  strcpy(outputFilename, filename);
  strcat(outputFilename, ".CBF");

  f = fopen(inputFilename, "r");
  if (!f) {
    printf("No such file: %s\n", inputFilename);
    return;
  }

  output = fopen(outputFilename, "w");
  if (!output) {
    printf("Unable to open file: %s\n", outputFilename);
    return;
  }

  printf("Reading file: %s\n", inputFilename);
  while (fgets(buffer, 100, f)) {
    line++;
    tokenCount = 0;

    token[0] = strtok(buffer, " ");
    if (token[0] && token[0][0] != 10) {
      for (tokenCount = 1; tokenCount < 4; tokenCount++) {
        token[tokenCount] = strtok(NULL, " ");
        if (!token[tokenCount]) break;
        trimr(token[tokenCount]);
      }
    }

    if (tokenCount) {
      assembleLine(line, token, tokenCount);
    }
  }
  fclose(f);
  fclose(output);

  printf("Done, output saved to %s\n", outputFilename);
}

