#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>

int mia_random(int n){
    //genera numero random compreso tra 0 e n-1
    int casuale = rand() % n;
    return casuale;
}

int main(int argc, char ** arcv){
    //riporto su stdout il pid del padre
    printf("PID padre: %i\n", getpid());
    int ret = fork();
    if (ret == -1){
        //la fork non ha avuto successo
        exit(-1);
    }
    if (ret == 0){
        //siamo nel figlio
        printf("Nel figlio ==========\n");
        printf("PID del figlio: %i\n", getpid());
        printf("PID padre dal figlio: %i\n", getppid());
        srand(time(NULL));
        exit(mia_random(100));
    }
    int status;
    int pidFiglio = wait(&status); 
    printf("Nel padre ==========\n");
    printf("PID del filgio dal padre: %i\n", pidFiglio);
    if ((status & 0xFF) != 0){
        //errore/i verificatesi nella
        printf("Il figlio non è terminato correttamente\n");
    }
    else{
        //il figlio è terminato correttamente
        //faccio lo shift
        ret = status >> 8;
        printf("Valore ritornato dal figlio: %i\n", ret &= 0xFF);
    }
    pidFiglio = wait(&status);
    if (pidFiglio < 0){
        printf("Errore nella seconda wait\n");
    }
    exit(0);
}