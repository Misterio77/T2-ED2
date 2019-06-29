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
Tad hash
===================
*/

//Estrutura da tabela hash
typedef struct _hash {
	string *vet;  //Vetor de strings armazenadas
	unsigned tam; //Tamanho da tabela
} hash;

//Aloca uma tabela hash e retorna seu ponteiro
hash *criar_hash (unsigned N) {
	//Aloca a estrutura
	hash *out = (hash *)malloc(sizeof(hash));
	//Alocar o vetor de strings
	out->vet = (string *)malloc(sizeof(string)*N);
	
	//Alocar (e inicializar) cada string do vetor
	for (unsigned cont = 0; cont < N; cont++) {
		out->vet[cont] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
		//Usamos strcpy ao inves de uma atribuicao, para que a string seja mutavel mais tarde
		strcpy(out->vet[cont], "");
	}
	//Guardar tamanho
	out->tam = N;
	//Retornar ponteiro para estrutura alocada.
	return(out);
}

//Insere uma chave em uma tabela hash, utilizando parametro dado. Retorna o numero de colisoes
unsigned inserir_hash (hash *in, string chave, int B) {
	unsigned chave_num = converter(chave);
	unsigned indice, cont;
	
	for (cont = 0; cont < in->tam; cont++) {
		indice = (h_mul(chave_num, cont, B) + cont * h_div(chave_num, cont, B)) % B;
		if (strcmp(in->vet[indice], "") == 0) {
			break;
		}	
	}
	
	//Copiar a chave na posicao do vetor
	strcpy(in->vet[indice], chave);
	//Retornar o numero de colisoes
	return(cont);
}

//Busca uma chave em uma tabela hash, utilizando parametro dado. Retorna TRUE caso encontre, FALSE caso contrario.
bool busca_hash (hash *in, string chave, int B) {
	unsigned chave_num = converter(chave);
	unsigned indice;
	
	for (unsigned cont = 0; cont < in->tam; cont++) {
		indice = (h_mul(chave_num, cont, B) + cont * h_div(chave_num, cont, B)) % B;
		if (strcmp(in->vet[indice], "") == 0) break;
		if (strcmp(in->vet[indice], chave) == 0) {
			return(TRUE);
		}
	}	
	//Não encontramos
	return(FALSE);
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
	const int N = 50000;
	const int M = 70000;
	const int B = 150001;

	unsigned colisoes = 0;
	unsigned encontrados = 0;

	string* insercoes = ler_strings("strings_entrada.txt", N);
	string* consultas = ler_strings("strings_busca.txt", M);


	hash *tabela = criar_hash(B); // cria tabela hash com hash por hash duplo

	// inserção dos dados na tabela hash
	inicia_tempo();
	for (int i = 0; i < N; i++) {
		colisoes += inserir_hash(tabela, insercoes[i], B); // inserir insercoes[i] na tabela hash
	}
	double tempo_insercao = finaliza_tempo();

	// busca dos dados na tabela hash
	inicia_tempo();
	for (int i = 0; i < M; i++) {
		encontrados += busca_hash(tabela, consultas[i], B);// buscar consultas[i] na tabela hash
	}
	double tempo_busca = finaliza_tempo();


	limpa_hash(tabela);
	
	printf("Colisões na inserção: %d\n", colisoes);
	printf("Tempo de inserção   : %fs\n", tempo_insercao);
	printf("Tempo de busca	  : %fs\n", tempo_busca);
	printf("Itens encontrados   : %d\n", encontrados);

	//Já que as consultas e inserções foram alocadas com malloc, temos que desalocar também.
	for (int cont = 0; cont < N; cont++) free(insercoes[cont]);
	free(insercoes);
	for (int cont = 0; cont < M; cont++) free(consultas[cont]);
	free(consultas);


	return 0;
}
