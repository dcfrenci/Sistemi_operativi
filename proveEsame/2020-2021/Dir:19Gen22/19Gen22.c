#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

typedef int pipe_t[2];

int main(int argc, char ** argv){
    //controllo il numero di parametri
    int nParametriMin = 2;
    if (argc <= nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
        exit(1);
    }
    //controllo il valore dei parametri
    int C = atoi(argv[argc - 1]);
    if (C % 2 == 0 || C <= 0){
        printf("ERROR: il parametro C non è corretto, doveva esssere maggiore di zero e dispari\n");
        exit(2);
    }
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
        //apro il file e verifico quanto sia lungo per poterlo comunicare ai figli affinché ne leggano solo metà
        int fd;
        if ((fd = open(argv[i + 1], O_RDONLY)) < 0){
            printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
            exit(5);
        }
        int dim = 0;
        char ch;
        while (read(fd, &ch, sizeof(char)) != 0){
            dim++;
        }
        //creo i figli
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork per il primo figlio\n");
            exit(6);
        }
        if (pid == 0){
            //siamo nel figlio 1
            //chiudo i lati non usati per la comunicazione dela pipe (lettura dalla pipe)
            for (int j = 0; j < M; j++) {                               
                close(piped[j][0]);
                if (i != j) close(piped[j][1]);
            }
            //apro il file
            fd = open(argv[i + 1], O_RDONLY);
            //leggo dal file
            char b1[C];
            int letti = 0;
            while (read(fd, b1, sizeof(char) * C) != 0){
                if (letti >= dim / 2) break;    //se leggo la metà dei char del file termino di leggere
                //scrivo sulla pipe il blocco letto
                if (write(piped[i][1], b1, sizeof(char) * C) == 0){
                    printf("ERROR: durante la write sulla pipe del figlio %d\n", i);
                    exit(-1);
                }
                letti += C;     //aggiorno il counter
            }
            int nro = letti / C;
            exit(nro);
        }
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork per il secondo figlio\n");
            exit(7);
        }
        if (pid == 0){
            //siamo nel figlio 2
            //chiudo i lati non usati per la comunicazione della pipe (scrittura sulla pipe)
            for (int i = 0; i < M; i++){
                close(piped[i][1]);
            }
            //apro il file
            fd = open(argv[i + 1], O_RDONLY);
            //creo il file
            char *fileName = calloc(strlen(argv[i + 1]) + strlen(".mescolato") + 1, sizeof(char));
            if (fileName == NULL){
                printf("ERROR: durante l'allocazione di memoria per l'array\n");
                exit(-1);
            }
            fileName = strcat(fileName, argv[i + 1]);
            fileName = strcat(fileName, ".mescolato");
            int fcreato;
            if ((fcreato = creat(fileName, O_WRONLY)) == - 1){
                printf("ERROR: durante la creazione del file\n");
            }
            //leggo dal file
            char b2[C];
            int letti = 0;
            while (read(fd, b2, sizeof(char) * C) != 0){
                if (letti >= dim / 2){
                    //ho letto la prima parte di file (letta già dal primo figlio) --> il secondo può cominciare a scrivere i dati
                    write(fcreato, b2, sizeof(char) * C);
                    //leggo dalla pipe per il blocco letto dal primo figlio
                    if (read(piped[i][0], b2, sizeof(char) * C) == 0){
                        printf("ERROR: durante la lettura dalla pipe del figlio %d\n", i);
                        exit(-1);
                    }
                    //scrivo i dati letti
                    write(fcreato, b2, sizeof(char) * C);
                }   
                letti += C;     //aggiorno il counter
            }
            int nro = letti / C;
            exit(nro);
        }
    }
    //aspetto che tutti i processi terminino
    for (int i = 0; i < N * 2; i++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %c (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    exit(0);
}