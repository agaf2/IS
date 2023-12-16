#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, t, p, *visit = NULL, *product = NULL, *product_in_use = NULL; // n = número de arquivos, t = número de threads, p = número de produtos, 
                                                    //visit = vetor de visitados, product = vetor de produtos, product_in_use = vetor de produtos em uso

pthread_mutex_t *mutex_arr = NULL;   // mutex para cada produto
pthread_cond_t *product_free = NULL; // cond para cada produto

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

void open()
{
    FILE *fp;
    for (int i = 0; i < n; i++)
    {
        char s[100];
        sprintf(s, "%d.in", i + 1);     // cria o nome do arquivo
        fp = fopen(s, "w");             // abre o arquivo
        fclose(fp); 
    }
}

void file_reader(int i, int id)
{
    visit[i] = 1;   // marca o arquivo como visitado
    FILE *fp;
    char s[100];
    sprintf(s, "%d.in", i + 1);
    fp = fopen(s, "r");
    printf("Thread %d abriu o arquivo %d.in\n", id, i + 1);
    while (fscanf(fp, "%d", &product[i]) != EOF)
    {
        printf("Thread %d leu o produto %d do arquivo %d.in\n", id, product[i], i + 1);

        pthread_mutex_lock(&mutex_arr[i]); // lock no mutex do produto

        if (product_in_use[product[i]] == 1)    // se o produto estiver em uso, espera
        {
            printf("Thread %d esperando o produto %d ficar livre\n", id, product[i]);    
            pthread_cond_wait(&product_free[product[i]], &mutex_arr[i]);          // espera o produto ficar livre
        }
        else
        {
            product_in_use[product[i]] = 1;             // marca o produto como em uso
            product[i]++;                               // incrementa o produto
            product_in_use[product[i]] = 0;             // marca o produto como livre
            pthread_cond_broadcast(&product_free[product[i]]); // sinaliza que o produto está livre
        }
    }
    fclose(fp);
    printf("Thread %d fechou o arquivo %d.in\n", id, i + 1);
}

void *file_selecter(void *threadid)
{
    int id = *((int *)threadid);
    for (int i = 0; i < n; i++)             
        {
            if (visit[i] == 0)            // se o arquivo não foi visitado, lê o arquivo
            {
                file_reader(i, id);
            }
        }
    pthread_exit(NULL);     // termina a thread se todos os arquivos foram visitados
    
}

void print()
{
    printf("Produtos:\n");
    int cont = 0;
    for(int i = 0; i <=p; i++)
    {
        if(product[i] != 0)
        {
            cont+=product[i];
        }
    }

    printf("Total: %d\n", cont);

    for(int i = 0; i <=p; i++)
    {
        if(product[i] != 0)
        {
            printf("Produto %d teve %d%% de venda\n", i, (product[i]*100)/cont);
        }
    }

}

int main()
{
    setup();

    open();
                                                                                // alocação de memória
    mutex_arr = (pthread_mutex_t *)malloc((p + 1) * sizeof(pthread_mutex_t));   // mutex_arr = mutex para cada produto
    product_free = (pthread_cond_t *)malloc((p + 1) * sizeof(pthread_cond_t));  // product_free = cond para cada produto
    product_in_use = (int *)calloc((p + 1), sizeof(int));                    // product_in_use = vetor de produtos em uso
    visit = (int *)calloc(n, sizeof(int));                                  // visit = vetor de visitados
    product = (int *)calloc(p+1, sizeof(int));                         // product = vetor de produtos

    for (int i = 0; i < p; i++)
    {
        mutex_arr[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;          // inicialização dos mutex e cond
        product_free[i] = (pthread_cond_t)PTHREAD_COND_INITIALIZER;         
    }

    pthread_t threads[t];   // declaração das threads
    int id[t];        // id das threads

    for (int i = 0; i < t; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, &file_selecter, &id[i]);  // criação das threads
    }

    for (int i = 0; i < t; i++)
    {
        pthread_join(threads[i], NULL); // espera as threads terminarem
    }
                        // liberação de memória

    print();

    
    free(mutex_arr);
    free(product_free);
    free(product_in_use);
    free(visit);
    free(product);
    
    pthread_exit(NULL);
}
