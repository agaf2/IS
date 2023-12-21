#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int N_robos, M_mesa, Q_fila;
int cont = 0;
int first = 0;
int last = 0;
int *fila;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void put(int i)
{
    pthread_mutex_lock(&mutex); // trava o mutex
    while (cont == Q_fila)
    {
        pthread_cond_wait(&empty, &mutex); // espera a fila não estar cheia
    }
    fila[last] = i;
    cont++;
    last++;

    if (last == Q_fila)
    {
        last = 0;
    }
    if (cont == 1)
    {
        pthread_cond_broadcast(&full); // sinaliza que a fila não está mais vazia
    }
    pthread_mutex_unlock(&mutex); // libera o mutex
}

int get()
{
    int resultado;
    pthread_mutex_lock(&mutex);
    while (cont == 0)
    {
        pthread_cond_wait(&full, &mutex); // espera a fila não estar vazia
    }
    resultado = fila[first];

    cont--;
    first++;

    if (first == Q_fila)
    {
        first = 0;
    }

    if (cont == Q_fila - 1)
    {
        pthread_cond_broadcast(&empty); // sinaliza que a fila não está mais cheia
    }
    pthread_mutex_unlock(&mutex); // libera o mutex

    return resultado;
}

void *producer(void *threadid)
{
    int id = *((int *)threadid);
    for (int i = 0;; i++)
    {
        printf("Mesa %d iniciou\n", *((int *)threadid));
        put(i); // produz
        printf("Mesa %d produziu %d\n", id, i);
        printf("A fila tem %d itens\n", cont);
        printf("Mesa %d terminou\n", id);
    }
}

void *consumer(void *threadid)
{
    int save;
    int id = *((int *)threadid);
    printf("Robo %d iniciou\n", id);
    for (int i = 0;; i++)
    {
        save = get(); // consome
        printf("Robo %d consumiu %d\n", id, save);
    }
    printf("Robo %d terminou \n", id);
}

int main()
{
    printf("Digite o número de robôs: ");
    scanf("%d", &N_robos);
    printf("Digite o número de mesas: ");
    scanf("%d", &M_mesa);

    while (M_mesa < N_robos)
    {
        printf("O número de mesas deve ser maior ou igual ao número de robôs.\nDigite novamente: ");
        scanf("%d", &M_mesa);
    }

    printf("Digite tamanho maximo da fila: ");
    scanf("%d", &Q_fila);
    fila = (int *)malloc(sizeof(int) * Q_fila);

    pthread_t producer_thread[M_mesa];  // Threads do tipo consumidor
    pthread_t consumer_thread[N_robos]; // Threads do tipo produtor

    int *idR[N_robos];
    int *idM[M_mesa];

    for (int i = 0; i < N_robos; i++)
    {
        idR[i] = (int *)malloc(sizeof(int));
        *idR[i] = i;
        pthread_create(&consumer_thread[i], NULL, consumer, (void *)idR[i]); // Cria as threads de consumidor(Mesas)
    }

    for (int i = 0; i < M_mesa; i++)
    {
        idM[i] = (int *)malloc(sizeof(int));
        *idM[i] = i;
        pthread_create(&producer_thread[i], NULL, producer, (void *)idM[i]); // Cria as threads de produtor(Garçons)
    }

    for (int i = 0; i < M_mesa; i++)
    {
        pthread_join(producer_thread[i], NULL); // Impede que a main acabe antes das threads
    }

    for (int i = 0; i < N_robos; i++)
    {
        pthread_join(consumer_thread[i], NULL); // Impede que a main acabe antes das threads
    }
}