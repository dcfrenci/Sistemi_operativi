#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char** argv){
    int nParametriMin = 2;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d", nParametriMin, argc);
        exit(1);
    }
    int N = argc - 1;
    for (int i = 0; i < N; i++){
        int fd;
        if ((fd = open(argv[i + 1], O_RDONLY)) < 0){
            printf("ERROR: il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
            exit(2);
        }
    }
    int piped[2];
    if (pipe(piped) < 0){
        printf("ERROR: durante la creazioned della pipe\n");
        exit(6);
    }
    //genero i figli
    int lunghTot = 0;
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork nel figlio\n");
            exit(3);
        }
        if (pid == 0){
            //siamo nel figlio
            int pidNip;
            if ((pidNip = fork()) < 0){
                printf("ERROR: durante la fork nel nipote\n");
                exit(4);
            }
            if (pidNip == 0){
                //siamo nel nipote
                close(0);
                open(argv[i + 1], O_RDONLY);
                execlp("wc", "wc", "-l", (char *)0);
                printf("ERROR: durante l'execlp nel nipote\n");
                exit(-1);
            }
            //siamo nel figlio
            close(piped[0]);
            int pidNipote, statusNipote;
            if ((pidNipote = wait(&statusNipote)) < 0){
                printf("ERROR: durante la wait nel padre del figlio\n");
                exit(7);
            }
            if ((statusNipote & 0xFF) != 0) printf("ERROR: Nipote con pid %d terminato in modo anomalo\n", pidNipote);
            int ritorno = (int)((statusNipote >> 8) & 0xFF);
            char str[5];
            read(1, &str, 5 * sizeof(char));
            int righe = atoi(str);
            fprintf("%d\n", righe);
            //read(0, &righe, sizeof(int));
            write(piped[1], &righe, sizeof(int));
            exit(ritorno);
        }
        //siamo nel padre
        
    }

    //stampo il numero totale di linee trovante nei file
    //aspetto il termine di tutti i processi
    for (int i = 0; i < N; i++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(8);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %c (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
        close(piped[1]);
        int num = 0;
        read(piped[0], "%d", num);
        lunghTot += num;
    }
    printf("Nella totalità dei file analizzati sono state lette %d righe.\n", lunghTot);
    exit(0);
}