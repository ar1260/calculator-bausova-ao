#include "calculator.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXP_LEN 1024

#ifndef TESTING
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

        if (floatMode) {
            double result = countFloat(str);
            printf("%.4f\n", result);
        } else {
            long int result = countInt(str);
            printf("%ld\n", result);
        }
    }

    return 0;
}
#endif
