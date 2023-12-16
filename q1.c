#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int N_robos, M_mesa, Q_fila;
int cont = 0;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void put() {
    pthread_mutex_lock(&mutex); // trava o mutex
    while (cont == Q_fila) {
        pthread_cond_wait(&empty, &mutex);  // espera a fila não estar cheia
    }
    cont++;

    if (cont == 1) {
        pthread_cond_broadcast(&full); // sinaliza que a fila não está mais vazia
    }
    pthread_mutex_unlock(&mutex); // libera o mutex
}

void get() {
    pthread_mutex_lock(&mutex);
    while (cont == 0) {
        pthread_cond_wait(&full, &mutex); // espera a fila não estar vazia
    }
    cont--;
    
    if (cont == Q_fila - 1) {
        pthread_cond_broadcast(&empty); // sinaliza que a fila não está mais cheia
    }
    pthread_mutex_unlock(&mutex); // libera o mutex
}

void *producer(void *threadid) {                    
    int id = *((int *)threadid);
    while(1){          
    printf("Mesa %d iniciou\n", *((int *)threadid)); 
    put();      // produz                       
    printf("Mesa %d produziu 1\n",id);
    printf("A fila tem %d itens\n", cont);
    printf("Mesa %d terminou\n", id);
    }
}

void *consumer(void *threadid) {
    int id = *((int *)threadid);
    while(1){
    printf("robo %d iniciou\n", id);    
    get();      // consome
    printf("robo %d consumiu 1\n",id);  
    printf("A fila tem %d itens\n", cont);
    printf("robo %d terminou \n", id);  
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

    pthread_t producer_thread[M_mesa];      // quantidade de threads
    pthread_t consumer_thread[N_robos];

    int idM[M_mesa];                     
    int idN[N_robos];
    
        for (int i = 0; i < N_robos; i++) {       
            idN[i] = i;                                                                            
            pthread_create(&consumer_thread[i], NULL, consumer, &idN[i]);    // cria as threads
        }

        for (int i = 0; i < M_mesa; i++) {
            idM[i] = i;
            pthread_create(&producer_thread[i], NULL, producer, &idM[i]);   // cria as threads
        }

        for (int i = 0; i < M_mesa; i++) {
            pthread_join(producer_thread[i], NULL);   // impede que a main acabe antes das threads
        }

        for (int i = 0; i < N_robos; i++) {
            pthread_join(consumer_thread[i], NULL);   // impede que a main acabe antes das threads
        }

}