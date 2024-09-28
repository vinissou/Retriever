/***********************************************************
* Name: app.h
*
* Function: 
* Header file for the executable program that calls the 
* functions from the portable library retriever.h.
*
* Creator: Vinícius Souza
* Site:    https://github.com/vinissou
***********************************************************/
#include <stdio.h>
#include <errno.h>

#include "modules/finderC.h"

#define false 0
#define true  1

struct options_status {
    short DEBUG;
    short COUNT;
    short LIMITERS;
    short LINES;
    short OUTPUT;
    short POSITIONS;
    short RANGE;
} options_status;

