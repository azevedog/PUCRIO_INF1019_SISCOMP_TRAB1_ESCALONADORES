/*
Escalonador com politica de Round Robin

@Autores:
Gustavo B H Azevedo - 1321442
Maria Carolina Santos - 1312063
*/
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "contractRound.h"

//Tempo de execucao de um programa
#define QUANTUM 1

//Identificador area de memoria do PID deste processo
int ipcid_PID_ESC;

//Identificador area de memoria do PID do processo interpretador
int ipcid_PID_INT;

//Identificador area de memoria compartilhada para o nome do programa
int ipcid_TRANSF;

//Proximo indice vazio.
int nextEmptyIndex = 0;

//Id deste processo
int* pID_ESC;

//Vetor de identificadores de procesos ativos
int* pID_VET;

//Id do processo escalonador
int* pID_INT = NULL;

//Vetor de caracteres do nome do programa (memoria compartilhada)
char* transfAREA;

//Vetor de caracteres do nome do programa (memoria local)
char localAREA[MAX_];

//Arquivo de saída para escrita de stdout
FILE* outputFILE;

//Inicializa o vetor de processos.
void startPIDVET(){
	pID_VET = (int*) malloc(sizeof(int)*MAX_);
	int i;
	for (i=0; i<MAX_; i++){
		//Codigo para posicao vazia
		pID_VET[i]=-1;
	}
}

//Rotina de limpeza para encerramento
void exitRoutine(){
	shmdt(pID_ESC);
	shmctl(ipcid_PID_ESC, IPC_RMID, 0);
	shmdt(pID_INT);
	shmctl(ipcid_PID_INT, IPC_RMID, 0);
	shmdt(transfAREA);
	shmctl(ipcid_TRANSF, IPC_RMID, 0);
	free(pID_VET);
	fclose(outputFILE);
}

//Aguarda sinal de dados escritos iniciar escalonamento
void usr1Handler(int sinal){
	if(transfAREA==NULL){
		ipcid_TRANSF = shmget(TRANSF_KEY, sizeof(char*)*MAX_, S_IRUSR|S_IWUSR);
		transfAREA = (char*) shmat(ipcid_TRANSF, 0, 0); 
	}

	int i;
	for(i=0;i<MAX_; i++){
		localAREA[i]=transfAREA[i];
	}


	int f = fork();
	if(f!=0){
		kill(f, SIGSTOP);
		printf("ESC: Recebido (%s), pid para o novo processo: %d\n", localAREA,f);
		pID_VET[nextEmptyIndex++]=f;
		if(pID_INT==NULL){
			ipcid_PID_INT = shmget(INT_KEY, sizeof(int), S_IRUSR|S_IWUSR);
			pID_INT = (int*) shmat(ipcid_PID_INT, 0, 0);
		}
		
		kill(*pID_INT, SIGUSR2);
				
	}else{
		char* s[] = {localAREA, NULL};
		execve(localAREA, s ,0);
	}


	
}

void sigHandler(int sinal){
	printf("TOMEI EXIT %d\n", sinal);
	exitRoutine();
	exit(42);
}

// Cadastramento do tratador de sinal para liberar recursos
void registerHandler(){
	void (*p) (int);
	p = signal (SIGUSR1, usr1Handler);
	if(p == SIG_ERR){
		printf("Erro de registro SIGUSR1\n");
		exit(-1);
	}
	p = signal (SIGSEGV, sigHandler);
	if(p == SIG_ERR){
		printf("Erro de registro SIGSEGV\n");
		exit(-1);
	}
	p = signal (SIGQUIT, sigHandler);
	if(p == SIG_ERR){
		printf("Erro de registro SIGQUIT\n");
		exit(-1);
	}
	p = signal (SIGTERM, sigHandler);
	if(p == SIG_ERR){
		printf("Erro de registro SIGTERM\n");
		exit(-1);
	}
}

// Funcao principal de trabalho. 42 indica conclusao com sucesso
int main (void){
	
	//Cadastramento do PID em memoria
	ipcid_PID_ESC = shmget(ESC_KEY, sizeof(int), IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR);
	pID_ESC = (int*) shmat(ipcid_PID_ESC, 0, 0); 
	*pID_ESC = getpid();
	
	// Cadastramento do tratador de sinal para liberar recursos
	registerHandler();
	
	//Inicializacao do vetor de processos
	startPIDVET();

	if ((outputFILE = fopen ("./saida.txt", "a+")) == NULL){
		printf("Erro ao abrir o arquivo de outputFILE!\n");
		exit(-1);
	}
	
	while(1){
		int i;
		for(i=0; i<nextEmptyIndex; i++){
			int pid = pID_VET[i];
			if(pid!=-1){
				int status;
				pid_t result = waitpid(pid, &status, WNOHANG);
				if (result == 0){
				printf("ESC: CONT: (%d)\n", pid);
					kill(pid, SIGCONT);
					sleep(QUANTUM);
					kill(pid, SIGSTOP);	
				printf("ESC: STOP: (%d)\n", pid);
				}else{
					pID_VET[i]=-1;
				}
			}
		}
	}
	return 42;
}
