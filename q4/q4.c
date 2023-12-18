#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int L, C, T;
int matriz[3][3];
int eh_magica = 1;
int head_sum = 0; // Armazenar a soma da primeira linha/coluna/diagonal

void setup()
{
    int temp = 0;
    printf("Digite um número inteiro para as colunas e linhas: ");
    scanf("%d", &temp);
    C = temp, L = temp, T = temp * 2 + 2;

    printf("Digite os números da matriz:\n");
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            scanf("%d", &matriz[i][j]);
        }
    }
}

void *quadrado_magico(void *threadid)
{
    int id = *((int *)threadid);
    int sum = 0;

    if (id < L) // Linhas
    {
        for (int j = 0; j < C; j++)
        {
            sum += matriz[id][j];
        }
    }
    else if (id < L + C) // Colunas
    {
        for (int i = 0; i < L; i++)
        {
            sum += matriz[i][id - L];
        }
    }
    else
    {
        if (id == L + C) // Diagonal principal
        {
            for (int i = 0; i < L; i++)
            {
                sum += matriz[i][i];
            }
        }
        else // Diagonal secundária
        {
            for (int i = 0; i < L; i++)
            {
                sum += matriz[i][L - i - 1];
            }
        }
    }

    if (head_sum == 0)
    {
        head_sum = sum; // Armazenar a soma da primeira linha/coluna/diagonal que foi computada
    }
    else if (head_sum != sum)
    {
        eh_magica = 0; // Se a soma da linha/coluna/diagonal atual for diferente da primeira, não é um quadrado mágico
    }

    pthread_exit(NULL);
}

int main()
{

    setup();

    pthread_t threads[T];
    int *taskids[T];

    for (int t = 0; t < T; t++)
    {
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        pthread_create(&threads[t], NULL, quadrado_magico, (void *)taskids[t]);
    }

    for (int u = 0; u < T; u++)
    {
        pthread_join(threads[u], NULL);
    }

    if (eh_magica)
    {
        printf("É um quadrado magico.\n");
    }
    else
    {
        printf("Não é um quadrado mágico.\n");
    }

    pthread_exit(NULL);
}
