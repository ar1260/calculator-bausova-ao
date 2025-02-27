#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXP_LEN 1024
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

bool isInRange(long int value)
{
    return (value >= MIN_VALUE && value <= MAX_VALUE);
}

bool isInRangeFloat(double value)
{
    return (value >= MIN_VALUE && value <= MAX_VALUE);
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

void count(char* str, Stack* output, bool floatMode)
{
    Stack* operatorStack = createStack();
    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        if (isdigit(current)) {
            if (floatMode) {
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
            } else {
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
            }
            i--;
        } else if (current == '(') {
            pushInt(operatorStack, current);
        } else if (current == ')') {
            while (!IsEmpty(operatorStack) && topOperator(operatorStack) != '(') {
                char op = popInt(operatorStack);
                if (floatMode) {
                    double right = popFloat(output);
                    double left = popFloat(output);
                    double result = evaluateFloat(left, right, op);
                    pushFloat(output, result);
                } else {
                    long int right = popInt(output);
                    long int left = popInt(output);
                    long int result = evaluateInt(left, right, op);
                    pushInt(output, result);
                }
            }
            popInt(operatorStack); // delete '('
        } else if (current == '+' || current == '-' || current == '*' || current == '/') {
            while (!IsEmpty(operatorStack) && precedence(topOperator(operatorStack)) >= precedence(current)) {
                char op = popInt(operatorStack);
                if (floatMode) {
                    double right = popFloat(output);
                    double left = popFloat(output);
                    double result = evaluateFloat(left, right, op);
                    pushFloat(output, result);
                } else {
                    long int right = popInt(output);
                    long int left = popInt(output);
                    long int result = evaluateInt(left, right, op);
                    pushInt(output, result);
                }
            }
            pushInt(operatorStack, current);
        } else {
            printf("Incorrect input\n");
            exit(-3);
        }
    }

    while (!IsEmpty(operatorStack)) {
        char op = popInt(operatorStack);
        if (floatMode) {
            double right = popFloat(output);
            double left = popFloat(output);
            double result = evaluateFloat(left, right, op);
            pushFloat(output, result);
        } else {
            long int right = popInt(output);
            long int left = popInt(output);
            long int result = evaluateInt(left, right, op);
            pushInt(output, result);
        }
    }

    free(operatorStack);
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

int main(int argc, char* argv[])
{
    bool floatMode = false;

    if (argc > 1 && strcmp(argv[1], "--float") == 0) {
        floatMode = true;
    }

    char str[EXP_LEN + 1];
    while (fgets(str, EXP_LEN + 1, stdin) != NULL) {
        if (strlen(str) > EXP_LEN) {
            printf("Input data exceeds 1 KiB limit\n");
            exit(-1);
        }

        str[strcspn(str, "\n")] = 0;
        isValidCharset(str);
        Stack* output = createStack();
        count(str, output, floatMode);

        if (floatMode) {
            double result = popFloat(output);
            printf("%.4f\n", result);
        } else {
            long int result = popInt(output);
            printf("%ld\n", result);
        }

        freeStack(output);
    }

    return 0;
}
