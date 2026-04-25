#define _POSIX_C_SOURCE 200809L

#include "lib1.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define ESCALATION_TRESHOLD 2

void init_district(const char* distr_id){
    struct stat buf;
    if(stat(distr_id,&buf)==-1){
        mkdir(distr_id,DIR_PER);
        chmod(distr_id,DIR_PER);
        printf("s-a creat districtul %s",distr_id);
    }
    char filepath[256];
    strcpy(filepath,"");
    strcat(filepath,distr_id);
    strcat(filepath,"/");
    strcat(filepath,"reports.dat");
    int fp;
    if(stat(filepath,&buf)==-1){
        fp=open(filepath,O_CREAT|O_RDWR,REPORT_PER);
        if(fp!=-1){
            fchmod(fp,REPORT_PER);
            close(fp);
        }
    }
    strcpy(filepath,"");
    strcat(filepath,distr_id);
    strcat(filepath,"/");
    strcat(filepath,"district.cfg");
    if(stat(filepath,&buf)==-1){
        fp=open(filepath,O_CREAT|O_RDWR,CONFIG_PER);
        if(fp!=-1){
            fchmod(fp,CONFIG_PER);
            char buf[16];
            int tmp=sprintf(buf,"%d\n",ESCALATION_TRESHOLD);
            write(fp,buf,tmp);
            close(fp);
        }
    }

    strcpy(filepath,"");
    strcat(filepath,distr_id);
    strcat(filepath,"/");
    strcat(filepath,"logged_district");
    if(stat(filepath,&buf)==-1){
        fp=open(filepath,O_CREAT|O_RDWR,LOG_PER);
        if(fp!=-1){
            fchmod(fp,LOG_PER);
            close(fp);
        }
    }
}



