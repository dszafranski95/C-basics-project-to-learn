// calculator.h
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdbool.h>

/**
 * Parses and computes a simple binary expression of the form:
 *     number operator number
 *
 * @param expr        The input string, e.g. "12.3+4.5"
 * @param out_result  Pointer to a double where the result will be stored
 * @return            true if parsing & computation succeeded, false otherwise
 */
bool calc_compute(const char *expr, double *out_result);

#endif // CALCULATOR_H
