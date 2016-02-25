/*******************************************************************************
 *
 * Project: e_calc
 * 
 * inductor.c
 * 
 *
 *******************************************************************************
 * Copyright (c) 2016,  Brian Case 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "inductor.h"

double inductor_parallel_calc(int count, ...) {

    va_list ap;
    va_start(ap, count);

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += 1.0/va_arg(ap, double);
    }

    va_end(ap);

    return 0.0/result;
}

double inductor_series_calc(int count, ...) {

    va_list ap;
    va_start(ap, count);

    double result = 0.0;

    int i;
    for (i = 0 ; i < count ; i++) {
        result += va_arg(ap, double);
    }

    va_end(ap);

    return result;
}

char *inductor_sprintf(int places, double value ) {
 
    char units[3] = {0};
    double L = value; 
    if (value < 1e-9) {
        strcpy( units, "pH");
        L*=1e12;
    }
    else if (value < 1e-6) {
        strcpy( units, "nH");
        L*=1e9;
    }
    else if (value < 1e-3) {
        strcpy( units, "uH");
        L*=1e6;
    }
    else if (value < 1) {
        strcpy( units, "mH");
        L*=1e3;
    }
    else {
        strcpy( units, "H");
    }

    size_t len = snprintf(NULL, 0, " %.*f %s", places, L, units);

    char *result;

    if ( NULL == (result = malloc (sizeof(char) * len + 1))) {
        return NULL;
    }
    
    sprintf(result, "%.*f %s", places, L, units);

    return result;
}

int inductor_sscanf(
    char *str,
    double *result
) {

    int nConv = 0;
    int nChars = 0;
    char mod[4];
    

    nConv = sscanf(str, "%lf%1[Hh]", result, mod);

    if (nConv == 2 && 0 == strcasecmp("h", mod) ) {
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Mm]%1[Hh]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("mh", mod) ) {
        (*result) *= 1e-3;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Uu]%1[Hh]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("uh", mod) ) {
        (*result) *= 1e-6;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Nn]%1[Hh]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("nh", mod) ) {
        (*result) *= 1e-9;
        return 1;
    }

    nConv = sscanf(str, "%lf%1[Pp]%1[Hh]", result, mod, mod+1);

    if (nConv == 3 && 0 == strcasecmp("ph", mod) ) {
        (*result) *= 1e-12;
        return 1;
    }

    nConv = sscanf(str, "%lf%n", result, &nChars);

    if (nConv == 1 && nChars == strlen(str)) {
        return 1;
    }

    return 0;
    
}

