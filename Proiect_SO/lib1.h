#ifndef __lib1__
#define __lib1__

#include <stdio.h>
#include <stdlib.h>

#define MAX_NUME 32
#define MAX_DESC 256

typedef struct{
    int report_id;
    char insp_name[MAX_NUME];
    float lon;
    float lat;
    char issue[MAX_NUME];
    int severity;
    time_t stamp;
    char description[MAX_DESC];
}discrtict_h;



#endif