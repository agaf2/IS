// Inclusão de bibliotecas necessárias
#define _XOPEN_SOURCE 600 // Definição para uso específico em algumas bibliotecas

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define INCOGNITAS 4 // Define o tamanho das matrizes
#define ITERACOES 4  // Define o número de iterações

int N; // Número de threads

int A[INCOGNITAS][INCOGNITAS] = { // Matriz A para o método de Jacobi
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 8, 7, 6},
    {5, 4, 3, 2}};

int B[INCOGNITAS] = {1, 2, 3, 4};   // Vetor B para o método de Jacobi
float X[INCOGNITAS] = {1, 1, 1, 1}; // Vetor de valores iniciais para as incógnitas

float result[ITERACOES][INCOGNITAS]; // Matriz para armazenar os resultados de cada iteração

pthread_barrier_t barriers[ITERACOES]; // Barreiras para sincronização das threads

void setup()
{
    printf("Digite o número de threads: \n");
    scanf("%d", &N); // Obtém o número de threads a serem usadas
}

// Função que realiza o método de Jacobi
void *jacobi(void *threadid)
{
    int id = *((int *)threadid); // Identificador da thread

    for (int k = 0; k < ITERACOES; k++) // Loop para cada iteração
    {
        for (int i = id; i < INCOGNITAS; i += N) // Loop para calcular as novas aproximações
        {
            float A_temp = A[i][i]; // Elemento diagonal da matriz A
            float B_temp = B[i];    // Elemento correspondente no vetor B
            float soma = 0;

            for (int j = 0; j < INCOGNITAS; j++) // Loop para calcular a soma
            {
                if (i != j)
                {
                    soma += A[i][j] * X[j]; // Cálculo da soma
                }
            }

            result[k][i] = (1 / A_temp) * (B_temp - soma); // Cálculo da nova aproximação
        }

        pthread_barrier_wait(&barriers[k]); // Sincronização das threads antes de atualizar os valores

        for (int i = id; i < INCOGNITAS; i += N) // Loop para atualizar os valores das incógnitas
        {
            X[i] = result[k][i]; // Atualização dos valores
        }

        pthread_barrier_wait(&barriers[k]); // Sincronização das threads após a atualização
    }

    pthread_exit(NULL); // Finaliza a thread
}

int main()
{
    setup();              // Configuração do número de threads
    pthread_t threads[N]; // Vetor para armazenar as threads
    int ids[N];

    for (int i = 0; i < ITERACOES; i++) // Inicialização das barreiras
    {
        pthread_barrier_init(&barriers[i], NULL, N);
    }

    for (int i = 0; i < N; i++) // Criação das threads
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, jacobi, (void *)&ids[i]);
    }

    for (int i = 0; i < N; i++) // Espera as threads terminarem
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < ITERACOES; i++) // Exibe os resultados de cada iteração
    {
        printf("Refinamento número %d: ", i);
        for (int j = 0; j < INCOGNITAS; j++)
        {
            printf("%d: %f ", j, result[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < ITERACOES; i++) // Destrói as barreiras
    {
        pthread_barrier_destroy(&barriers[i]);
    }

    return 0;
}
