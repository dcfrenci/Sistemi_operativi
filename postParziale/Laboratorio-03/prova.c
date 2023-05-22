#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>

int main(int argc, char ** argv){
    //controlliamo l'input
    if(argc != 1){
        printf("ERROR: numero di parametri errato\n");
        exit(1);
    }
    int choose = 0;
    printf("Si desidera invocare ricorsivamente il programma ? (No = 0, si = {...})\n");
    scanf("%d", &choose);
    //verifico se devo chiamare ricorsivamente il programma
    if(choose != 0){
        //aggiungo al PATH la directory
        execl("/home/dc/Esercizi/postParziale/Laboratorio-03/prova", "prova", (char *)0);
    }
    exit(0);
    printf("Termino il programma\n");
}