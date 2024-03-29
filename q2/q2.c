// O arquivo de executável deve está no mesmo diretório dos arquivos de entrada.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, t, p, *visit = NULL, *product = NULL; // n = número de arquivos, t = número de threads, p = número de produtos,
                                             // visit = vetor de arquivos visitados, product = vetor de produtos

pthread_mutex_t *mutex_arr = NULL; // mutex para cada produto

void setup()
{
    printf("Digite o número de arquivos: ");
    scanf("%d", &n);
    printf("Digite o número de threads: ");
    scanf("%d", &t);
    while (t > n)
    {
        printf("O número de threads deve ser menor ou igual ao número de arquivos.\n");
        printf("Digite o número de threads: ");
        scanf("%d", &t);
    }
    printf("Digite o número de produtos: ");
    scanf("%d", &p);
}

void file_reader(int i, int id)
{
    visit[i] = 1; // marca o arquivo como visitado
    int val;
    FILE *fp;
    char s[100];
    sprintf(s, "%d.in", i + 1);
    fp = fopen(s, "r");
    printf("Thread %d abriu o arquivo %d.in\n", id, i + 1);
    while (fscanf(fp, "%d", &val) != EOF)
    {
        printf("Thread %d leu o produto %d do arquivo %d.in\n", id, val, i + 1);
        pthread_mutex_lock(&mutex_arr[val]);   // lock no mutex do produto
        product[val]++;                        // incrementa o produto
        pthread_mutex_unlock(&mutex_arr[val]); // libera o mutex do produto
    }
    fclose(fp);
    printf("Thread %d fechou o arquivo %d.in\n", id, i + 1);
}

void *file_selecter(void *threadid)
{
    int id = *((int *)threadid);
    for (int i = 0; i < n; i++)
    {
        if (visit[i] == 0) // se o arquivo não foi visitado, lê o arquivo
        {
            file_reader(i, id);
        }
    }
    pthread_exit(NULL); // termina a thread se todos os arquivos foram visitados
}

void print()
{

    int cont = 0;
    for (int i = 0; i <= p; i++)
    {
        if (product[i] != 0)
        {
            cont += product[i];
        }
    }

    printf("Total: %d\n", cont);

    for (int i = 0; i <= p; i++)
    {
        if (product[i] != 0)
        {
            printf("Produto %d teve %d%% de venda\n", i, (product[i] * 100) / cont);
        }
    }
}

int main()
{
    setup();

    mutex_arr = (pthread_mutex_t *)malloc((p + 1) * sizeof(pthread_mutex_t)); // mutex_arr = mutex para cada produto
    visit = (int *)calloc(n, sizeof(int));                                    // visit = vetor de visitados
    product = (int *)calloc(p + 1, sizeof(int));                              // product = vetor de produtos

    for (int i = 0; i < p; i++)
        mutex_arr[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; // inicialização dos mutex

    pthread_t threads[t]; // declaração das threads
    int id[t];            // id das threads

    for (int i = 0; i < t; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, &file_selecter, &id[i]); // criação das threads
    }

    for (int i = 0; i < t; i++)
        pthread_join(threads[i], NULL); // espera as threads terminarem

    print();

    // liberação de memória
    for (int i = 0; i < n; i++)
    {
        pthread_mutex_destroy(&mutex_arr[i]);
    }

    free(mutex_arr);
    free(visit);
    free(product);

    pthread_exit(NULL);
}
