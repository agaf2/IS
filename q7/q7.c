#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//falta criar a lista de prioridade

int pos[10] = {0,0,0,0, 0, 0, 0, 0, 0, 0}; // posicao de cada trem 
int inter_cont[5] = {0,0,0,0,0}; // quantidade de trens em cada intercesao

pthread_mutex_t mutex[5];
pthread_cond_t cond_array[5];

void insert(int id) {
    pthread_mutex_lock(&mutex[pos[id]]); // trava o mutex
    
    while (inter_cont[pos[id]] == 2){
        pthread_cond_wait(&cond_array[pos[id]], &mutex[pos[id]]);  // espera a fila não estar cheia
    }
    
    inter_cont[pos[id]]++;

    pthread_mutex_unlock(&mutex[pos[id]]); // libera o mutex
    
}

void removing(int id) {
    pthread_mutex_lock(&mutex[pos[id]]);
   
    inter_cont[pos[id]]--;
    if (inter_cont[pos[id]] < 2) {
        pthread_cond_broadcast(&cond_array[pos[id]]); // sinaliza que a fila não está mais cheia
    }
    pthread_mutex_unlock(&mutex[pos[id]]); // libera o mutex
}

void *train_function(void *threadid){
    int id = *((int *)threadid);
    while(1){
        //chegar na posicao
        insert(id);
        printf("O trem %d parou na intercesao %d\n", id, pos[id]);
        // fazer o timer de espera
        usleep(500000);
        //sair da posicao
        removing(id);
        printf("O trem %d saiu da intercesao %d\n", id, pos[id]);
        if (pos[id] == 4){
            pos[id] = 0;
        } else {
            pos[id]++;
        }
    }
}

int main(){

    for(int i = 0; i < 5; i++)
        mutex[i] = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    for(int i = 0; i < 5; i++)
        cond_array[i] = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    
    pthread_t trains[10];
    int id_trains[10];
    
    //fazer o infinito
    for (int x = 0; x < 10; x++){
        id_trains[x] = x;
        pthread_create(&trains[x], NULL, train_function, &id_trains[x]); //cria as threads
    }
    for (int x = 0; x < 10; x++) {
        pthread_join(trains[x], NULL);   // impede que a main acabe antes das threads
    }
    
}
