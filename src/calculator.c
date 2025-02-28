#include "calculator.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int precedence(char op)
{
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

long int evaluateInt(long int left, long int right, char op)
{
    long int result = 0;
    switch (op) {
    case '+':
        result = left + right;
        break;
    case '-':
        result = left - right;
        break;
    case '*':
        result = left * right;
        break;
    case '/':
        if (right == 0) {
            printf("Division by zero is forbidden\n");
            exit(-1);
        }
        result = left / right;
        break;
    default:
        printf("Unknown operator: \n");
        exit(-2);
    }

    if (!isInRange(result)) {
        printf("Intermediate result out of range\n");
        exit(-1);
    }

    return result;
}

double evaluateFloat(double left, double right, char op)
{
    double result = 0;
    switch (op) {
    case '+':
        result = left + right;
        break;
    case '-':
        result = left - right;
        break;
    case '*':
        result = left * right;
        break;
    case '/':
        if (fabs(right) < 1e-4) {
            printf("Division by a number less than 10^-4 is forbidden\n");
            exit(-1);
        }
        result = left / right;
        break;
    default:
        printf("Unknown operator\n");
        exit(-2);
    }

    if (!isInRangeFloat(result)) {
        printf("Intermediate result out of range\n");
        exit(-1);
    }

    return result;
}

long int countInt(char* str)
{
    Stack* output = createStack();
    Stack* operatorStack = createStack();

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        if (isdigit(current)) {
            long int num = 0;
            while (isdigit(current)) {
                num = num * 10 + (current - '0');
                current = str[++i];
            }

            if (!isInRange(num)) {
                printf("Input number out of range\n");
                exit(-1);
            }
            pushInt(output, num);
            i--;
        } else if (current == '(') {
            pushInt(operatorStack, current);
        } else if (current == ')') {
            while (!IsEmpty(operatorStack) && topOperator(operatorStack) != '(') {
                char op = popInt(operatorStack);
                long int right = popInt(output);
                long int left = popInt(output);
                long int result = evaluateInt(left, right, op);
                pushInt(output, result);
            }
            popInt(operatorStack); // Удаляем '('
        } else if (current == '+' || current == '-' || current == '*' || current == '/') {
            while (!IsEmpty(operatorStack) && precedence(topOperator(operatorStack)) >= precedence(current)) {
                char op = popInt(operatorStack);
                long int right = popInt(output);
                long int left = popInt(output);
                long int result = evaluateInt(left, right, op);
                pushInt(output, result);
            }
            pushInt(operatorStack, current);
        }
    }

    while (!IsEmpty(operatorStack)) {
        char op = popInt(operatorStack);
        long int right = popInt(output);
        long int left = popInt(output);
        long int result = evaluateInt(left, right, op);
        pushInt(output, result);
    }

    long int finalResult = popInt(output);
    freeStack(output);
    freeStack(operatorStack);
    return finalResult;
}

double countFloat(char* str)
{
    Stack* output = createStack();
    Stack* operatorStack = createStack();

    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        if (isdigit(current)) {
            double num = 0;
            while (isdigit(current)) {
                num = num * 10 + (current - '0');
                current = str[++i];
            }
            if (current == '.') {
                double fraction = 0.1;
                current = str[++i];
                while (isdigit(current)) {
                    num += (current - '0') * fraction;
                    fraction *= 0.1;
                    current = str[++i];
                }
            }

            if (!isInRangeFloat(num)) {
                printf("Input number out of range: %f\n", num);
                exit(-1);
            }
            pushFloat(output, num);
            i--;
        } else if (current == '(') {
            pushInt(operatorStack, current);
        } else if (current == ')') {
            while (!IsEmpty(operatorStack) && topOperator(operatorStack) != '(') {
                char op = popInt(operatorStack);
                double right = popFloat(output);
                double left = popFloat(output);
                double result = evaluateFloat(left, right, op);
                pushFloat(output, result);
            }
            popInt(operatorStack); // Удаляем '('
        } else if (current == '+' || current == '-' || current == '*' || current == '/') {
            while (!IsEmpty(operatorStack) && precedence(topOperator(operatorStack)) >= precedence(current)) {
                char op = popInt(operatorStack);
                double right = popFloat(output);
                double left = popFloat(output);
                double result = evaluateFloat(left, right, op);
                pushFloat(output, result);
            }
            pushInt(operatorStack, current);
        }
    }

    while (!IsEmpty(operatorStack)) {
        char op = popInt(operatorStack);
        double right = popFloat(output);
        double left = popFloat(output);
        double result = evaluateFloat(left, right, op);
        pushFloat(output, result);
    }

    double finalResult = popFloat(output);
    finalResult = round(finalResult * 10000) / 10000;
    freeStack(output);
    freeStack(operatorStack);
    return finalResult;
}

void isValidCharset(char* str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (!isdigit(c) && c != '(' && c != ')' && c != '*' && c != '+' && c != '/' && c != '-' && !isspace(c)) {
            printf("Incorrect input\n");
            exit(-3);
        }
    }
    return;
}
