#ifndef __lib1__
#define __lib1__

#include <time.h>

#define MAX_NUME 50
#define MAX_ISS 32
#define MAX_DESC 256

typedef struct{
    int report_id;
    char insp_name[MAX_NUME];
    float lon;
    float lat;
    char issue[MAX_ISS];
    int severity;
    time_t stamp;
    char description[MAX_DESC];
}district_h;

void init_district(const char* );

#define DIR_PER 0750//rwxr-x---
#define REPORT_PER 0664//rw-rw-r--
#define CONFIG_PER 0640//rw-r-----
#define LOG_PER 0644//rw-r--r--


#endif