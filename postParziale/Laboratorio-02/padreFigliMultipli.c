#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>

int main(int argc, char ** argv){
    if (argc != 2){
        printf("Numero di parametri errato\n");
        exit(1);
    }
    printf("PID padre: %i\n", getpid());
    int N = atoi(argv[0]);
    if (N > 255){
        printf("Il valore eccede 255\n");
        exit(2);
    }
    for (int i = 0; i < N; i++){
        int ret = fork();
        if(ret < 0){
            printf("Si è verificato un errore nel processo figlio con indice %i\n", i);
        }
        if(ret == 0){
            //siamo nel figlio
            printf("PID nel figlio di indice %i: %i\n", i, getpid());
            exit(i);
        }
    }
    for (int i = 0; i < N; i++){
        int status;
        int PIDfiglio = wait(&status);
        if((status & 0xFF) != 0){
            printf("Errore nel valore di ritorno\n");
            exit(3);
        }
        int temp = status >> 8;
        temp &= 0xFF;
        printf("PID figlio di indice %i: %i\n", i, PIDfiglio);
        printf("\tCon valore di ritorno %i\n", temp);
    }
    exit(0);
}