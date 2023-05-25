#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

typedef int pipe_t[2];

int main(int argc, char** argv){
    //controllo il numero di parametri
    int nParametriMin = 3;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
        exit(1);
    }
    //controllo i parametri
    int nroLinee = atoi(argv[argc - 1]);
    if (nroLinee <= 0){
        printf("ERROR: il parametro corrispondente alle linee non è corretto, deve essere un numero maggiore di zero\n");
        exit(1);
    }
    //creo il file
    int fcreato;
    if ((fcreato = creat("DELLA_CASA", O_WRONLY)) == - 1){
        printf("ERROR: durante la creazione del file\n");
    }

    int N = argc - 2;
    int M = N;		//numero di pipe da creare
    //alloco la memoria per la pipe
    pipe_t* piped = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (piped == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(4);
    }
    //creo M pipe
    for (int j = 0; j < M; j++){
        if (pipe(piped[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(5);
        }
    }
    //creo il vettore per tutte le linee
    /*char *tutteLinee = calloc(N, sizeof(char) * nroLinee * 250);
    if (tutteLinee == NULL){
        printf("ERROR: durante l'allocazione di memoria per l'array\n");
        exit(6);
    }*/
    char tutteLinee[N][250];
    //genero i figli
    for (int n = 0; n < N; n++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(3);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                if (j != n - 1 || n == 0) close(piped[j][0]);
                if (n != j) close(piped[j][1]);
            }
            //apro il file
            int fd;
            if ((fd = open(argv[n + 1], O_RDONLY)) < 0){
                printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[n + 1]);
                exit(2);
            }
            //leggo il file
            char carattere;
            char linea[250];
            int pos = 0;
            while (read(fd, &carattere, sizeof(char)) != 0){
                if (carattere == '\n'){
                    //ho finito di leggere una riga
                    if (n != 0){
                        if (read(piped[n - 1][0], tutteLinee, sizeof(char) * N * 250) == 0){
                            printf("ERROR: durante la read nel figlio della pipe\n");
                            exit(-1);
                        }
                    }
                    //inserisco nell'array il vettore stringa che ho letto
                    linea[pos] = 0;                     //rendo la stringa zero terminata
                    //copio la riga nel vettore strighe
                    for (int i = 0; i < 250; i++){
                        tutteLinee[n][i] = linea[i];
                    }
                    if (write(piped[n][1], tutteLinee, sizeof(char) * N * 250) == 0){
                        printf("ERROR: durante la write nel figlio della pipe\n");
                        exit(-1);
                    }
                    pos = 0;                //resetto il counter
                } else {
                    linea[pos] = carattere;
                    pos++;
                }
            }
            exit(pos);
        }
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);
        if (i != M - 1) close(piped[i][0]);
    }
    //leggo dalla pipe tutte le linee lette dai figli
    for (int k = 0; k < nroLinee; k++){
        if (read(piped[N][0], tutteLinee, sizeof(char) * N * 250) == 0){
            printf("ERROR: nella lettura della pipe nel padre\n");
            exit(6);
        }
        //scrivo nel file
        for (int i = 0; i < N * nroLinee; i++){
            if (write(fcreato, tutteLinee[i], sizeof(char) * 250) == 0){
                printf("ERROR: nella scrittura del nuovo file nel padre\n");
                exit(7);
            }
            if (i < N * nroLinee - 1){
                char carattere = '\n';
                if (write(fcreato, &carattere, sizeof(char)) == 0){
                    printf("ERROR: nella scrittura del nuovo file nel padre\n");
                    exit(8);
                }
            }
        }
    }
    //aspetto che tutti i processi siano terminati
    for (int n = 0; n < N; n++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (%d se uguale 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    exit(0);
}