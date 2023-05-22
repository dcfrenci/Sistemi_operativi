#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MAX_LENGHT 512

int main(int argc, char ** argv){
    //controllo il numero di parametri
    if (argc != 2){
        printf("ERROR: numero di parametri errato, doveva essere solo uno (nome del file)\n");
        exit(1);
    }
    //controllo che il parametro sia un file
    int fd1; 
    if ((fd1 = open(argv[1], O_RDONLY)) < 0){
        printf("ERROR: durante l'apertura del file passato come parametro\n");
        exit(2);
    }
    //creo il figlio
    int pid;
    if ((pid = fork()) < 0){
        printf("ERROR: durante la creazione del figlio da parte della fork\n");
        exit(3);
    }
    //creo la pipe
    int piped[2];
    if (pipe(piped) < 0){
        printf("ERROR: durante la creazione della pipe\n");
    }
    char* mess = malloc(MAX_LENGHT * sizeof(char));
    int L = 0;
    if (pid == 0){
        //siamo nel figlio
        close(piped[0]);
        while (read(fd1, &(mess[L]), sizeof(char) != 0)){
            if (mess[L] == '\n' /*|| L == 511*/){
                mess[L] = 0;    //termino la stringa con lo zero
                printf("\n%s\n", mess);

        
                L++;
                write(piped[1], &L, sizeof(int));
                write(piped[1], mess, L * sizeof(char));
                L = 0; 
            }  else {
                L++;
            }
            printf("%d\n", L);
        }
    }
    //siamo nel padre
    close(piped[1]);
    int string = 0;
    /*L = 0;
    while(read(piped[0], &(mess[L]), sizeof(char))){
        if (L == k || L == 511){ 
            //ho terminato la stringa da leggere
            printf("%d: %s\n", string, mess);
            L = 0;
            read(piped[0], &k, sizeof(int));
            string++;
        } else {
            L++;
        }
    }*/
    while (read(piped[0], &L, sizeof(int)) > 0){
        read(piped[0], mess, L * sizeof(char));
        printf("%s\n", mess);
        string++;
    }

    printf("DEBUG-Padre %d letto %d messaggi dalla pipe\n", getpid(), string);
	/* padre aspetta il figlio */
    int pidFiglio, status, ritorno;
	if ((pidFiglio=wait(&status)) < 0)
	{
      		printf("Errore wait\n");
      		exit(4);
	}
	if ((status & 0xFF) != 0)
        	printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
	else
	{
       		ritorno=(int)((status >> 8) & 0xFF);
       		printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\n", pidFiglio, ritorno);
	}

	exit(0);
}