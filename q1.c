#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int N_robos, M_mesa, Q_fila;
int cont = 0;
int first = 0;
int last = 0;
int *fila = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void put(int i) {
    pthread_mutex_lock(&mutex); // entra na região crítica

    while (cont == Q_fila) {
        pthread_cond_wait(&empty, &mutex); // dorme enquanto não tem espaço
    }

    fila[last] = i; // coloca o valor na fila

    cont++;
    last++;

    if (last == Q_fila) {
        last = 0;            
    }

    if (cont == 1) {
        pthread_cond_broadcast(&full);   // acorda as threads que estão dormindo
    }

    pthread_mutex_unlock(&mutex);   // sai da região crítica
}

void get() {
    pthread_mutex_lock(&mutex);   // entra na região crítica

    while (cont == 0) {
        pthread_cond_wait(&full, &mutex);  // dorme enquanto não tem item
    }
    
    cont--;
    first++;

    if (first == Q_fila) {
        first = 0;
    }

    if (cont == Q_fila - 1) {
        pthread_cond_broadcast(&empty);    // acorda as threads que estão dormindo
    }

    pthread_mutex_unlock(&mutex);   // sai da região crítica
}

void *producer(void *threadid) {                    
    while(1){          
    printf("Mesa %d iniciou\n", *((int *)threadid)); 
    put(1);                                          // produz                       
    printf("Mesa %d produziu %d\n",*((int *)threadid), 1);
    printf("A fila tem %d itens\n", cont);
    printf("Mesa %d terminou\n", *((int *)threadid));
    }
}

void *consumer(void *threadid) {
    while(1){
    printf("robo %d iniciou\n", *((int *)threadid));    
    get(1);                             // consome
    printf("robo %d consumiu %d\n",*((int *)threadid), 1); // consome 
    printf("A fila tem %d itens\n", cont);
    printf("robo %d terminou \n", *((int *)threadid));  
    }

}

int main() {
    printf("Digite o número de robôs: ");
    scanf("%d", &N_robos);
    printf("Digite o número de mesas: ");
    scanf("%d", &M_mesa);

    while (M_mesa < N_robos) {
        printf("O número de mesas deve ser maior ou igual ao número de robôs.\nDigite novamente: ");
        scanf("%d", &M_mesa);
    }

    printf("Digite tamanho maximo da fila: ");
    scanf("%d", &Q_fila);

    fila = (int *)malloc(Q_fila * sizeof(int));

    pthread_t producer_thread[M_mesa];      // quantidade de threads
    pthread_t consumer_thread[N_robos];

    int *idM[M_mesa];                     
    int *idN[N_robos];
    
        for (int i = 0; i < N_robos; i++) {       
            idN[i] = (int *)malloc(sizeof(int));             // aloca memória para o id
            *idN[i] = i;                                                                            
            pthread_create(&consumer_thread[i], NULL, consumer, (void *)idN[i]);    // cria a thread
        }

        for (int i = 0; i < M_mesa; i++) {
            idM[i] = (int *)malloc(sizeof(int));            // aloca memória para o id 
            *idM[i] = i;
            pthread_create(&producer_thread[i], NULL, producer, (void *)idM[i]);   // cria a thread
        }

        for (int i = 0; i < M_mesa; i++) {
            pthread_join(producer_thread[i], NULL);   // espera a thread terminar
        }

        for (int i = 0; i < N_robos; i++) {
            pthread_join(consumer_thread[i], NULL);   // espera a thread terminar
        }

        pthread_exit(NULL);   // termina a thread
    

}
