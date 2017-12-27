//=====================================================
// Arquivo: main.c
//
// Autor: Guilherme Cardoso Silva 0022545
// Data: 27/07/2016
// Disciplina: Programacao II
// Atividade: Trabalho 2 
// Para compilar é necessario utilizar -lm para ler a biblioteca math.h
//=====================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define EXECUCOES 3
//Na compilação definir estas constantes com -D"Constante"="valor"
  //#define TAM;
  //#define VIDEO;

struct Dado2{
	double *tempo;
	double *comparacoes;
	double *trocas;
};
typedef struct Dado2 *VDados;

struct Dado{
	double tempo;
  double comparacoes;
  double trocas;
};
typedef struct Dado *Dados;

void MetodosOrdencacao(int metodo, FILE* arquivo, unsigned long int* vetor, unsigned long int* copia, Dados PDados, VDados PVDados);
void destroiVetores(unsigned long int *vetor, unsigned long int *copia, Dados PDados, VDados PVDados);
void preencheVetor (unsigned long int *vetor, int t);
void copiaVetor(unsigned long int *copia, unsigned long int *vetor, int t);
double calculaDesvio(double* vetor, int tam, double media);
double calculaMedia(double* vetor, int tam);
int ComparacaoVal1MaiorVal2(unsigned long int val1, unsigned long int val2, double *comparacoes);
void QtdComparacoes(double *comparacoes);
void TrocaVal1ComVal2(unsigned long int *val1, unsigned long int *val2, double *trocas);
void QtdTrocas(double *trocas);
void Bolha(unsigned long int *copia, int t, Dados PDados);
void Insercao(unsigned long int *copia, int t, Dados PDados);
int Particao(unsigned long int *copia, int inicio, int fim, double *comparacoes, double *trocas);
void Quicksort(unsigned long int *copia, int inicio, int fim, Dados PDados);
void Ordena(unsigned long int *copia, int inicio, int fim, double *comparacoes, double *trocas);
void QuicksortTurbinado(unsigned long int *copia, int inicio, int fim, Dados PDados);
void OrdenaTurbinado(unsigned long int *copia, int inicio, int fim, double *comparacoes, double *trocas, Dados PDados);
int ordenadoCorretamente(unsigned long int *v, int t);
void gravaResultado(FILE *arquivo, double tempoMedio, double tempoDesvio, double comparacoesMedias, double comparacoesDesvio, double trocasMedias, double trocasDesvio, int metodo);
void imprimeVetor(unsigned long int* copia,int n);

int main(){
	unsigned long int* vetor;
	unsigned long int* copia;
	Dados PDados;
  VDados PVDados;
	FILE *arquivo;
	vetor  							  = (unsigned long int*) malloc(TAM * sizeof(unsigned long int));
	copia  							  = (unsigned long int*) malloc(TAM * sizeof(unsigned long int));
	PDados 							  = (Dados) malloc(sizeof (struct Dado));
	PVDados 							= (VDados) malloc(sizeof (struct Dado2));
	PVDados->trocas 			= (double*)malloc((TAM+1)*sizeof(double));
	PVDados->comparacoes  = (double*)malloc((TAM+1)*sizeof(double));
	PVDados->tempo  			= (double*)malloc((TAM+1)*sizeof(double));
	arquivo = fopen("Resultado.txt", "wt");
	if(!arquivo){
		printf("Erro ao criar arquivo");
		exit(EXIT_FAILURE);
	}
	if(!vetor || !copia){
		printf ("Memoria insuficiente");
		exit (EXIT_FAILURE);
	}
  printf("********** ORDENAÇÃO DE VETOR DE %d ELEMENTOS **********\n\n",TAM);
  printf("Preenchendo vetor aleatoriamente \n\n\n");
  preencheVetor(vetor, TAM);

  MetodosOrdencacao(1,arquivo,vetor,copia,PDados,PVDados);
  MetodosOrdencacao(2,arquivo,vetor,copia,PDados,PVDados);
  MetodosOrdencacao(3,arquivo,vetor,copia,PDados,PVDados);
  MetodosOrdencacao(4,arquivo,vetor,copia,PDados,PVDados);

  destroiVetores(vetor, copia, PDados, PVDados);
}

void MetodosOrdencacao(int metodo, FILE* arquivo, unsigned long int* vetor, unsigned long int* copia, Dados PDados, VDados PVDados){
  double tempoMedio, comparacoesMedias, trocasMedias,
         tempoDesvio, comparacoesDesvio, trocasDesvio;
  int i;

  if(metodo==1){printf("********** METODO DE ORDENAÇÃO BOLHA **********\n\n");}
  if(metodo==2){printf("********** METODO DE ORDENAÇÃO INSERÇÃO **********\n\n");}
  if(metodo==3){printf("********** METODO DE ORDENAÇÃO QUICKSORT **********\n\n");}
  if(metodo==4){printf("********** METODO DE ORDENAÇÃO QUICKSORT TURBINADO **********\n\n");}
  tempoMedio        = 0;  tempoDesvio       = 0;
  comparacoesMedias = 0;  comparacoesDesvio = 0;
  trocasMedias      = 0;  trocasDesvio      = 0;
  for(i=1; i <= EXECUCOES; i++){
    printf("\t***** EXECUCAO %d ***** \n\n", i);
    printf("Copiando vetor\n");
    copiaVetor(copia, vetor, TAM);
    if(metodo==1){printf("Ordenando vetor usando o metodo Bolha\n");Bolha(copia, TAM, PDados);}
    if(metodo==2){printf("Ordenando vetor usando o metodo Inserção\n");Insercao(copia, TAM, PDados);}
    if(metodo==3){printf("Ordenando vetor usando o metodo Quicksort\n");Quicksort(copia, 0, TAM-1, PDados);}
    if(metodo==4){printf("Ordenando vetor usando o metodo Quicksort Turbinado\n");QuicksortTurbinado(copia, 0, TAM-1, PDados);}
    if(ordenadoCorretamente(copia, TAM)){
      printf("Vetor ordenado corretamente\n");
      PVDados->tempo[i] 			= PDados->tempo;
      PVDados->comparacoes[i] = PDados->comparacoes;
      PVDados->trocas[i] 		  = PDados->trocas;
      printf("\n\n");
    }else{
      printf("ERRO: Vetor não ordenado\n");
      exit(EXIT_FAILURE);
    }
  }
  tempoMedio        = calculaMedia (PVDados->tempo, EXECUCOES);
  comparacoesMedias = calculaMedia (PVDados->comparacoes, EXECUCOES);
  trocasMedias      = calculaMedia (PVDados->trocas, EXECUCOES);

  tempoDesvio				= calculaDesvio(PVDados->tempo,EXECUCOES,tempoMedio);
  comparacoesDesvio = calculaDesvio(PVDados->comparacoes, EXECUCOES, comparacoesMedias);
  trocasDesvio			= calculaDesvio(PVDados->trocas, EXECUCOES, trocasMedias);
  gravaResultado(arquivo, tempoMedio, tempoDesvio, comparacoesMedias,comparacoesDesvio, trocasMedias, trocasDesvio, metodo);
}

double calculaDesvio(double* vetor, int tam, double media){
	int i;
	double soma = 0;
	for(i=1; i<= tam; i++){
		soma = soma + pow((media - vetor[i]),2);
	}
	soma = soma/tam;
	return(sqrt(soma));
}

double calculaMedia(double* vetor, int tam){
  int i;
  double soma = 0;
  for(i=1; i<=tam; i++){
    soma = soma + vetor[i];
  }
  return(soma/tam);
}

void destroiVetores(unsigned long int* vetor, unsigned long int* copia, Dados PDados, VDados PVDados){
  if (vetor){free(vetor);}
	if (copia){free(copia);}
  if (PVDados->tempo){free(PVDados->tempo);}
  if (PVDados->comparacoes){free(PVDados->comparacoes);}
  if (PVDados->trocas){free(PVDados->trocas);}
	if (PVDados){free(PVDados);}
	if (PDados){free(PDados);}
}

void preencheVetor(unsigned long int *vetor, int t){
	int i;
	srand(time(NULL));
	for(i=0; i < t; i++){
		vetor[i] = (rand()/(double)RAND_MAX) * t;
	}
}

void copiaVetor(unsigned long int *copia, unsigned long int *vetor, int t){
	int i;
	for(i=0; i < t; i++){
		copia[i] = vetor[i];
	}
}

int ComparacaoVal1MaiorVal2(unsigned long int val1, unsigned long int val2, double *comparacoes){
  QtdComparacoes(comparacoes);
  if(val1 > val2){
    return(1);
  }
  return(0);
}

void QtdComparacoes(double *comparacoes){
  comparacoes[0]++;
}

void TrocaVal1ComVal2(unsigned long int *val1, unsigned long int *val2, double *trocas){
  QtdTrocas(trocas);
	unsigned long int aux;
  aux   	= val1[0];
  val1[0] = val2[0];
  val2[0] = aux;
}

void QtdTrocas(double *trocas){
  trocas[0]++;
}

void Bolha(unsigned long int *copia, int t, Dados PDados){
	double start, finish, elapsed, comparacoes = 0, trocas = 0;
	int i, j;
	start = (double)clock()/CLOCKS_PER_SEC;

	for(i=t; i > 0; i--){
		for (j=0; j < i; j++){
			if(ComparacaoVal1MaiorVal2(copia[j], copia[j+1], &comparacoes)){
				TrocaVal1ComVal2(&copia[j], &copia[j+1], &trocas);
			}
		}
	}

	finish = (double)clock()/CLOCKS_PER_SEC;
	elapsed = finish - start;
	PDados->tempo       = elapsed;
	PDados->comparacoes = comparacoes;
	PDados->trocas      = trocas;
}

void Insercao(unsigned long int *copia, int t, Dados PDados){
  double start, finish, elapsed, comparacoes=0.0, trocas=0.0;
	int i, j = 0;
  start = (double)clock()/CLOCKS_PER_SEC;
	unsigned long int aux;

	while (j < t) {
		aux = copia[j];
		i = j-1;
		while (i >= 0 && ComparacaoVal1MaiorVal2(copia[i], aux, &comparacoes)) {
			QtdTrocas(&trocas);
			copia[i+1] = copia[i];
			i--;
		}
		QtdTrocas(&trocas);
		copia[i+1] = aux;
		j++;
	}

	finish = (double)clock()/CLOCKS_PER_SEC;
	elapsed = finish - start;
	PDados->tempo       = elapsed;
	PDados->comparacoes = comparacoes;
	PDados->trocas      = trocas;
}

int Particao(unsigned long int *copia, int inicio, int fim, double *comparacoes, double *trocas){
	unsigned long int x;
	int i, j, sort,sort1,sort2;
	sort = (rand()/(double)RAND_MAX) * (fim-inicio) + inicio;  //Escolhendo Pivo Aleatoriamente
	sort1 = (rand()/(double)RAND_MAX) * (fim-inicio) + inicio; //Escolhendo Pivo Aleatoriamente
	sort2 = (rand()/(double)RAND_MAX) * (fim-inicio) + inicio; //Escolhendo Pivo Aleatoriamente

	if((sort<sort1 && sort1<sort2) || (sort2<sort1 && sort1<sort)){									 //segundo sorteio caso medio
			sort=sort1;
	}else{
		if((sort<sort2 && sort2<sort1) || (sort1<sort2 && sort2<sort)){							 //terceiro sorteiro caso medio
			sort=sort2;
		}
	}																													 //se não é o primeiro que é caso medio
  TrocaVal1ComVal2(&copia[sort], &copia[fim], trocas);

	x = copia[fim];  /* pivo */
	j = inicio-1;
	for(i=inicio; i < fim; i++){
		QtdComparacoes(comparacoes);
		if (copia[i] <= x) {
			j++;
      TrocaVal1ComVal2(&copia[j], &copia[i], trocas);
		}
	}
  TrocaVal1ComVal2(&copia[j+1], &copia[fim], trocas);
	return(j+1);
}

void Quicksort(unsigned long int *copia, int inicio, int fim, Dados PDados){
  double start, finish, elapsed, comparacoes = 0.0, trocas = 0.0;
  start = (double)clock()/CLOCKS_PER_SEC;

	Ordena(copia, inicio, fim, &comparacoes, &trocas);

  finish = (double)clock()/CLOCKS_PER_SEC;
  elapsed = finish - start;
  PDados->tempo       = elapsed;
  PDados->comparacoes = comparacoes;
  PDados->trocas      = trocas;
}

void Ordena(unsigned long int *copia, int inicio, int fim, double *comparacoes, double *trocas){
  if(inicio < fim) {
    int q = Particao(copia, inicio, fim, comparacoes, trocas);
    Ordena(copia, inicio, q - 1, comparacoes, trocas);
    Ordena(copia, q + 1, fim, comparacoes, trocas);
  }
}

void QuicksortTurbinado(unsigned long int *copia, int inicio, int fim, Dados PDados){
  double start, finish, elapsed, comparacoes = 0.0, trocas = 0.0;
  start = (double)clock()/CLOCKS_PER_SEC;

	OrdenaTurbinado(copia, inicio, fim, &comparacoes, &trocas, PDados);

  finish = (double)clock()/CLOCKS_PER_SEC;
  elapsed = finish - start;
  PDados->tempo       = PDados->tempo + elapsed;
  PDados->comparacoes = PDados->comparacoes + comparacoes;
  PDados->trocas      = PDados->trocas + trocas;
}

void OrdenaTurbinado(unsigned long int *copia, int inicio, int fim, double *comparacoes, double *trocas, Dados PDados){
  if(inicio < fim){
		if((fim-inicio) < 40){
			Insercao(&copia[inicio],fim-inicio+1, PDados);
		}else{
			int q = Particao(copia, inicio, fim, comparacoes, trocas);
	    OrdenaTurbinado(copia, inicio, q - 1, comparacoes, trocas, PDados);
	    OrdenaTurbinado(copia, q + 1, fim, comparacoes, trocas, PDados);
		}
  }
}

int ordenadoCorretamente(unsigned long int *v, int t){
	int i;
	for(i = 0; i < (t-1); i++){
		if(v[i] > v[i+1])
			return(0);
	}
	return(1);
}

void gravaResultado(FILE *arquivo, double tempoMedio, double tempoDesvio, double comparacoesMedias, double comparacoesDesvio, double trocasMedias, double trocasDesvio, int metodo){
  if(metodo == 1){fprintf(arquivo, "***** Metodo Bolha *****\n\n");}
  if(metodo == 2){fprintf(arquivo, "***** Metodo Inserção *****\n\n");}
  if(metodo == 3){fprintf(arquivo, "***** Metodo Quicksort *****\n\n");}
  if(metodo == 4){fprintf(arquivo, "***** Metodo Quicksort Turbinado *****\n\n");}
	fprintf(arquivo, "Media do tempo gasto na ordenacao considerando %d execucoes: %.2f segundos \n", EXECUCOES, tempoMedio);
	fprintf(arquivo, "Media das comparacoes na ordenacao considerando %d execucoes: %.2f comparacoes \n", EXECUCOES, comparacoesMedias);
	fprintf(arquivo, "Media das trocas na ordenacao considerando %d execucoes: %.2f trocas \n", EXECUCOES, trocasMedias);
  fprintf(arquivo, "Desvio-Padrao do tempo gasto na ordenacao considerando %d execucoes: %.2f segundos \n", EXECUCOES, tempoDesvio);
	fprintf(arquivo, "Desvio-Padrao das comparacoes na ordenacao considerando %d execucoes: %.2f comparacoes \n", EXECUCOES, comparacoesDesvio);
	fprintf(arquivo, "Desvio-Padrao das trocas na ordenacao considerando %d execucoes: %.2f trocas \n\n", EXECUCOES, trocasDesvio);
}

void imprimeVetor(unsigned long int* copia,int n){
	int i;
	for(i=0; i < n; i++){
		printf("%ld - ",copia[i]);
	}
	printf("\n");
}
