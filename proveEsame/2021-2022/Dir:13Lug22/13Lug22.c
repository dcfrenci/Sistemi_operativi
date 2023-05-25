#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char ** argv){
    //controllo il numero di parametri
    int nParametriMin = 4;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d", nParametriMin, argc);
        exit(1);
    }
    //controllo i parametri
    if (argv[1][0] != '/'){
        printf("ERROR: il primo parametro deve essere un nome di file assoluto\n");
        exit(2);
    }
    int L;
    if ((L = atoi(argv[2])) <= 0){
        printf("ERROR: il secondo parametro che indica il numero di righe del file deve essere maggiore di zero\n");
        exit(3);
    }
    int Q = argc - 3;       //numero di figli che dovranno essere generati
    //genero i processi figli
    int M = Q;
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
    //apro il file F e controllo che esista e si possa leggere
    /*int F;
    if ((F = open(argv[1], O_RDONLY)) < 0){
        printf("ERROR: il parametro %s non è un file o non può essere aperto in lettura\n", argv[1]);
        exit(6);
    }*/
    //genero i figli
    for (int q = 0; q < Q; q++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(6);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                close(piped[j][0]);
                if (q != j) close(piped[j][1]);
            }
            //il figlio legge una linea contando le occorrenze del carattere argv[q + 3][0]
            char carattere;
            int occorrenze = 0;
            //apro il file F e controllo che esista e si possa leggere
            int F;
            if ((F = open(argv[1], O_RDONLY)) < 0){
                printf("ERROR: il parametro %s non è un file o non può essere aperto in lettura\n", argv[1]);
                exit(7);
            }
            //leggo il file
            while (read(F, &carattere, sizeof(char)) != 0){
                if (carattere == argv[q + 3][0]){
                    occorrenze++;
                }
                if (carattere == '\n'){
                    //ho letto una riga completa --> scrivo sulla pipe le occorrenze in quella riga
                    if (write(piped[q][1], &occorrenze, sizeof(int)) == 0){
                        printf("ERROR: durante la write nella pipe\n");
                        exit(-1);
                    }
                    int pidFiglio = getpid();
                    if (write(piped[q][1], &pidFiglio, sizeof(int)) == 0){
                        printf("ERROR: durante la write nella pipe\n");
                        exit(-1);
                    }
                    occorrenze = 0;         //resetto occorrenze per la prossima linea
                }
            }
            //scrivo sulla pipe per l'ultima riga
            if (write(piped[q][1], &occorrenze, sizeof(int)) == 0){
                printf("ERROR: durante la write nella pipe\n");
                exit(-1);
            }
            int pidFiglio = getpid();
            if (write(piped[q][1], &pidFiglio, sizeof(int)) == 0){
                printf("ERROR: durante la write nella pipe\n");
                exit(-1);
            }
            exit(occorrenze);
        }
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);
    }
    //leggo dalle pipe dei figli
    for (int i = 0; i < L; i++){
        printf("Linea %d del file %s\n", i, argv[1]);
        for (int q = 0; q < Q; q++){
            int occorrenze, pid;
            read(piped[q][0], &occorrenze, sizeof(int));
            read(piped[q][0], &pid, sizeof(int));
            printf("Figlio con indice %d e pid %d ha letto %d caratteri %c nella linea corrente\n", q, pid, occorrenze, argv[q + 3][0]);
        }
    }
    //aspetto che tutti i figli termino e stampo il loro valore di ritorno
    for (int q = 0; q < Q; q++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(8);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    exit(0);
}