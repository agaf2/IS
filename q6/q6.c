#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Definição de estruturas para pares e matrizes
typedef struct
{
    int index;
    float val;
} Pair;

typedef struct
{
    Pair **matrix;
    int lines;
    int columns;
    int esplines;
    int *espcolumns;
    int maxcolumns;
} Matrix;

// Matrizes utilizadas
Matrix aux = {NULL, 0, 0, 0, NULL, 0};
Matrix defaultzin = {NULL, 0, 0, 0, NULL, 0};
Matrix result = {NULL, 0, 0, 0, NULL, 0};

// Função para multiplicar matrizes esparsas
void *multiply_sparse_matrices(void *args)
{
    int row = *((int *)args);

    for (int j = 0; j < aux.columns; j++)
    {
        result.matrix[row][j].val = 0;

        for (int k = 0; k < aux.columns; k++)
        {
            if (k < aux.espcolumns[row] && aux.matrix[row][k].index == k)
            {
                for (int m = 0; m < defaultzin.columns; m++)
                {
                    if (m < defaultzin.espcolumns[k] && defaultzin.matrix[k][m].index == m)
                    {
                        result.matrix[row][j].val += aux.matrix[row][k].val * defaultzin.matrix[k][m].val;
                    }
                }
            }
        }
    }
    pthread_exit(NULL);
}

void print_result_matrix(Matrix *matriz)
{
    printf("Matriz Resultante:\n");
    for (int i = 0; i < matriz->lines; i++)
    {
        for (int j = 0; j < matriz->columns; j++)
        {
            printf("%.2f ", matriz->matrix[i][j].val);
        }
        printf("\n");
    }
    printf("\n");
}

// Função para configurar matrizes esparsas
void setup_sparse_matrix(int lines, int columns, Matrix *matriz)
{
    matriz->lines = lines;
    matriz->columns = columns;
    matriz->esplines = 0;
    matriz->espcolumns = NULL;
    matriz->maxcolumns = 0;

    matriz->matrix = (Pair **)malloc(lines * sizeof(Pair *));
    for (int i = 0; i < lines; i++)
    {
        matriz->matrix[i] = NULL;
        matriz->esplines++;
        matriz->espcolumns = (int *)realloc(matriz->espcolumns, matriz->esplines * sizeof(int));
        matriz->espcolumns[i] = 0;

        matriz->matrix[i] = (Pair *)malloc(columns * sizeof(Pair));
        for (int j = 0; j < columns; j++)
        {
            float val;
            printf("Digite o elemento da linha %d, coluna %d: ", i, j);
            scanf("%f", &val);

            if (val != 0)
            {
                matriz->matrix[i][matriz->espcolumns[i]].val = val;
                matriz->matrix[i][matriz->espcolumns[i]].index = j;
                matriz->espcolumns[i]++;
                if (matriz->espcolumns[i] > matriz->maxcolumns)
                {
                    matriz->maxcolumns = matriz->espcolumns[i];
                }
            }
        }
        matriz->matrix[i] = realloc(matriz->matrix[i], matriz->espcolumns[i] * sizeof(Pair));
    }
}

// Função principal
int main()
{
    int option;
    pthread_t *threads = NULL;
    int *thread_ids = NULL;

    while (1)
    {
        printf("----------Menu----------\n");
        printf("Escolha o tipo de operação: \n");
        printf("1 - Ler matriz default\n");
        printf("2 - Ler matriz ou vetor denso\n");
        printf("3 - Multiplicar a matriz defaultzin por auxiliar\n");
        printf("4 - Sair\n");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("Digite as dimensões para a matriz 'default' (linhas colunas): ");
            int lines_defaultzin, columns_defaultzin;
            scanf("%d %d", &lines_defaultzin, &columns_defaultzin);
            setup_sparse_matrix(lines_defaultzin, columns_defaultzin, &defaultzin);
            break;
        case 2:
            printf("Digite as dimensões da 'aux' (linhas colunas): ");
            int lines_aux, columns_aux;
            scanf("%d %d", &lines_aux, &columns_aux);
            setup_sparse_matrix(lines_aux, columns_aux, &aux);
            break;
        case 3:
            if (defaultzin.lines == aux.columns)
            {
                result.lines = aux.lines;
                result.columns = defaultzin.columns;
                result.matrix = (Pair **)malloc(result.lines * sizeof(Pair *));
                for (int i = 0; i < result.lines; i++)
                {
                    result.matrix[i] = (Pair *)malloc(result.columns * sizeof(Pair));
                }
                threads = malloc(aux.esplines * sizeof(pthread_t));
                thread_ids = malloc(aux.esplines * sizeof(int));

                for (int i = 0; i < aux.esplines; i++)
                {
                    thread_ids[i] = i;
                    pthread_create(&threads[i], NULL, multiply_sparse_matrices, &thread_ids[i]);
                }

                for (int i = 0; i < aux.esplines; i++)
                {
                    pthread_join(threads[i], NULL);
                }

                printf("Multiplicação de matrizes concluída.\n");
                print_result_matrix(&result);
            }
            else
            {
                printf("Dimensões da matriz não são compatíveis para multiplicação.\n");
            }
            break;
        case 4:
            // Limpeza e saída do programa
            for (int i = 0; i < aux.lines; i++)
            {
                free(aux.matrix[i]);
            }
            free(aux.matrix);
            free(aux.espcolumns);

            for (int i = 0; i < defaultzin.lines; i++)
            {
                free(defaultzin.matrix[i]);
            }
            free(defaultzin.matrix);
            free(defaultzin.espcolumns);

            for (int i = 0; i < result.lines; i++)
            {
                free(result.matrix[i]);
            }
            free(result.matrix);

            free(threads);
            free(thread_ids);
            return 0;
        default:
            printf("Opção inválida!\n");
            break;
        }
    }

    return 0;
}
