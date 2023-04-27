#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <time.h>

int random(int n){
    //return random between 0 and n+ 100
    int casuale = rand() % (n + 101);
    return casuale;
}
int main(int val, char **str){
    if(val != 2){
        printf("Numero di valori errato\n");
    }
    int N = atoi(str[1]);
    printf("PID padre: %i con N = %i\n", getpid(), N);
    for(int i = 0; i < N; i++){
        int ret = fork();
        if (ret == 0){
            //siamo nel figlio
            printf("PID nel figlio di indice %i: %i\n", i, getpid());
            srand(time(NULL));
            exit(random(i));
        }
    }
    for(int i = 0; i < N; i++){
        int status;
        int PIDfiglio = wait(&status);
        if((status & 0xFF) != 0){
            printf("Errore nel figlio %i\n", PIDfiglio);
            exit(1);
        }
        int ret = status >> 8;
        ret &= 0xFF;
        printf("PIDfiglio = %i, Indice = %i, Random = %i", PIDfiglio, i, ret);
    }
    exit(0);
}