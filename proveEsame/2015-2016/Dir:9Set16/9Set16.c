#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

typedef int pipe_t[2];
struct dati {
    char carattere;
    long int occorrenze;
} typedef dati;

void scambia(dati *v1, dati *v2){
    dati elem = *v1;
    *v1 = *v2;
    *v2 = elem;
}
void scambiaInt(int *v1, int *v2){
    int elem = *v1;
    *v1 = *v2;
    *v2 = elem;
}

void bubbleSort(dati v[], int dim, int pidArray[]){
    bool ordinato = false;
    while (dim > 1 && !ordinato){
        ordinato = true;
        for (int i = 0; i < dim - 1; i++){
            if (v[i].occorrenze > v[i + 1].occorrenze){
                scambia(&v[i], &v[i + 1]);
                scambiaInt(&pidArray[i], &pidArray[i + 1]);
                ordinato = false;
            }
        }
        dim--;
    }
}

int main(int argc, char ** argv){
    //controllo il numero di parametri
    int nParametriMin = 2;
    if (argc != nParametriMin){
        printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
        exit(1);
    }

    int N = 26;         //numero di figli da generare
    int M = N;          //numero di pipeline da generare
    //alloco la memoria per la pipe
    pipe_t* piped = (pipe_t *) malloc(M * sizeof(pipe_t));
    if (piped == NULL){
        printf("ERROR: durante l'allocazione della memoria per la pipe\n");
        exit(4);
    }
    //creo M pipe
    for (int j = 0; j < M; j++){
        if (pipe(piped[j]) < 0){
            printf("ERROR: durante la creazione della pipe\n");
            exit(5);
        }
    }
    //creo il vettore del pid
    int pidArray[26];
    //genero i figli
    for (int i = 0; i < N; i++){
        int pid;
        if ((pid = fork()) < 0){
            printf("ERROR: durante la fork\n");
            exit(3);
        }
        if (pid == 0){
            //siamo nel figlio
            //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
            for (int j = 0; j < M; j++) {                               
                if (i != j) close(piped[j][1]);                 //chiudo tutti i lati in scrittura meno quello relativo all'indice del figlio
                if (i == 0 || j != i - 1) close(piped[j][0]);   //chiudo tutti i lati in lettura meno quello del figlio precedente se presente
            }
            //apro il file e controllo che sia stato aperto correttamente
            int fd;
            if ((fd = open(argv[1], O_RDONLY)) < 0){
                printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
                exit(2);
            }
            //leggo il file e cerco le occorrenze del carattere 'a' + i
            char toFind = 'a' + i;
            char carattere;
            long int occorrenze = 0;
            while (read(fd, &carattere, sizeof(char)) != 0){
                if (carattere == toFind) occorrenze++;
            }
            //carico i dati sull'array recuperato e lo scrivo sulla pipe
            dati arr[26];
            if (i != 0){
                read(piped[i - 1][0], arr, sizeof(dati) * 26);
            }
            arr[i].carattere = toFind;
            arr[i].occorrenze = occorrenze;
            write(piped[i][1], arr, sizeof(dati) * 26);
            //inserisco il pid del figlio nell'array
            exit(carattere);
        }
        pidArray[i] = pid;
    }

    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);                     //chiudo tutti i lati di scrittura sulle pipe
        if (i == M - 1) close(piped[i][1]);     //chiudo tutti i lati di lettura meno quello dell'ultimo figlio
    }
    //ottengo l'array dall'ultimo figlio
    dati arr[26];
    read(piped[N - 1][0], arr, sizeof(dati) * 26);
    //ordino
    bubbleSort(arr, 26, pidArray);
    //riporto su stdout 
    printf("Occorrenze ogni carattere nel file %s\n", argv[1]);
    for (int i = 0; i < N; i++){
        printf("Il processo con pid = %d per il carattere %c sono state trovate %ld occorrenze\n", pidArray[i], arr[i].carattere, arr[i].occorrenze);
    }
    //aspetto che tutti i processi terminino
    for (int i = 0; i < N; i++){
        int pidFiglio, status;
        if ((pidFiglio = wait(&status)) < 0){
            printf("ERROR: durante la wait nel padre del figlio\n");
            exit(6);
        }
        if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else {
            int ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %c (%d se 255 problemi!)\n", pidFiglio, ritorno, ritorno);
        }
    }
    exit(0);
}