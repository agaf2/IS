#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int tam;
int *arr;

typedef struct {
    int left;
    int right;
} Interval;


void setup(){
    printf("Digite o tamanho do vetor: ");
    scanf("%d", &tam);
    arr = (int *) malloc(tam*sizeof(int));
    for(int i = 0, a; i < tam; i++){
        printf("Digite o elemento %d do vetor: ", i);
        scanf("%d", &a);
        arr[i]=a;
    }
}

void print(){
    for (int i = 0; i < tam; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void merge(int left, int mid, int right) {   // função merge do merge sort não usa threads
    int i, j, k, b[tam];
    i = left;
    j = mid + 1;
    k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            b[k++] = arr[i++];
        } else {
            b[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        b[k++] = arr[i++];
    }

    while (j <= right) {
        b[k++] = arr[j++];
    }

    for (i = left; i < k; i++) {
        arr[i] = b[i];
    }
}

void *sort(void *actual) {
    Interval *inactual = (Interval *) actual; // recebe o intervalo atual
    int mid;
    if (inactual->left < inactual->right) {
        mid = (inactual->left + inactual->right) / 2;

        Interval inl = {inactual->left, mid};   // cria o intervalo esquerdo
        Interval inr = {mid + 1, inactual->right};  // cria o intervalo direito

        pthread_t idl, idr;     // id das threads
        pthread_create(&idl, NULL, sort, &inl); // criação da thread esquerda
        pthread_create(&idr, NULL, sort, &inr); // criação da thread direita

        pthread_join(idl, NULL);    // espera a thread esquerda acabar
        pthread_join(idr, NULL);    // espera a thread direita acabar
                                    // isso garante que a thread pai só vai continuar quando as duas filhas acabarem

        merge(inactual->left, mid, inactual->right);    // faz o merge dos dois intervalos
    }
    pthread_exit(NULL); // termina a thread
}

int main() {
    
    setup();
    
    Interval first = {0, tam - 1};      // é necssário criar uma struct para passar os parâmetros para a thread
    pthread_t id;                       // id da thread
    pthread_create(&id, NULL, sort, &first);   // cria a primeira thread do intervalo [0, tam-1]
    pthread_join(id, NULL);                   // espera a primeira thread acabar

    print();

    free(arr);
    
    return 0;
}

