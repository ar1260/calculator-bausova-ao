#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define EXP_LEN 1024

typedef struct Node {
   int value;
   struct Node* next;
} Node;

typedef struct {
   Node* top;
} Stack;

Stack* createStack() {
   Stack* s = (Stack*)malloc(sizeof(Stack));
   if (s == NULL) {
      printf("Malloc error\n");
      exit(0);
   }
   s->top = NULL;
   return s;
}

bool IsEmpty(Stack* s) {
   return s->top == NULL;
}

void push(Stack* s, int value) {
   Node* newNode = (Node*)malloc(sizeof(Node));
   if (newNode == NULL) {
      printf("Malloc error\n");
      exit(0);
   }
   newNode->value = value;
   newNode->next = s->top;
   s->top = newNode;
}

int pop(Stack* s) {
   if (IsEmpty(s)) {
      printf("Empty stack\n");
      return 0;
   }
   Node* temp = s->top;
   int value = temp->value;
   s->top = s->top->next;
   free(temp);
   return value;
}

int top(Stack* s) {
   if (IsEmpty(s)) {
      printf("Empty stack\n");
      return 0;
   }
   return s->top->value;
}

int precedence(char op) {
   if (op == '+' || op == '-') {
      return 1;
   }
   if (op == '*' || op == '/') {
      return 2;
   }
   return 0;
}

int evaluate(int left, int right, char op) {
   switch (op) {
   case '+':
      return left + right;
   case '-':
      return left - right;
   case '*':
      return left * right;
   case '/':
      return left / right;
   default:
      return 0;
   }
}

void count(char* str, Stack* output) {
   Stack* operatorStack = createStack();
   for (int i = 0; str[i] != '\0'; i++) {
      char current = str[i];

      if (isdigit(current)) {
         int num = 0;
         while (isdigit(current)) {
            num = num * 10 + (current - '0');
            current = str[++i];
         }
         push(output, num);
         i--; //!!!!!!! 
      }
      else if (current == '(') {
         push(operatorStack, current);
      }
      else if (current == ')') {
         while (!IsEmpty(operatorStack) && top(operatorStack) != '(') {
            char op = pop(operatorStack);
            int right = pop(output);
            int left = pop(output);
            int result = evaluate(left, right, op);
            push(output, result);
         }
         pop(operatorStack); // delete '(' 
      }
      else if (current == '+' || current == '-' || current == '*' || current == '/') {
         while (!IsEmpty(operatorStack) && precedence(top(operatorStack)) >= precedence(current)) {
            char op = pop(operatorStack);
            int right = pop(output);
            int left = pop(output);
            int result = evaluate(left, right, op);
            push(output, result);
         }
         push(operatorStack, current);
      }
   }

   while (!IsEmpty(operatorStack)) {
      char op = pop(operatorStack);
      int right = pop(output);
      int left = pop(output);
      int result = evaluate(left, right, op);
      push(output, result);
   }

   free(operatorStack);
}

int evaluatePostfix(Stack* output) {
   if (IsEmpty(output)) {
      printf("empty stack\n");
      return 0;
   }
   return pop(output);
}

void freeStack(Stack* s) {
   while (!IsEmpty(s)) {
      pop(s);
   }
   free(s);
}

int main() {
   char str[EXP_LEN];
   //fgets(str, EXP_LEN, stdin);
   //str[strcspn(str, "\n")] = 0;

   //Stack* output = createStack();
   //count(str, output);

   //int result = evaluatePostfix(output);
   //printf("%d\n", result);

   //freeStack(output);

   while (fgets(str, EXP_LEN, stdin) != NULL) {
      str[strcspn(str, "\n")] = 0; 
      Stack* output = createStack();
      count(str, output);

      int result = evaluatePostfix(output);
      printf("%d\n", result);

      freeStack(output);
   }

   return 0;
}
