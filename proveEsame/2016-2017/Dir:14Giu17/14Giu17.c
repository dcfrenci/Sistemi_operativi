#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef int pipe_t[2];

bool finish(int * child, int dim){
    for (int i = 0; i < dim; i++){
        if (child[i] == 0) return true;
    }
    return false;
}

int main(int argc, char** argv){
    //controllo il numero di parametri
    int nParametriMin = 3;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
        exit(1);
    }
    //controllo i parametri in ingresso
    if (strlen(argv[argc - 1]) != 1){
        printf("ERROR: il parametro Cx non è corretto, deve essere un carattere singolo e alfabetico\n");
        exit(2);
    }
    if (isalpha(argv[argc - 1][0]) != 0){
        printf("ERROR: il parametro Cx non è corretto, deve essere un carattere singolo e alfabetico\n");
        exit(3);
    }
    int N = argc - 2;           //numero di figli da generare
    //pipe per comunicazione figlio --> padre
    int M = N;                  //numero di pipe da generare
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
    //pipe per la comunicazione padre --> figlio
    pipe_t* pipedPadre = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (pipedPadre == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(6);
    }
    //creo M pipe
    for (int j = 0; j < M; j++){
        if (pipe(pipedPadre[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(7);
        }
    }
    //genero i figli
    char Cx = argv[argc - 1][0];
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(8);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                close(piped[j][0]);             //chiudo tutti i lati di lettura della pipe figlio --> padre
                if (i != j) close(piped[j][1]);
                close(pipedPadre[j][1]);        //chiudo tutti i lati di scrittura della pipe padre --> figlio
            }
            //apro il file 
            int fd;
            if ((fd = open(argv[i + 1], O_RDWR)) < 0){
                printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
                exit(-1);
            }
            //leggo il file alla ricerca del carattere Cx
            char carattere;
            long int pos = 0; 
            int scambi = 0;
            while (read(fd, &carattere, sizeof(char)) != 0){
                if (carattere == Cx){
                    //ho trovato un carattere --> chiedo al padre con cosa lo devo sostituire
                    if (write(piped[i][1], &pos, sizeof(long int)) == 0){
                        printf("ERROR: nella write sulla pipe causata dal figlio %d\n", i);
                        exit(-1);
                    }
                    if (read(pipedPadre[i][0], &carattere, sizeof(char)) == 0){
                        printf("ERROR: durante la read nel figlio %d della pipePadre\n", i);
                        exit(-1);
                    }
                    //sostituisco il carattere
                    if (carattere != '\n'){
                        lseek(fd, -1L, SEEK_CUR);
                        write(fd, &carattere, sizeof(char));
                        scambi++;
                    }
                }
                pos++;  //aggiorno il counter
            }
            //quando il file termina mando un comando specifico per segnalare che il figlio ha termino di leggere il file
            pos = - 1; 
            if (write(piped[i][1], &pos, sizeof(long int)) == 0){
                printf("ERROR: nella write sulla pipe causata dal figlio %d\n", i);
                exit(-1);
            }
            exit(scambi);
        }
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);
        close(pipedPadre[i][0]);
    }
    //se i figli leggono dalle pipe il padre chiede all'utente con che cosa sostituire
    int *childFinished = calloc(N, sizeof(int));
    if (childFinished == NULL){
        printf("ERROR: durante l'allocazione di memoria per l'array\n");
        exit(9);
    }
    while (finish(childFinished, N)){
        long int pos;
        for (int i = 0; i < N; i++){
            if (childFinished[i] != 1){
                if (read(piped[i][0], &pos, sizeof(long int)) == 0){
                    printf("ERROR: durante la read della pipe nel padre\n");
                    exit(10);
                }
                if (pos == - 1){
                    //segno che il figlio[i] ha terminato di leggere
                    childFinished[i] = 1;
                    continue;
                }
                //chiedo all'utente con che carattere vuole sostituire quel carattere in quella posizione
                printf("Per il file %s è stata trovata un occorrenza del carattere %c. Con quale carattere si vuole sostituire?\t", argv[i + 1], Cx);
                char carattere;
                if (scanf("%c", &carattere) == 0){
                    printf("ERROR: durante la lettura dell'input utente per il carattere nel padre\n");
                    exit(11);
                }
                char scarto;
                if (scanf("%c", &scarto) == 0){
                    printf("ERROR: durante la lettura dell'input utente per il carattere nel padre\n");
                    exit(12);
                }
                //se il carattere non è un carattere vuoto allora lo mando al figlio
                if (write(pipedPadre[i][1], &carattere, sizeof(char)) == 0){
                    printf("ERROR: durante la write nella pipe nel padre\n");
                    exit(13);
                }
            }
        }
    }
    //aspetto che tutti i figli terminino
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

    exit(0);
}