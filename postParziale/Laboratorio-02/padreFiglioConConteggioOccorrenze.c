#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <string.h>

int main(int elem, char **str){
    if (elem < 3){
        printf("ERROR: numero di parametri errato\n");
        exit(1);
    }
    int N = elem;
    //controllo che i primi N elementi siano tutti file presenti
    for(int i = 1; i < N - 1; i++){
        int fd1;
        if((fd1 = open(str[i], O_RDONLY)) < 0){
            printf("ERROR: il parametro corrispondente al file = %s non è corretto\n", str[i]);
            exit(2);
        }
    }
    //controllo l'ultimo parametro
    if(strlen(str[N - 1]) != 1){
        printf("ERROR: l'ultimo parametro non è di un carattere\n");
    }
    char caracter = str[N - 1][0];
    //generono N figli
    for(int i = 0; i < N - 2; i++){
        int ret = fork();
        if (ret < 0){
            printf("ERROR: il figlio non è stato generato correttamente\n");
            exit(3);
        }
        if(ret == 0){
            //siamo nel figlio che deve contare le occorenze del carattere all'interno del file
            int fd1;
            if((fd1 = open(str[i + 1], O_RDONLY)) < 0){
                printf("ERROR: il file non è stato aperto correttamente\n");
                exit(-1);
            }
            int counter = 0;
            while(1){
                char car;
                int scanRet = read(fd1, &car, 1);
                if (scanRet == 0){
                    //abbiamo raggiunto la fine del file --> usciamo dal while
                    break;
                }
                if(car == caracter){
                    counter++;
                }
            }
            exit(counter);
        }
    }
    //stampiamo per i processi terminati
    for(int i = 0; i < N - 2; i++){
        int status;
        int PIDfiglio = wait(&status);
        if(PIDfiglio < 0 ){
            printf("ERROR: durante la fase di wait del processo\n");
            exit(4);
        }
        if((status & 0xFF) != 0){
            printf("ERROR: durante l'esecuzione del figlio si sono verificati dei problemi\n");
            exit(3);
        }
        int temp = status >> 8;
        temp &= 0xFF;
        printf("Per il figlio con PID = %i sono state trovate n°%i occorrenze (se 255 problemi!) del carattere %c\n", PIDfiglio, temp, caracter);
    }
    exit(0);
}