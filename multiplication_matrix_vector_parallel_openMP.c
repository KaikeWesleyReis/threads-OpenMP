#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

// Variaveis definidas
#define row_matriz_A 800
#define col_matriz_A 800
#define upper 1000
#define lower 0
#define THREADS_NUM 4

// A[r,c] * X[c] = B[r]

// FUNCAO - GERAR MATRIZES
void gerarMatrizes(int r, int c, int mA[], int vX[]){
    // Variaveis
    int i;               // indice linha para as matrizes/vetor
    int j;               // indice coluna para as matrizes/vetor
    srandom(time(0));    // Seed
    // Loop para inserir os valores na matrizA[r][c]
    for(i = 0; i < r; i++){
        for(j = 0; j < c; j++){
            mA[i * c + j] = (random() % (upper - lower + 1)) + lower;
        }
    }
    // Loop para inserir os valores no vetorX[c]
    for(j = 0; j < c; j++) vX[j] = (random() % (upper - lower + 1)) + lower; 
    // Retornar para o main
    return;
}

// FUNCAO - MULTIPLICAÇÃO SEQUENCIAL
void multSequencial(int r, int c, int mA[], int vX[], int vBS[]){
    // Variaveis
    int i;               // indice linha para as matrizes/vetor
    int j;               // indice coluna para as matrizes/vetor
    int sum;             // Resultado
    struct timeval tv1;  // auxiliar para pegar o valor de tempo inicial 
    struct timeval tv2;  // auxiliar para pegar o valor de tempo final
    double t1;           // tempo inicial de execucao
    double t2;           // tempo final de execucao
    // Recupera o tempo inicial
    gettimeofday(&tv1, NULL);
    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;
    // MULTIPLICACAO - A[r,c] * X[c] = B[r]
    for(i = 0; i < r; i++){
        sum = 0;
        for(j = 0; j < c; j++){
            sum += mA[i * c + j] * vX[j];
        }
        vBS[i] = sum;
    }
    // Recuperar tempo final de execucao
    gettimeofday(&tv2, NULL);
    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;
    printf("%lf;", (t2 - t1));
    // Retornar para o main
    return;
}

// FUNCAO - MULTIPLICACAO PARALELA COM OpenMP
void matvecHost(int r, int c, int mA[], int vX[], int vBP[]){
    // Variaveis
    int i;               // indice linha para as matrizes/vetor
    int j;               // indice coluna para as matrizes/vetor
    int sum;             // Resultado
    struct timeval tv1;  // auxiliar para pegar o valor de tempo inicial 
    struct timeval tv2;  // auxiliar para pegar o valor de tempo final
    double t1;           // tempo inicial de execucao
    double t2;           // tempo final de execucao
    // Recupera o tempo inicial
    gettimeofday(&tv1, NULL);
    t1 = (double)(tv1.tv_sec) + (double)(tv1.tv_usec)/ 1000000.00;
    // Gerar numero de threads igual a existente fisicamente
    omp_set_num_threads(THREADS_NUM);
    // Bloco paralelo
    #pragma omp parallel private(i, j, sum) shared(mA, vBP, vX)
    {
        #pragma omp for
            for(i = 0; i < r; i++){
                sum = 0;
                for(j = 0; j < c; j++){
                    sum += mA[i * c + j] * vX[j];
                }
                vBP[i] = sum;
            }
    }
    // Recuperar tempo final
    gettimeofday(&tv2, NULL);
    t2 = (double)(tv2.tv_sec) + (double)(tv2.tv_usec)/ 1000000.00;  // debug - verifica o resultado
    // calcula e imprime o tempo total de execucao
    printf("%lf\n", (t2 - t1));
    // Retornar
    return;
}

// FUNCAO - MULTIPLICACAO PARALELA COM CUDA
void matvecDevice(int r, int c, int mA[], int vX[], int vBP[]) {
  printf("matvecDevice\n");
  return;
}

// FUNCAO - PRINCIPAL
int main(int argc, char * argv[]) {
    // Inicializacoes de valores
    int i;
    int b;
    int row;
    int col;
    int verifyB = 0;
    int benchmarks = 10;
    row = row_matriz_A;
    col = col_matriz_A;
    // Inicializacoes de matrizes e vetores dinâmicamente
    int *matrizA = (int *)calloc(row * col, sizeof(int));
    int *vectorX = (int *)calloc(col * 1, sizeof(int));
    int *vectorBS = (int *)calloc(row * 1, sizeof(int));
    int *vectorBP = (int *)calloc(row * 1, sizeof(int));
    // Atribuicao da opcao
    int opcao = atoi(argv[1]);
    // Benchmarks
    printf("linhas - %d | colunas - %d\nbenchmark;serial;paralelismo\n",row,col);
    for(b = 0; b < benchmarks; b++){
        // Gerar matrizes para avaliacao
        gerarMatrizes(row, col, matrizA, vectorX);
        // Print benchmark run
        printf("%d;",b);
        // Fazer multiplicacao sequencial sem paralelismo                     
        multSequencial(row, col, matrizA, vectorX, vectorBS);
        // Paralelismo por CPU                    
        if(opcao == 1) matvecHost(row, col, matrizA, vectorX, vectorBP);
        // Paralelismo por GPU          
        else if(opcao == 2) matvecDevice(row, col, matrizA, vectorX, vectorBP);
        // Verificacao do vetor B
        for(i=0;i<row;i++){
            if(vectorBS[i] != vectorBP[i]){
                printf("Elemento distinto | i = %d | BS[i] = %d | BP[i] = %d\n",i,vectorBS[i],vectorBP[i]);
                verifyB = 1;
            }
        }
        if(verifyB == 1) printf("ERROR - Vetores B distintos!\n");
    }
    // Finalizacao
    return 0;
}