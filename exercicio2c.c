#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20


unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
	  h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
	FILE* f = fopen(arquivo, "r");
	
	string* strings = (string *) malloc(sizeof(string) * n);

	for (int i = 0; !feof(f); i++) {
		strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
		fscanf(f, "%s\n", strings[i]);
	}

	fclose(f);

	return strings;
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

unsigned h_div(unsigned x, unsigned i, unsigned B)
{
	return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
	const double A = 0.6180;
	return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}
/*
===================
Tad lista
===================
*/
typedef struct _no {
	string data;
	struct _no *prox;
} no;

string no_data(no *in) {
	return(in->data);
}

no *no_prox(no *in) {
	return(in->prox);
}

no *no_criar(string data) {
	no *out = (no *) malloc(sizeof(no));
	out->data = (string) malloc(sizeof(char) * MAX_STRING_LEN);
	strcpy(out->data, data);
	out->prox = NULL;
	
	return (out);
}

unsigned no_push(no *in, string data, unsigned cont) {
	if (in == NULL) {
		in = no_criar(data);
		return(cont);
	}
	else {
		no_push(in->prox, data, cont+1);
	}
}

void no_apagar(no *in, no *anterior) {
	free(in->data);
	free(in);
	anterior->prox = NULL;
}

void no_limpar(no *in) {
	if (in->prox != NULL) no_limpar(in->prox);
	free(in);
}

bool no_encontrar(no *in, string data) {
	printf("woosh %s", in->data);
	while (in->prox != NULL) {
		if (strcmp(in->data, data) == 0) {
			return(TRUE);
		}
		in = in->prox;
	}
	return(FALSE);
}

/*
===================
Tad hash
===================
*/

//Estrutura da tabela hash
typedef struct _hash {
	no **vet;  //Vetor de listas
	unsigned tam; //Tamanho da tabela
} hash;

//Aloca uma tabela hash e retorna seu ponteiro
hash *criar_hash (unsigned N) {
	//Aloca a estrutura
	hash *out = (hash *)malloc(sizeof(hash));
	//Alocar o vetor de strings
	out->vet = (no **)malloc(sizeof(no*)*N);
	
	//Alocar (e inicializar) cada string do vetor
	for (unsigned cont = 0; cont < N; cont++) {
		out->vet[cont] = NULL;
	}
	
	//Guardar tamanho
	out->tam = N;
	//Retornar ponteiro para estrutura alocada.
	return(out);
}

//Insere uma chave em uma tabela hash, utilizando parametro e funcao hash dados. Retorna o numero de colisoes
unsigned inserir_hash (hash *in, string chave, unsigned (*func)(unsigned, unsigned, unsigned), int B) {
	unsigned chave_num = converter(chave);
	unsigned indice = func(chave_num, 0, B);
	
	//Colocar a chave na lista
	unsigned cont = no_push(in->vet[indice], chave, 0);
	
	//Retornar o numero de colisoes
	return(cont);
}

//Busca uma chave em uma tabela hash, utilizando parametro e funcao hash dada. Retorna TRUE caso encontre, FALSE caso contrario.
bool busca_hash (hash *in, string chave, unsigned (*func)(unsigned, unsigned, unsigned), int B) {
	unsigned chave_num = converter(chave);
	unsigned indice = func(chave_num, 0, B);
	
	return(no_encontrar(in->vet[indice], chave));
}

//Desaloca a estrutura completamente
void limpa_hash (hash *in) {
	for (unsigned cont = 0; cont < in->tam; cont++) free(in->vet[cont]);
	free(in->vet);
	free(in);
}

/*
===================
Main
===================
*/

int main(int argc, char const *argv[])
{
	unsigned N = 50000;
	unsigned M = 70000;
	unsigned B = 150001;

	unsigned colisoes_h_div = 0;
	unsigned colisoes_h_mul = 0;

	unsigned encontrados_h_div = 0;
	unsigned encontrados_h_mul = 0;

	string* insercoes = ler_strings("strings_entrada.txt", N);
	string* consultas = ler_strings("strings_busca.txt", M);


	hash *tabela_div = criar_hash(B);// cria tabela hash com hash por divisão

	// inserção dos dados na tabela hash usando hash por divisão
	inicia_tempo();
	for (unsigned i = 0; i < N; i++) {
		colisoes_h_div += inserir_hash(tabela_div, insercoes[i], h_div, B); // inserir insercoes[i] na tabela hash
	}
	double tempo_insercao_h_div = finaliza_tempo();

	// consulta dos dados na tabela hash usando hash por divisão
	inicia_tempo();
	for (unsigned i = 0; i < M; i++) {
		encontrados_h_div += busca_hash(tabela_div, consultas[i], h_div, B); // buscar consultas[i] na tabela hash
	}
	double tempo_busca_h_div = finaliza_tempo();

	limpa_hash(tabela_div); // limpa a tabela hash com hash por divisão


	hash *tabela_mul = criar_hash(B); // cria tabela hash com hash por divisão

	// inserção dos dados na tabela hash usando hash por multiplicação
	inicia_tempo();
	for (unsigned i = 0; i < N; i++) {
		colisoes_h_mul += inserir_hash(tabela_mul, insercoes[i], h_mul, B); // inserir insercoes[i] na tabela hash
	}
	double tempo_insercao_h_mul = finaliza_tempo();

	// busca dos dados na tabela hash com hash por multiplicação
	inicia_tempo();
	for (unsigned i = 0; i < M; i++) {
		encontrados_h_mul += busca_hash(tabela_mul, consultas[i], h_mul, B); // buscar consultas[i] na tabela hash
	}
	double tempo_busca_h_mul = finaliza_tempo();

	limpa_hash(tabela_mul); // limpa a tabela hash com hash por multiplicação



    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);
	
	//Já que as consultas e inserções foram alocadas com malloc, temos que desalocar também.
	for (unsigned cont = 0; cont < N; cont++) free(insercoes[cont]);
	free(insercoes);
	for (unsigned cont = 0; cont < M; cont++) free(consultas[cont]);
	free(consultas);

	return 0;
}
