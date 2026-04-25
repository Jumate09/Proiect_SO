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

int check_per(const char* filepath,int role,int read,int write){
    struct stat st;

    if(stat(filepath,&st)==-1){
        fprintf(stderr,"eroare nu s-a gasit fisierul pentru verificare permisiune");
        return 0;
    }
    int readable;
    int writeable;
    if(role==0){//daca e insp
        if((st.st_mode & S_IRGRP)==0){
            readable=0;
        }
        else{
            readable=1;
        }
        if((st.st_mode & S_IWGRP)==0){
            writeable=0;
        }
        else{
            writeable=1;
        }
    }
    if(role==1){//daca e man
        if((st.st_mode & S_IRUSR)==0){
            readable=0;
        }
        else{
            readable=1;
        }
        if((st.st_mode & S_IWUSR)==0){
            writeable=0;
        }
        else{
            writeable=1;
        }
    }
    if(read==1&&readable==0){
        fprintf(stderr,"per anulata, rolul nu are drept de scriere %s\n",filepath);
        return 0;
    }
    if(write==1&&writeable==0){
        fprintf(stderr,"per anulata, rolul nu are drept de citire %s\n",filepath);
        return 0;
    }
    return 1;
}

void log_command(const char* distr_id,int role,const char* name,const char* command){
    char filepath[256]="";
    strcpy(filepath,distr_id);
    strcat(filepath,"/");
    strcat(filepath,"logged_district");
    int fp=open(filepath,O_WRONLY|O_APPEND);
    if(fp==-1){
        fprintf(stderr,"eroare nu s-au putut deschide loguriile");
        return;
    }
    char entry[512];
    time_t stamp=time(NULL);
    char time_str[64];
    strcpy(time_str,ctime(&stamp));
    time_str[strcspn(time_str,"\n")]=0;
    char role_str[]="INSPECTOR";
    if(role){
        strcpy(role_str,"MANAGER");
    }
    int len=snprintf(entry, sizeof(entry),"%s %s %s %s\n",time_str,role_str,name,command);
    write(fp,entry,len);
    close(fp);
}

void add_report(const char* distr_id,const char* nume,int role,float lat, float lon, const char* issue, int severity, const char* desc){
    char filepath[256];
    snprintf(filepath,sizeof(filepath),"%s/reports.dat",distr_id);
    if(!check_per(filepath,role,0,1)){
        fprintf(stderr,"eroare nu ai permisiune pentru a adauga un raport");
        return;
    }
    int fp=open(filepath,O_WRONLY|O_APPEND|O_CREAT,REPORT_PER);
    if(fp==-1){
        fprintf(stderr,"eroare nu s-a reusit deschiderea fisierului %s",filepath);
        return;
    }
    district_h rep;
    rep.lat=lat;
    rep.lon=lon;
    rep.severity=severity;
    rep.stamp=time(NULL);
    strncpy(rep.insp_name,nume,MAX_NUME-1);
    strncpy(rep.issue,issue,MAX_ISS-1);
    strncpy(rep.description,desc,MAX_DESC-1);

    struct stat st;
    fstat(fp,&st);
    rep.report_id=st.st_size/sizeof(district_h)+1;//calculeza al catale raport este in functie de cate exista deja in fisier
    if(write(fp,&rep,sizeof(district_h))!=sizeof(district_h)){
        fprintf(stderr,"eroare nu s-a scris tot raportul");
    }
    close(fp);
}

