#ifndef __lib1__
#define __lib1__

#include <time.h>

#define MAX_NUME 24
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



#define DIR_PER 0750//rwxr-x---
#define REPORT_PER 0664//rw-rw-r--
#define CONFIG_PER 0640//rw-r-----
#define LOG_PER 0644//rw-r--r--

void init_district(const char* );
int check_per(const char* ,int ,int ,int );
void log_command(const char* ,int ,const char* ,const char* );
void add_report(const char* ,const char* ,int ,float , float , const char* , int , const char* );
void creare_simblink(const char* );
void list_reports(const char* , int );
void view_report(const char* , int , int );
void remove_report(const char* , int , int );
void update_threshold(const char* , int , int );
void filter_reports(const char* , int , int , char** );
void remove_district(const char* , int );

#endif