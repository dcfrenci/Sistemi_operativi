#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
typedef int pipe_t[2];

int main(int argc, char **argv){
    //controllo il numero di parametri in ingresso 
    if(argc < 4){
        printf("ERROR: numero di parmetri insufficiente. Dovevano essere almeno 4 ma erano %d\n", argc);
        exit(1);
    }
    //controllo il primo parametro 
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0){
        printf("ERROR: il parametro %s non è un file o non può essere aperto in lettura\n", argv[1]);
        exit(2);
    }
    //controllo che gli altri parametri siano dei caratteri
    int N = argc - 1;
    for (int i = 1; i < N; i++){
        if (isalpha(argv[i + 1]) == 0){
            printf("ERROR: il parametro %s non è un caratteere\n", argv[i + 1]);
            exit(3);
        }
    }
    //creo la pipe per i figli
    int M = N;
    pipe_t* piped = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (piped == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(4);
    }
    for (int j = 0; j < M; j++){
        if (pipe(piped[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(5);
        }
    }
    //creo N processi figli
    int fd = fopen(argv[1], O_RDONLY);
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(4);
        }
        if (pid == 0){
            //siamo nel figlio
            long int occorrenze = 0;
            char carattere = 0;
            while (read(fd, &carattere, sizeof(char)) != 0){
                if (carattere == argv[i + 1]){
                    occorrenze++;
                }
            }
            exit(occorrenze);
        }
    }
    //siamo nel padre
    for ()
}