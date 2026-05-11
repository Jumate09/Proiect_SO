#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int sig){
    keep_running = 0;
    char msg[] = "\ns-a primit SIGINT se inchide\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

void sigusr1_handler(int sig){
    char msg[] = "un nou raport a fost adaugat\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main(){
    pid_t pid = getpid();
    
    int fd = open(".monitor_pid", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd == -1){
        fprintf(stderr, "eroare creare fisier .monitor_pid\n");
        exit(-1);
    }
    
    char pid_str[16];
    int len = snprintf(pid_str, sizeof(pid_str), "%d\n", pid);
    if(write(fd, pid_str, len) != len){
        fprintf(stderr, "eroare scriere pid\n");
    }
    close(fd);
    
    printf("Monitorul a pornit cu PID: %d\n", pid);
    struct sigaction sa_int, sa_usr;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if(sigaction(SIGINT, &sa_int, NULL) == -1){
        fprintf(stderr, "eroare setare sigaction SIGINT\n");
        exit(-1);
    }
    sa_usr.sa_handler = sigusr1_handler;
    sigemptyset(&sa_usr.sa_mask);
    sa_usr.sa_flags = 0;
    if(sigaction(SIGUSR1, &sa_usr, NULL) == -1){
        fprintf(stderr, "eroare setare sigaction SIGUSR1\n");
        exit(-1);
    }

    while(keep_running){
        pause();
    }
    if(unlink(".monitor_pid") == -1){
        fprintf(stderr, "eroare la stergerea .monitor_pid\n");
    } else {
        printf("fisierul .monitor_pid a fost sters exec s-a terminat\n");
    }

    return 0;
}