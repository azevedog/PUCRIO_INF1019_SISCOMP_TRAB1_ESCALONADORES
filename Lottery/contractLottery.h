/*
Contrato de comunicacao

@Autores:
Gustavo B H Azevedo - 1321442
Maria Carolina Santos - 1312063
*/

// Numero maximo de programas de entrada e tamanho maximo do nome dos programas de entrada
#define MAX_ 25

//Numero maximo de tickets que um processo pode ter
#define MAX_TICK 4

// Chave acordada entre Interpretador e escalonador para chave do PID do Escalonador
key_t ESC_KEY = 2115;

// Chave acordada entre Interpretador e escalonador para chave do PID do Interpretador
key_t INT_KEY = 2116;

// Chave acordada entre Interpretador e escalonador para dados de transferencia (NOME)
key_t TRANSF_NAME_KEY = 2117;

// Chave acordada entre Interpretador e escalonador para dados de transferencia (Tickets)
key_t TRANSF_TICK_KEY = 2118;
