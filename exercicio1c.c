/*
SCC0224 - Estrutura de Dados 2
Projeto 2 - Exercício 1
06/2019

Gabriel Silva Fontes		10856803
Rafaela Delgado Nascimento  10818904
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

void troca(int *a, int *b) {
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int* ler_inteiros(const char * arquivo, const int n)
{
	FILE* f = fopen(arquivo, "r");

	int * inteiros = (int *) malloc(sizeof(int) * n);

	for (int i = 0; !feof(f); i++)
		fscanf(f, "%d\n", &inteiros[i]);

	fclose(f);

	return inteiros;
}

void inicia_tempo()
{
	srand(time(NULL));
	_ini = clock();
}

double finaliza_tempo()
{
	_fim = clock();
	return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}



//Busca sequencial com transposicao
bool busca_sequencial_transp(int *vet, int tam, int chave) {
	for (int cont = 0; cont < tam; cont++) {
		if (vet[cont] == chave) {//Caso encontre
			if (cont != 0) troca(&vet[cont], &vet[cont-1]);
			return (TRUE);	
		}
	}
	return (FALSE);
}

int main(int argc, char const *argv[])
{
	const int N = 50000;
	unsigned encontrados = 0;

	int* entradas = ler_inteiros("inteiros_entrada.txt", N);
	int* consultas = ler_inteiros("inteiros_busca.txt", N);

	// realiza busca sequencial
	inicia_tempo();
	for (int i = 0; i < N; i++) {
		//Busca o elemento atual
		if (busca_sequencial_transp(entradas, N, consultas[i]))
			encontrados++;//Caso encontre, adicione 1
	}
	double tempo_busca = finaliza_tempo();

	printf("Tempo de busca	:\t%fs\n", tempo_busca);
	printf("Itens encontrados :\t%d\n", encontrados);

	return 0;
}
