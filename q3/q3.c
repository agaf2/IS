#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int N; // Número de threads

#define INCOGNITAS 4
#define ITERACOES 4

int A[INCOGNITAS][INCOGNITAS] = { // MATRIZ A
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 8, 7, 6},
    {5, 4, 3, 2}};

int B[INCOGNITAS] = {1, 2, 3, 4}; // MATRIZ B

int X[INCOGNITAS] = {1, 1, 1, 1}; // MATRIZ X

float result[ITERACOES][INCOGNITAS]; // MATRIZ RESULTADO

pthread_barrier_t barrier;

void setup()
{
    printf("Digite o numero de threads: ");
    scanf("%d", &N);
}

void *jacobi(void *threadid)
{

    int id = *((int *)threadid);
    float soma = 0;
    float A_temp = 0;
    float B_temp = 0;

    for (int k = 0; k < ITERACOES; k++)
    {
        for (int i = id; i < INCOGNITAS; i += N)
        {
            A_temp = A[i][i], B_temp = B[i];
            soma = 0;

            // Calcula a soma
            for (int j = 0; j < N; j++)
            {
                if (i != j)
                {
                    if (k == 0)
                    {
                        soma += A[i][j];
                    }
                    else
                    {
                        soma += A[i][j] * X[j];
                    }
                }
            }
            result[k][i] = (1 / A_temp) * (B_temp - soma); // Calcula o valor de X
        }
        pthread_barrier_wait(&barrier);
        for (int i = id; i < INCOGNITAS; i += N)
        {
            X[i] = result[k][i];
        }
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

int main()
{
    setup();
    pthread_t threads[N];
    int *ids[N];

    pthread_barrier_init(&barrier, NULL, N);

    for (int i = 0; i < N; i++)
    {
        ids[i] = (int *)malloc(sizeof(int));
        *ids[i] = i;
        pthread_create(&threads[i], NULL, jacobi, (void *)ids[i]);
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

    pthread_barrier_destroy(&barrier);

    pthread_exit(NULL);

    return 0;
}