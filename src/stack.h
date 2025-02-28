#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define MIN_VALUE -2000000000
#define MAX_VALUE 2000000000

typedef struct Node {
    union {
        long int intValue;
        double floatValue;
    } value;
    bool isFloat;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
} Stack;

Stack* createStack();
bool IsEmpty(Stack* s);
void pushInt(Stack* s, long int value);
void pushFloat(Stack* s, double value);
long int popInt(Stack* s);
double popFloat(Stack* s);
char topOperator(Stack* s);
void freeStack(Stack* s);
bool isInRange(long int value);
bool isInRangeFloat(double value);

#endif // STACK_H
