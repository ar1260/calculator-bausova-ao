#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* createStack()
{
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (s == NULL) {
        printf("Malloc error\n");
        exit(-4);
    }
    s->top = NULL;
    return s;
}

bool IsEmpty(Stack* s)
{
    return s->top == NULL;
}

void pushInt(Stack* s, long int value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Malloc error\n");
        exit(-4);
    }
    newNode->value.intValue = value;
    newNode->isFloat = false;
    newNode->next = s->top;
    s->top = newNode;
}

void pushFloat(Stack* s, double value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Malloc error\n");
        exit(-4);
    }
    newNode->value.floatValue = value;
    newNode->isFloat = true;
    newNode->next = s->top;
    s->top = newNode;
}

long int popInt(Stack* s)
{
    if (IsEmpty(s)) {
        printf("Incorrect input (operators)\n");
        exit(-3);
    }
    Node* temp = s->top;
    long int value = temp->value.intValue;
    s->top = s->top->next;
    free(temp);
    return value;
}

double popFloat(Stack* s)
{
    if (IsEmpty(s)) {
        printf("Incorrect input (operators)\n");
        exit(-3);
    }
    Node* temp = s->top;
    double value = temp->value.floatValue;
    s->top = s->top->next;
    free(temp);
    return value;
}

char topOperator(Stack* s)
{
    if (IsEmpty(s)) {
        printf("Incorrect input\n");
        exit(-3);
    }
    return (char)s->top->value.intValue;
}

void freeStack(Stack* s)
{
    while (!IsEmpty(s)) {
        if (s->top->isFloat) {
            popFloat(s);
        } else {
            popInt(s);
        }
    }
    free(s);
}

bool isInRange(long int value)
{
    return (value >= MIN_VALUE && value <= MAX_VALUE);
}

bool isInRangeFloat(double value)
{
    return (value >= MIN_VALUE && value <= MAX_VALUE);
}
