#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

typedef int pipe_t[2];
struct dati {
    char type;
    long int trasformazioni;
} typedef dati;

int main(int argc, char **argv){
    //controllo il numero di parametri
    int nParametriMin = 3;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
        exit(1);
    }

    int N = argc - 1;   //numero di figli da generare
    int M = N;          //numero di pipe da generare
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
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(3);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                close(piped[j][0]);
                if (i != j) close(piped[j][1]);
            }

            //genero il nipote
            if ((pid = fork()) < 0){
                printf("ERROR: durante la fork per il nipote\n");
                exit(-1);
            }
            if (pid == 0){
                //siamo nel nipote
                //leggo il file e sostituisco i caratteri alfabetici con quelli maiuscoli
                char carattere;
                long int trasformazioni = 0;
                //apro il file
                int fd;
                if ((fd = open(argv[i + 1], O_RDWR)) < 0){
                    printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
                    exit(-1);
                }
                while (read(fd, &carattere, sizeof(char)) != 0){
                    if (isalpha(carattere) != 0 && islower(carattere) != 0){
                        lseek(fd, -1L, SEEK_CUR);
                        //carattere = toupper(atoi(&carattere));
                        carattere -= 32;
                        if (write(fd, &carattere, sizeof(char)) == 0){
                            printf("ERROR: durante la write del nipote nel file %s\n", argv[i + 1]);
                            exit(-1);
                        }
                        trasformazioni++;
                    }
                }
                char type = 'n';
                dati toRet;
                toRet.type = type;
                toRet.trasformazioni = trasformazioni;
                if (write(piped[i][1], &toRet, sizeof(dati)) == 0){
                    printf("ERROR: durante la write nelle pipe del nipote di indice %d\n", i);
                    exit(-1);
                }
                int ret = (int) trasformazioni / 256;
                exit(ret);
            }
            //siamo nel figlio
            char carattere;
            long int trasformazioni = 0;
            //apro il file
            int fd;
            if ((fd = open(argv[i + 1], O_RDWR)) < 0){
                printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
                exit(-1);
            }
            while (read(fd, &carattere, sizeof(char)) != 0){
                if (isdigit(carattere) != 0){
                    lseek(fd, -1L, SEEK_CUR);
                    carattere = ' ';
                    if (write(fd, &carattere, sizeof(char)) == 0){
                        printf("ERROR: durante la write del nipote nel file %s\n", argv[i + 1]);
                        exit(-1);
                    }
                    trasformazioni++;
                }
            }
            char type = 'f';
            dati toRet;
            toRet.type = type;
            toRet.trasformazioni = trasformazioni;
            if (write(piped[i][1], &toRet, sizeof(dati)) == 0){
                printf("ERROR: durante la write nelle pipe del figlio di indice %d\n", i);
                exit(-1);
            }
            //il figlio aspetta il nipote per stampare il suo valore
            int pidNipote, status;
            if ((pidNipote = wait(&status)) < 0){
                printf("ERROR: durante la wait nel padre del nipote\n");
                exit(6);
            }
            if ((status & 0xFF) != 0) printf("ERROR: Nipote con pid %d terminato in modo anomalo\n", pidNipote);
            else {
                int ritorno = (int)((status >> 8) & 0xFF);
                printf("Il nipote con pid=%d ha ritornato %d (%d se 255 problemi!)\n", pidNipote, ritorno, ritorno);
            }
            //calcolo il ritorno per il figlio
            int ret = (int) trasformazioni / 256;
            exit(ret);
        }
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);
    }
    //leggo dalle pipe per i figli e i nipoti
    for (int i = 0; i < N; i++){
        dati toRet;
        for (int j = 0; j < 2; j++) {
            if (read(piped[i][0], &toRet, sizeof(dati)) == 0){
                printf("ERROR: durante la read da perte del padre\n");
                exit(8);
            }
        }
        //stampo sul file
        if (toRet.type == 'f') printf("Il figlio di indice %d ha trovato %ld occorrenze di caratteri numerici\n", i, toRet.trasformazioni);
        else printf("Il nipote con indice %d ha trovato %ld occorrenze di caratteri alfabetici\n", i, toRet.trasformazioni);
    }
    //faccio la wait dei figli
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