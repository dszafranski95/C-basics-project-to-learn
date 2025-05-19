// calculator.c
#include "calculator.h"
#include <stdio.h>

/**
 * Implements calc_compute declared in calculator.h.
 */
bool calc_compute(const char *expr, double *out_result) {
    double a, b;
    char op;

    // Try to parse "<double><char><double>"
    // sscanf returns number of successfully matched items
    if (sscanf(expr, "%lf%c%lf", &a, &op, &b) != 3) {
        return false;  // parsing failed
    }

    // Perform the operation based on op
    switch (op) {
        case '+':
            *out_result = a + b;
            break;
        case '-':
            *out_result = a - b;
            break;
        case '*':
            *out_result = a * b;
            break;
        case '/':
            if (b == 0.0) return false;  // division by zero
            *out_result = a / b;
            break;
        default:
            return false;  // unknown operator
    }

    return true;
}
