#include "lib1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    int user=-1;//0 insp,1 manager
    char command[17];
    char name[MAX_NUME];
    char distr_id[20];

    if(argc<6){
        fprintf(stderr, "eroare argumente linie de comanda");
        exit(-1);
    }
    for(int i=1;i<5;i++){
        if(strcmp(argv[i],"--role")==0){
            if(strcmp(argv[i+1],"manager")==0){
                user=1;
                i++;
                continue;
            }
            else if(strcmp(argv[i+1],"inspector")==0){
                user=0;
                i++;
                continue;
            }
            else{
                fprintf(stderr,"eroare rol, se accepta doara inspector sau manager");
                exit(-1);
            }
        }
        else if(strcmp(argv[i],"--user")==0){
            strcpy(name,argv[i+1]);
            i++;
            continue;
        }
    }
    strcpy(command,argv[5]);
    strcpy(distr_id,argv[6]);

    init_district(distr_id);
    log_command(distr_id,user,name,command);

    if(strcmp(command,"--add")==0){
        float lat,lon;
        int severity;
        char issue[MAX_ISS],desc[MAX_DESC];
        printf("latitudine: ");
        scanf("%f",&lat);
        printf("longitudine: ");
        scanf("%f",&lon);
        printf("categorie: ");
        scanf("%31s",issue);
        printf("severitate: ");
        scanf("%d",&severity);
        getchar();
        printf("descriere: ");
        fgets(desc,MAX_DESC,stdin);
        desc[strcspn(desc,"\n")]=0;
        add_report(distr_id,name,user,lat,lon,issue,severity,desc);
    }
    else if(strcmp(command,"--list")==0){
        list_reports(distr_id,user);
    }
    else if(strcmp(command,"--view")==0){
        if(argc<8){
            fprintf(stderr,"eroare argumente lipsa pentru view");
            exit(-1);
        }
        int target_id=atoi(argv[7]);
        view_report(distr_id,user,target_id);
    }
    else if(strcmp(command,"--remove_report")==0){
        if(argc<8){
            fprintf(stderr,"eroare argumente lipsa pentru remove_report");
            exit(-1);
        }
        int target_id=atoi(argv[7]);
        remove_report(distr_id,user,target_id);
    }
    else if(strcmp(command,"--update_threshold")==0){
        if(argc<8){
            fprintf(stderr,"eroare argumente lipsa pentru update_threshold");
            exit(-1);
        }
        int new_val=atoi(argv[7]);
        update_threshold(distr_id,user,new_val);
    }
    else if(strcmp(command,"--filter")==0){
        int cond_count=argc-7;
        if(cond_count>0){
            filter_reports(distr_id,user,cond_count,&argv[7]);
        }
        else{
            fprintf(stderr,"eroare lipsesc conditii pentru filter");
        }
    }

    return 0;
}
