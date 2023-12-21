#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define INCOGNITAS 4
#define ITERACOES 4

int N; // Número de threads

int A[INCOGNITAS][INCOGNITAS] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 8, 7, 6},
    {5, 4, 3, 2}};

int B[INCOGNITAS] = {1, 2, 3, 4};

float X[INCOGNITAS] = {1, 1, 1, 1};

float result[ITERACOES][INCOGNITAS];

pthread_barrier_t barriers[ITERACOES];

void setup()
{
    printf("Digite o número de threads: \n");
    scanf("%d", &N);
}

void *jacobi(void *threadid)
{
    int id = *((int *)threadid);

    for (int k = 0; k < ITERACOES; k++)
    {
        for (int i = id; i < INCOGNITAS; i += N)
        {
            float A_temp = A[i][i];
            float B_temp = B[i];
            float soma = 0;

            for (int j = 0; j < INCOGNITAS; j++)
            {
                if (i != j)
                {
                    soma += A[i][j] * X[j];
                }
            }

            result[k][i] = (1 / A_temp) * (B_temp - soma);
        }

        pthread_barrier_wait(&barriers[k]);

        for (int i = id; i < INCOGNITAS; i += N)
        {
            X[i] = result[k][i];
        }

        pthread_barrier_wait(&barriers[k]);
    }

    pthread_exit(NULL);
}

int main()
{
    setup();
    pthread_t threads[N];
    int ids[N];

    for (int i = 0; i < ITERACOES; i++)
    {
        pthread_barrier_init(&barriers[i], NULL, N);
    }

    for (int i = 0; i < N; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, jacobi, (void *)&ids[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < ITERACOES; i++)
    {
        printf("Refinamento número %d: ", i);
        for (int j = 0; j < INCOGNITAS; j++)
        {
            printf("%d: %f ", j, result[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < ITERACOES; i++)
    {
        pthread_barrier_destroy(&barriers[i]);
    }

    return 0;
}
