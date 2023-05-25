#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
typedef int pipe_t[2];

//funzione con la quale verifico se esiste almeno un figlio da cui si deve ancora leggere
bool finish(int *terminati, int N){
    for (int i = 0; i < N; i++){
        if (terminati[i] == 0) return true;     //esiste almeno un figlio da cui leggere
    }
    return false;   //non esistono figli da cui leggere
}

int main(int argc, char ** argv){
    //controllo il numero di parametri
    int nParametriMin = 3;
    if (argc < nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d", nParametriMin, argc);
        exit(1);
    }
    int N = argc - 1;
    //creo un array di pipe per gli N figli
    int M = N;
    //alloco la memoria per la pipe
    pipe_t* piped = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (piped == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(2);
    }
    //creo M pipe
    for (int j = 0; j < M; j++){
        if (pipe(piped[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(3);
        }
    }
    //genero i figli
    for (int n = 0; n < N; n++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(4);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                close(piped[j][0]);
                if (n != j) close(piped[j][1]);
            }
            //apro il parmetro n° verificando che sia un file nella sua apertura
            int fdw;
            if ((fdw = open(argv[n + 1], O_RDONLY)) < 0){
                printf("ERROR: il parametro %s non è un file o non può essere aperto in lettura\n", argv[n + 1]);
                exit(5);
            }
            char chs[2];            //array di caratteri su cui scrivo i caratteri letti dal file
            int nCharLetti = 0;     //counter per i caratteri letti dal file n + 1
            while (1){
                //controllo che vengano letti dei caratteri. Se il file termina scrivo 0 sulla pipe
                if (read(fdw, &chs[0], sizeof(char)) == 0){
                    chs[0] = 0;
                    write(piped[n][1], &chs[0], sizeof(char));
                    break;
                }
                if (read(fdw, &chs[1], sizeof(char)) == 0){ 
                    chs[1] = 0;      //se il file non contiene un numero pari di caratteri assegno all'ultimo char 0
                    nCharLetti--;
                }
                //scrivo sulla pipe i char letti
                write(piped[n][1], &chs[0], sizeof(char));
                //se il secondo carattere è lo zero termi
                write(piped[n][1], &chs[0], sizeof(char));
                //aggiorno il counter
                nCharLetti += 2;
            }
            exit(nCharLetti);
        }
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);
    }
    //inizializzo l'array di char letti dal padre. Avrà dimensione massima 255 * N poiché si suppone che per ogni file non ci siano più di 255 caratteri.
    char *letti = calloc(254 * N, sizeof(char));
    if (letti == NULL){
        printf("ERROR: durante l'allocazione di memoria per l'array\n");
        exit(6);
    }
    int *terminati = calloc(N, sizeof(int));
    if (terminati == NULL){
        printf("ERROR: durante l'allocazione di memoria per l'array\n");
        exit(6);
    }
    int pos = 0;
    //il while terminana quando tutti i figli hanno finito di scrivere (hanno ritornato uno 0 sul primo o secondo carattere)
    while (finish(terminati, N)){
        for (int n = 0; n < N; n++){
            //controllo se per quel figlio ho finito di leggere
            if (terminati[n] != 0) continue;
            //leggo dal figlio
            read(piped[n][0], &letti[pos], sizeof(char));
            pos++;
            read(piped[n][0], &letti[pos], sizeof(char));
            //se il secondo char è uno zero allora ho terminato di leggere da quel figlio
            if (letti[pos] == 0 || letti[pos - 1] == 0){
                terminati[n] = 1;
                if (letti[pos - 1] == 0) pos--;
            } else {
                pos++;
            }
        }
    }
    //tutti i figli hanno finito di leggere i file. Stampo i caratteri letti
    printf("I figli hanno letto i seguenti caratteri:\n");
    for (int i = 0; i < pos + 1; i++){
        printf("%c", letti[i]);
    }
    printf("\n");
    //faccio la wait per tutti i figli
    for (int n = 0; n < N; n++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    free(terminati);
    free(letti);
    exit(0);
}