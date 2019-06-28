#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int comparar (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

typedef struct _indice {
	int *ind;
	int tam;
} indice;

//Indexa o vetor dado (Só funciona com vetores completos com todos e sem repeticao)
indice *indexar(int *vet, int N, int S) {
	indice *out = (indice *)malloc(sizeof(indice));
	out->ind = (int *)malloc(sizeof(int)*N/S);
	
	for (int cont = 0; cont < N/S; cont++) {
		out->ind[cont] = cont*S;
	}
	out->tam = N/S;
	
	return (out);
}

bool busca_sequencial_indexada(int *vet, int tam, indice *i, int chave) {
	int cont, cont2;
	
	for (cont = 1; cont < i->tam; cont++) {
		if (i->ind[cont] >= chave) {
			cont2 = i->ind[cont-1];
			break;
		}
	}
	if (cont == i->tam) cont2 = i->ind[cont-1];
	
	for (; cont2 < tam; cont2++) {
		if (vet[cont2] == chave) {
			return(TRUE);
		}
	}
	
	return(FALSE);

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

int main(int argc, char const *argv[])
{
	const int N = 50000;
	unsigned encontrados = 0;

	int* entradas = ler_inteiros("inteiros_entrada.txt", N);
	int* consultas = ler_inteiros("inteiros_busca.txt", N);

	// realiza busca sequencial
	inicia_tempo();
	qsort(entradas, N, sizeof(int), comparar);
	indice *ind = indexar(entradas, N, 10000);
	for (int i = 0; i < N; i++) {
		//Busca o elemento atual
		if (busca_sequencial_indexada(entradas, N, ind, consultas[i]))
			encontrados++;//Caso encontre, adicione 1
	}
	double tempo_busca = finaliza_tempo();

	printf("Tempo de busca	:\t%fs\n", tempo_busca);
	printf("Itens encontrados :\t%d\n", encontrados);

	return 0;
}
