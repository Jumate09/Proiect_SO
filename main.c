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
    if(strcmp(argv[2],"manager")==0){
        user=1;
    }
    else if(strcmp(argv[2],"inspector")==0){
        user=0;
    }
    strcpy(name,argv[4]);
    strcpy(command,argv[5]);
    strcpy(distr_id,argv[6]);

    

    init_district(distr_id);
    //printf("\n%d\n",check_per("district1/logged_district",1,1,1));
    log_command(distr_id,user,name,command);
    //add_report(distr_id,name,user,3.14,8.967,"road",5,"s-a spart batraneti");
    //list_reports(distr_id,user);
    //view_report(distr_id,user,3);
    //remove_report(distr_id,1,3);
    list_reports(distr_id,user);
}





