

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
for (i = 0; i < M; i++){
    close(piped[i][1]);
}