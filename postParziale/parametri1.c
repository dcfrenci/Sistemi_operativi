#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	//controllo il numero di parametri
	if(argc < 2){
		printf("Numero di parametri errato\n");
		exit(1);
	}
	//stampo il nome del file
	printf("%s\n", argv[0]);
	printf("%i", argc);
	//stampo i parametri
	int i;
	for(i = 0; i < argc - 1; i++){
		printf("Il parametro %i contiene %s\n", i + 1, argv[i + 1]);
	}
	exit(0);
}
