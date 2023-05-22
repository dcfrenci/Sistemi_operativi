#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char ** argv){
    //controllo il numero di parametri 
    if(argc < 3){
        printf("ERROR: numero di parametri insufficiente (< 3)\n");
        exit(1);
    }
    int N = argc - 1;
    //controllo che i parametri siano file
    for (int i = 0; i < N; i++){
        int fd;
        if ((fd = open(argv[N + 1], O_RDONLY)) < 0){
            printf("ERROR: il parametro %s non è un file\n", argv[N + 1]);
            exit(2);
        }
    }
    //creo le pipes
    int pipeChar[2];
    int pipeNumber[2];
    if(pipe(pipeChar) < 0){
        printf("ERROR: durante la creazione della pipe per i caratteri alfabetici\n");
        exit(3);
    }
    if(pipe(pipeNumber) < 0){
        printf("ERROR: durante la creazione della pipe per i caratteri numerici\n");
        exit(4);
    }
    //genera i figli
    int pid;
    for (int i = 0; i < N; i++){
        if((pid = fork()) < 0){
            printf("ERROR: nella creazione del figlio per il file %s\n", argv[N + 1]);
            exit(5);
        }
        if(pid == 0){
            //siamo nel figlio
            close(pipeChar[0]);
            close(pipeNumber[0]);
            char carattere;
            int fd = open(argv[i + 1], O_RDONLY);
            while(read(fd, &carattere, sizeof(char)) != 0){
                if (i % 2 == 0 || i == 0){
                    //leggo solo caratteri alfabetici
                    if (isalpha(carattere) != 0){
                        write(pipeChar[1], &carattere, sizeof(char));
                    }
                } else {
                    //leggo solo caratteri alfabetici
                    if (isalnum(carattere) != 0){
                        write(pipeNumber[1], &carattere, sizeof(char));
                    }
                }
            }
            exit(carattere);
        }
    }
    //siamo nel padre
    close(pipeChar[1]);
    close(pipeNumber[1]);
    //stampo i caratteri alternati
    char alpha = 0, number = 0, totalChar = 0;
    while ((read(pipeChar[0], alpha, sizeof(char)) != 0) || (read(pipeNumber[0], number, sizeof(char)) != 0)){
        if (alpha != 0){
            fprintf("%c", alpha);
            alpha = 0;
            totalChar++;
        }
        if (number != 0){
            fprintf("%c", number);
            number = 0;
            totalChar++;
        }
    }
    //il padre aspetta i figli
    for (int i = 0; i < N; i++){
        int pidFiglio, status;
        if (wait(&status) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
    	else {
      		int ritorno = (int)((status >> 8) & 0xFF);
    		printf("Il figlio con pid=%d ha ritornato %c (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
    	}
    }
}