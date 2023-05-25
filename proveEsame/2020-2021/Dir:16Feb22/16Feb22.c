#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

typedef int pipe_t[2];
typedef struct dati{
    int c1; 
    long int c2;
} dati;

int main(int argc, char **argv){
    //controllo il numero di parametri
    int nParametriMin = 3;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
        exit(1);
    }
    //controllo i parametri
    //if (!isalpha(atoi(argv[argc - 1])) || !islower(atoi(argv[argc - 1])) || strlen(argv[argc - 1]) > 1){
    if (!isalpha(argv[argc - 1][0]) || !islower(argv[argc - 1][0]) || strlen(argv[argc - 1]) > 1){
        printf("ERROR: l'ultimo parametro inserito è errato poichè doveva essere un carattere alfabetico minuscolo\n");
        exit(2);
    }
    char C = argv[argc - 1][0];
    int N = argc - 2;
    int M = N;
    //alloco la memoria per la pipe
    pipe_t* piped = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (piped == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(3);
    }
    //creo M pipe
    for (int j = 0; j < M; j++){
        if (pipe(piped[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(4);
        }
    }

    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(5);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                if (i != j) close(piped[j][1]);
                //se non è la prima pipe devo lasciare aperto la pipe di lettura precendente così da poter leggere la struttare dati
                if (i != 0 && j == i - 1){
                    continue;
                } else {
                    close(piped[j][0]);
                }
            }
            //apro il file
            int fd;
            if ((fd = open(argv[i + 1], O_RDONLY)) < 0){
                printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
                exit(2);
            }
            //leggo dal file alla ricerca del carattere
            char ch;
            long int occorrenze = 0;
            while (read(fd, &ch, sizeof(char)) != 0){
                if (ch == C) occorrenze++;
            }
            //creo il vettore da scrivere sull pipe
            dati* arr;
            if (i == 0){
                arr = malloc(sizeof(dati));
            } else {
                arr = malloc(sizeof(dati) * (i + 1));
                read(piped[i - 1][0], arr, sizeof(dati) * i);
            }
            //carico il vettore con i nuovi dati
            arr[i].c1 = getpid();
            arr[i].c2 = occorrenze;
            if (write(piped[i][1], arr, sizeof(dati) * (i + 1)) == 0){
                printf("ERROR: durante la scrittura sulla pipe del figlio con indice %d\n", i);
                exit(-1);
            }
            exit(i);
        }
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);                         //chiudo tutti i lati di scrittura
        if (i != M - 1) close(piped[i][0]);         //chiudo i lati di lettura di tutti i figli meno l'ultimo 
    }
    //leggo dalla pipe la l'array di strutture dati
    dati *arr = malloc(N * sizeof(dati));
    read(piped[N - 1][0], arr, sizeof(dati) * N);
    for (int i = 0; i < N; i++){
        printf("Per il processo %d e il file %s sono state trovate %ld occorrenze del carattere %c\n", arr[i].c1, argv[i + 1], arr[i].c2, C);
    }
    //aspetto che tutti i processi termino
    for (int i = 0; i < N; i++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    free(arr);
    exit(0);
}