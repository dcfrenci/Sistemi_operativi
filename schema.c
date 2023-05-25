si SCRIVE IN PIPED[1]
si LEGGE IN PIPED[0]

/*
LIBRERIE:
---------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <ctype.h>
#include <string.h>
#include <stdbool.h>
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
MAIN CON PARAMETRI (argc = parametri + 1, primo parametro in argv[1])
---------------------------------------------------------------------
int main(int argc, char** argv){

    exit(0);
}
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO del NUMERO DI PARAMETRI
---------------------------------
//controllo il numero di parametri
int nParametriMin = DICHIARARE;
if (argc < nParametriMin){
    printf("ERROR: numero di parametri insufficiente. Dovevano essere %d ma erano %d\n", nParametriMin, argc);
    exit(1);
}
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO se i parametri sono FILE E SI POSSANO APRIRE (N = numero di file da controllare)
------------------------------------------------------------------------------------------
int N = DICHIARARE;
for (int i = 0; i < N; i++){
        int fd;
        if ((fd = open(argv[i + 1], O_RDONLY)) < 0){
            printf("ERROR: durante l'apertura poiché il parametro %s non è un file o non può essere aperto in lettura\n", argv[i + 1]);
            exit(2);
        }
    }
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO se i parametri sono DIRECTORY E SI POSSONO APRIRE (N = numero di directory da controllare)
----------------------------------------------------------------------------------------------------

*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO sulla creazione di FORK
---------------------------------
int pid;
if ((pid = fork()) < 0){
    printf("ERROR: durante la fork\n");
    exit(3);
}
if (pid == 0){
    //siamo nel figlio
    
}
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO sulla generazione di una PIPE
---------------------------------------
int piped[2];
if (pipe(piped) < 0){
    printf("ERROR: durante la creazioned della pipe\n");
    exit(4);
}
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO sulla generazione di un PIPE di DIMENSIONE M = numero di pipe
-----------------------------------------------------------------------
typedef int pipe_t[2];

int M = N;
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
    
    //chiudo i lati non usati per la comunicazione con il padre (nel figlio)
    for (int j = 0; j < M; j++) {                               
	close(piped[j][0]);
	if (i != j) close(piped[j][1]);
    }
    
    //chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
    for (int i = 0; i < M; i++){
        close(piped[i][1]);
    }
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO sul valore di ritorno della WAIT
------------------------------------------
int pidFiglio, status;
if ((pidFiglio = wait(&status)) < 0){
    printf("ERROR: durante la wait nel padre del figlio\n");
    exit(6);
}
if ((status & 0xFF) != 0) printf("ERROR: Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
else {
    int ritorno = (int)((status >> 8) & 0xFF);
    printf("Il figlio con pid=%d ha ritornato %c (%d se guaale 255 --> problemi!)\n", pidFiglio, ritorno, ritorno);
}
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO sul valore di ritorno della CREAT
-------------------------------------------
int fd;
    if ((fd = creat(NOME_DEL_FILE_DA_CREARE, O_WRONLY)) == - 1){
        printf("ERROR: durante la creazione del file\n");
    }
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
CONTROLLO sull'allocazione di memoria per L'ARRAY
-------------------------------------------------
if (DICHIARARE == NULL){
        printf("ERROR: durante l'allocazione di memoria per l'array\n");
        exit(6);
    }
*/

/*🔽️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔸️🔽️
FUNZIONE per verificare se c'è ALMENO UN ELEMENTO DIVERSO DA ZERO --> true
--------------------------------------------------------------------------
bool finish(int * child, int dim){
    for (int i = 0; i < dim; i++){
        if (child[i] == 0) return true;
    }
    return false;
}
*/

/*
=========================================================================
STRUTTURE USATE SPESSO:
=========================================================================
*/

/*🔷️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔹️🔷️
GENERO N° FIGLI ED N° PIPE
--------------------------

typedef int pipe_t[2];

int M = N;		//numero di pipe da creare
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
            close(piped[j][0]);
            if (i != j) close(piped[j][1]);
        }
    }
}

//chiudo i lati non usati per ricevere informazioni dal figlio (nel padre)
for (int i = 0; i < M; i++){
    close(piped[i][1]);
}
*/
