// xPL Linux Hal Server

#include <stdio.h>
#include "xPLLib/xPL.h"

int main(int argc, String argv[])
{
    int argIndex = 0;
    xPL_ServicePtr theService = NULL;
    xPL_MessagePtr theMessage = NULL;
    String delimPtr;

    /* Start xPL up */
    if (!xPL_initialize(xPL_getParsedConnectionType())) {
        fprintf(stderr, "Unable to start xPL");
        exit(1);
    }

    printf("Hello World!");
    return 0;
}
