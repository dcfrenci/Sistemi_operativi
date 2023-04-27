#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>

int main(void){
    printf("PID padre:  %i\n", getpid());
    int status;
    int pidFiglio = wait(&status);
    if (pidFiglio < 0){
        printf("Il figlio non è terminato correttamente\n");
    }
    exit(0);
}