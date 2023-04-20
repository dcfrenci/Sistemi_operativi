#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char ** argv){
	if(argc != 4){
		printf("Numero di parametri errato\n");
		exit(1);
	}
	int fd1;
	if((fd1 = open(argv[1], O_RDONLY)) < 0){
		printf("ERROR: il parametro corrispondente al file non è corretto\n");
		exit(2);
	}
	int fd2;
	if((fd2 = atoi(argv[2]) <= 0)){
		printf("ERROR: il parametro corrispondente al numero è <= 0\n");
		exit(3);
	}
	char fd3;
	if((fd3 = strlen(argv[3])) > 1){
		printf("ERROR: il parametro corrispondente al carattere è più di uno\n");
		exit(4);
	}
	//visualizzo il nome dell'eseguibile e il numero di parametri
	printf("%s\n", argv[0]);
	printf("%i\n", argc);
	//visualizzo il contenuto dei parametri
	printf("FILE: %s\n", argv[1]);
	printf("NUMERO: %s\n", argv[2]);
	printf("CARATTERE: %s\n", argv[3]);
	exit(0);
}
