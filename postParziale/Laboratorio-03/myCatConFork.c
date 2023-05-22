#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char ** argv){
    //controllo il numero di parametri
    if(argc != 2){
        printf("ERROR: numero di parametri errato\n");
        exit(1);
    }
    //credo un processo figlio
    int pid;
    if((pid = fork()) < 0 ){
        printf("ERROR: durante l'esecuzione della fork\n");
        exit(2);
    }
    if(pid == 0){
        //siamo nel processo figlio e implementiamo la ridirezione di stdin 
        close(0);
        open(argv[1], O_RDONLY);
        execl("/home/dc/Esercizi/postParziale/Laboratorio-03/mycat", argv[1], (char *)0);
        //se la exec genera errori allora ritorno -1 
        exit(-1);
    }
    //siamo nel padre
    int pidfiglio, status;
    if((pidfiglio = wait(&status)) < 0){
        printf("ERROR: durante la wait del processo figlio\n");
        exit(3);
    }
    if ((status & 0xFF) != 0){
    	printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidfiglio);
    }
    int ritorno = (int)((status >> 8) & 0xFF);
  	printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidfiglio, ritorno);
    exit(0);
}