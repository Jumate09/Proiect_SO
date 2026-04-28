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

void creare_simblink(const char* distr_id) {
    char target[256];
    char link_name[256];

    snprintf(target,sizeof(target),"%s/reports.dat",distr_id);
    snprintf(link_name,sizeof(link_name),"active_reports-%s",distr_id);

    if(symlink(target, link_name)==-1) {
        if(errno==EEXIST) {
            // Dacă link-ul există deja, îl ștergem și îl recreăm
            unlink(link_name);
            if(symlink(target, link_name) == -1) {
                fprintf(stderr,"Eroare la actualizarae simblink");
            }
        }
        else{
            fprintf(stderr,"Eroare la creare simblink");
        }
    }
    else{
        printf("Simblink creat:%s->%s\n",link_name,target);
    }
}

void init_district(const char* distr_id){
    struct stat buf;
    if(stat(distr_id,&buf)==-1){
        mkdir(distr_id,DIR_PER);
        chmod(distr_id,DIR_PER);
        printf("s-a creat districtul %s",distr_id);
    }
    char filepath[256];
    snprintf(filepath,sizeof(filepath),"%s/reports.dat",distr_id);
    int fp;
    if(stat(filepath,&buf)==-1){
        fp=open(filepath,O_CREAT|O_RDWR,REPORT_PER);
        if(fp!=-1){
            fchmod(fp,REPORT_PER);
            close(fp);
        }
    }
    snprintf(filepath,sizeof(filepath),"%s/district.cfg",distr_id);
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

    snprintf(filepath,sizeof(filepath),"%s/logged_district",distr_id);
    if(stat(filepath,&buf)==-1){
        fp=open(filepath,O_CREAT|O_RDWR,LOG_PER);
        if(fp!=-1){
            fchmod(fp,LOG_PER);
            close(fp);
        }
    }
    creare_simblink(distr_id);
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
    snprintf(filepath,sizeof(filepath),"%s/logged_district",distr_id);

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
void mode_to_string(mode_t mode, char *str) {
    sprintf(str,"---------");
    if(mode & S_IRUSR){
        str[0] = 'r';
    }
    if(mode & S_IWUSR){
        str[1] = 'w';
    }
    if(mode & S_IXUSR){
        str[2] = 'x';
    }
    if(mode & S_IRGRP){
        str[3] = 'r';
    }
    if(mode & S_IWGRP){
        str[4] = 'w';
    }
    if(mode & S_IXGRP){
        str[5] = 'x';
    }
    if(mode & S_IROTH){
        str[6] = 'r';
    }
    if(mode & S_IWOTH){
        str[7] = 'w';
    }
    if(mode & S_IXOTH){
        str[8] = 'x';
    }
}
void list_reports(const char* distr_id, int role){
    char l_path[256];
    snprintf(l_path,sizeof(l_path),"active_reports-%s",distr_id);
    struct stat st_l, st_t;
    if(lstat(l_path,&st_l)==-1){
        return;
    }
    if(stat(l_path,&st_t)==-1){
        printf("Warning dangling link\n");
        return;
    }
    char p_s[11];
    mode_to_string(st_t.st_mode, p_s);
    printf("Permisiuni: %s | Marime: %ld | Modificat: %s", p_s, st_t.st_size, ctime(&st_t.st_mtime));
    int fd=open(l_path, O_RDONLY);
    if(fd==-1) return;
    district_h r;
    while(read(fd,&r,sizeof(district_h))==sizeof(district_h)){
        printf("ID: %d | User: %s | Issue: %s | Severity: %d | Timestsmp: %s", r.report_id, r.insp_name,  r.issue, r.severity, ctime(&r.stamp));
    }
    close(fd);
}
void view_report(const char* distr_id, int role, int target_id){
    char l_path[256];
    snprintf(l_path,sizeof(l_path),"active_reports-%s",distr_id);
    if(!check_per(l_path,role,1,0)){
        return;
    }
    int fd=open(l_path,O_RDONLY);
    if(fd==-1){
        fprintf(stderr,"eroare deschidere fisier");
        return;
    }
    struct stat st;
    fstat(fd,&st);
    district_h r;
    while(read(fd,&r,sizeof(district_h))==sizeof(district_h)){
        if(r.report_id==target_id){
            printf("ID: %d | User: %s | Lat: %.4f | Lon: %.4f | Issue: %s | Severity: %d | Timestamp: %s", r.report_id, r.insp_name, r.lat, r.lon, r.issue, r.severity, ctime(&r.stamp));
            printf("Descriere: %s\n", r.description);
            break;
        }
    }
    close(fd);
}