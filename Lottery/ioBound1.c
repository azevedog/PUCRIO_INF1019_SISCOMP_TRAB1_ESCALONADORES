/*
Programa focado em chamadas de I/O que necessita de 25 segundos para fazer sua tarefa.

@Autores:
Gustavo B H Azevedo - 1321442
Maria Carolina Santos - 1312063
*/
#include<stdio.h>
#include <time.h>
#include <stdlib.h>

// Tempo de trabalho.
#define END 25

// Estrutura de inicio da contagem do tempo.
struct timespec start;

// Estrutura para avaliar o tempo no momento.
struct timespec now;

// Funcao principal de trabalho. 42 indica conclusao com sucesso.
int main (void){
	double diff;
	int i =1;
	/*FILE* output;

	if ((output = fopen ("./saida.txt", "a+")) == NULL){
		printf("Erro ao abrir o arquivo de output!\n");
		exit(-1);
	}*/

	//fprintf(output, "ioBound1.c started\n");
	printf("ioBound1.c started\n");

	clock_gettime(CLOCK_MONOTONIC, &start);
	do{
		clock_gettime(CLOCK_MONOTONIC, &now);
		diff = (now.tv_sec - start.tv_sec);
		if(diff>=i){
			//fprintf(output, "IO1 - Working - %dseg\n",i);
			printf("IO1 - Working - %dseg\n",i);
			i+=1;
		}
	}while(diff<END);

	//fprintf(output, "ioBound1.c finished\n");
	printf("ioBound1.c finished\n");
	
	//fclose(output);

	return 42;
}
		


	

