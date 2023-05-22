#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
typedef int pipe_t[2];

struct {
    int pid; 
    int dim;
    char str[250];
} typedef dati;

int main(int argc, char** argv){
    //controllo il numero di parametri
    int nParametriMin = 3;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d", nParametriMin, argc);
        exit(1);
    }
    int N = argc - 1;
    int M = N;
    //alloco la memoria per la pipe
    pipe_t* piped = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (piped == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(2);
    }
    //creo M pipe
    for (int j = 0; j < M; j++){
        if (pipe(piped[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(3);
        }
    }

    //genero i figli
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(5);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j=0; j < M; j++) {                               
                close(piped[j][0]);
                if (i != j) close(piped[j][1]);
            }
            //creo la pipe per il la comunicazione tra nipote e figlio
            int pips[2];
            if (pipe(pips) < 0){
                printf("ERROR: durante la creazioned della pipe\n");
                exit(4);
            }
            if ((pid = fork()) < 0){
                printf("ERROR: durante la fork\n");
                exit(6);
            }
            if (pid == 0){
                //siamo nel nipote
                close(1);
                dup(pips[1]);
                close(pips[0]);
                close(pips[1]);
                execlp("sort", "sort", argv[i + 1], (char *)0);
                exit(-1);
            }
            //siamo nel figlio
            close(pips[0]);
            //creo la struttura dati da ritornare
            dati toRet;
            toRet.pid = pid;
            toRet.dim = 0;
            while (read(pips[1], &toRet.str[toRet.dim], sizeof(char)) != 0){
                if (toRet.str[toRet.dim] == '\n' || toRet.dim == 250){
                    toRet.str[toRet.dim] = 0;
                    break;
                }
                toRet.dim++;
            }
            printf("%s", toRet.str);
            write(piped[i][1], &toRet, sizeof(toRet));
            exit(toRet.dim - 1);
        }
    }
    //siamo nel padre
    for (int i =0; i < N; i++){
        //chiudo i lati della pipe che non sono necessari
        close(piped[i][1]);
        dati rit;
        read(piped[i][0], &rit, sizeof(dati));
        printf("Per il file %s è determinata la seguente struttura:\n\tpidNipote = %d\n\tdimensione della riga = %d\n\tstringa:%s\t\n", argv[i + 1], rit.pid, rit.dim, rit.str);
    }
    for (int i = 0; i < N; i++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (se %d = 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    exit(0);
}