#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
	//controllo il numero di parametri
	if(argc != 3){
		printf("ERROR: numero di parametri errato\n");
		exit(1);
	}
	//controllo i valori dei parametri
	int rf1;
	if((rf1 = open(argv[1]), O_ROLNY) != 0){
		printf("ERROR: primo parametro errato\n");
		exit(2);
	}
	int rf2;
	if((rf2 = strlen(argv[2])) != 1){
		printf("ERROR: secondo parametro errato\n");
		exit(3);
	}
	//visualizzo su stdout
	printf("%s\n", argv[0]);
	printf("%i\n", argc);
	
