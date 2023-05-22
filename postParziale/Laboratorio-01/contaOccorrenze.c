#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char ** argv){
    //controllo il numero di parametri
    if (argc != 3){
        printf("ERROR: numero di parametri sbagliato\n");
        exit(1);
    }
    //controllo i parametri
    int fd1;
    if((fd1 = open(argv[1], O_RDONLY)) < 0){
        printf("ERROR: durante l'apertura del primo parametro(%s)\n", argv[1]);
        exit(2);
    }
    if(strlen(argv[2]) != 1){
        printf("ERROR: il secondo parametro (%s) non corrisponde ad un carattere\n", argv[2]);
        exit(3);
    }
    char caracter = argv[2][0];
    //visualizzo su stdout
    printf("Nome eseguibile = %s -- Nome file = %s -- Carattere selezionato = %c\n", argv[0], argv[1], caracter);
    //cerco le occorrenze del carattere nel file 
    long counter = 0;
    char c;
    while (read(fd1, &c, 1)){
        if(caracter == c){
            counter++;
        }
    }
    //stampo il risultato
    printf("Nel file %s sono state trovate n°%lu occorrenze del carattere %c\n", argv[1], counter, caracter);
    exit(0);
}