#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>

int mia_random(int n){
    int casuale = rand() % n;
    return casuale;
}

int main(int argc, char ** argv){
    //controllo il nuemro di parametri
    int nParametriMin = 4;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d", nParametriMin, argc);
        exit(1);
    }
    //controllo l'ultimo parametro
    int N = argc - 2;       //numero di parametri file
    if (atoi(argv[N + 2]) < 0 || atoi(argv[N + 2]) > 255){
        printf("ERROR: l'ultimo parametro non soddisfa le condizioni (parametro > 0 && parametro <= 255)");
        exit(2);
    }
    //inizializzo il seme random
    srand(time(NULL));
    //creo il file
    int fd;
    if ((fd = creat("/tmp/creato", O_WRONLY)) == - 1){
        printf("ERROR: durante la creazione del file\n");
    }
    //generazione delle pipe
    int piped[2][2];
    if (pipe(piped[0]) < 0 || pipe(piped[1])){
        printf("ERROR: durante la creazioned della pipe\n");
        exit(4);
    }
    //generazione dei figli
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(3);
        }
        if (pid == 0){
            //siamo nel figlio
            close(piped[0][1]);
            close(piped[1][0]);
            char str[255];
            int dim = 0;
            int carat = 0;
            int file;
            if ((file = open(argv[i + 1], O_RDONLY)) < 0){
                printf("ERROR: il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
                exit(2);
            }
            printf("Leggo il file %s", argv[i + 1]);
            while (read(file, &str[dim], sizeof(char)) != 0){
                if (str[dim] == '\n' || str[dim] == 0){
                    printf("%s\n", str);
                    //abbiamo letto tutta una linea o siamo arrivati alla terminazione del file
                    write(piped[0][0], &dim, sizeof(int));
                    int pos = 0; 
                    read(piped[1][1], &pos, sizeof(int));
                    if (pos < dim){
                        write(fd, &str[pos], sizeof(char));
                        carat++;
                    }
                    dim = 0; 
                } else {
                    dim++;
                }
            }
            exit(carat);
        }
        //siamo nel padre
        close(piped[0][0]);
        close(piped[1][1]);
        int dim;
        read(piped[0][1], &dim, sizeof(int));
        int random = mia_random(dim);
        write(piped[1][0], &random, sizeof(int));
    }
    //aspetto la terminazione dei processi
    for (int i = 0; i < N; i++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
        }
    }
}