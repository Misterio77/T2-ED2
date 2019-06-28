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
	unsigned indice, colisoes = 0;
	//repetir até encontrarmos uma posicao vazia
	for (unsigned cont = 0; !(strcmp(in->vet[indice], "")); cont++) {
		indice = (h_mul())
	}




	//Efetua o hash na chave, guardando o numero
	unsigned indice = func(converter(chave), 0, B);
	//Pegamos o resto da divisao pelo tamanho da tabela, para garantir que nao saimos dela
	indice = indice % in->tam;
	//Variavel para manter conta de quantas colisoes tivemos
	unsigned colisoes = 0;
	
	//Enquanto nao encontrarmos uma posicao vazia
	while (strcmp(in->vet[indice], "")) {
		colisoes++;				//Uma colisao nova
		indice++;				  //Overflow
		indice = indice % in->tam; //Caso ultrapasse o tamanho
	}
	
	//Copiar a chave na posicao do vetor
	strcpy(in->vet[indice], chave);
	//Retornar o numero de colisoes
	return(colisoes);
}

//Busca uma chave em uma tabela hash, utilizando parametro dado. Retorna TRUE caso encontre, FALSE caso contrario.
bool busca_hash (hash *in, string chave, int B) {
	//Efetua o hash na chave, guardando o numero
	unsigned indice = func(converter(chave), 0, B);
	//Pegamos o resto da divisao pelo tamanho da tabela, para garantir que nao saimos dela
	indice = indice % in->tam;
	//Vamos guardar o indice inicial, para comparar mais tarde
	unsigned indice_inicial = indice;
	
	do {
		//Caso a posicao atual seja a chave buscada (Negamos pois strcmp retorna 0 em caso positivo)
		if (!(strcmp(in->vet[indice], chave))) {
			//Encontramos
			return (TRUE);
		}
		indice++; //Overflow
		indice = indice % in->tam; //Caso ultrapasse o tamanho
	} while (indice_inicial != indice); //Continuar buscando até chegar no indice que começamos, nesse caso o elemento nao existe
	
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


	// cria tabela hash com hash por hash duplo

	// inserção dos dados na tabela hash
	inicia_tempo();
	for (int i = 0; i < N; i++) {
		// inserir insercoes[i] na tabela hash
	}
	double tempo_insercao = finaliza_tempo();

	// busca dos dados na tabela hash
	inicia_tempo();
	for (int i = 0; i < M; i++) {
		// buscar consultas[i] na tabela hash
	}
	double tempo_busca = finaliza_tempo();


	printf("Colisões na inserção: %d\n", colisoes);
	printf("Tempo de inserção   : %fs\n", tempo_insercao);
	printf("Tempo de busca	  : %fs\n", tempo_busca);
	printf("Itens encontrados   : %d\n", encontrados);

	//Já que as consultas e inserções foram alocadas com malloc, temos que desalocar também.
	for (unsigned cont = 0; cont < N; cont++) free(insercoes[cont]);
	free(insercoes);
	for (unsigned cont = 0; cont < M; cont++) free(consultas[cont]);
	free(consultas);


	return 0;
}
