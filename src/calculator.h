#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "stack.h"

int precedence(char op);
long int evaluateInt(long int left, long int right, char op);
double evaluateFloat(double left, double right, char op);
long int countInt(char* str);
double countFloat(char* str);
void isValidCharset(char* str);

#endif // CALCULATOR_H
