#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char ** argv){
    //controllo il numero di parametri
    if(argc < 3){
        printf("ERROR: numero di parametri insufficienti\n");
        exit(1);
    }
    //genero i processi figli
    int N = argc - 1;
    for(int i = 0; i < N; i++){
        int pid;
        if((pid = fork()) < 0){
            printf("ERROR: durante la creazione del figlio\n");
            exit(2);
        }
        if(pid == 0){
            //siamo nel figlio
            char * file = calloc(strlen(argv[i + 1]) + strlen(".sort") + 1, sizeof(char));
            file = strcat(file, argv[i + 1]);
            file = strcat(file, ".sort");
            int fd = open(file, O_CREAT);
            if(fd < 0){
                printf("ERROR: durante la creazione del file\n");
            }
            //genero il nipote
            if((pid = fork()) < 0){
                printf("ERROR: durante la creazione del nipote\n");
                exit(3);
            }
            if(pid == 0){
                //siamo nel nipote
                close(0);
                open(argv[i + 1], O_RDONLY);
                close(1);
                open(file, O_WRONLY);
                execl("sort", "sort", "&", (char *)0); 
                exit(-1);
            }
            int status, pidNipote;
            if((pidNipote = wait(&status)) < 0){
                printf("ERROR: durante la wait del nipote con pid = %d\n", pidNipote);
                exit(4);
            }
             if ((status & 0xFF) != 0){
            printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidNipote);
            }
            int ritorno = (int)((status >> 8) & 0xFF);
            exit(ritorno);
        }
    }
    //aspetto tutti i figli
    for(int i = 0; i < N; i++){
        int pidFiglio, status;
        if((pidFiglio = wait(&status))){
            printf("ERROR: durante la wait del figlio con pid = %d\n", pidFiglio);
            exit(4);
        }
        if ((status & 0xFF) != 0){
    	printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        }
        int ritorno = (int)((status >> 8) & 0xFF);
        printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
        exit(0);
    }
}