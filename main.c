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
    for(int i=1;i<5;i++)
    {
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
    printf("\n%d\n",check_per("district1/logged_district",1,1,1));

}





