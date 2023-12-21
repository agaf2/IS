#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
TO DO:
-Limitar as entradas em setup
-Fazer uma função para definir como vai ser o tamnho das matrizes na multiplicação
-Fazer as funções/threads de multi. Uso de barreira?
*/

typedef struct{
    int index;
    float val;
} Pair;

typedef struct{
    Pair ** matrix;
    int lines;     //lines = linhas
    int columns;  //columns = colunas
    int esplines; //esplines = linhas não nulas
    int * espcolumns; // vetor que armazena quantos elementos não nulos tem em cada linha 
}Matrix;

Matrix defaultzin = {NULL, 0, 0, 0, NULL};
Matrix aux = {NULL, 0, 0, 0, NULL};

void espacar(int lines, int columns, Matrix * matriz){

    int cont;
    float aux;
    for(int i = 0; i < lines; i++){
        (*matriz).esplines++;                             //conta quantas linhas não nulas tem na matriz
        (*matriz).matrix = (Pair**) realloc((*matriz).matrix, ((*matriz).esplines)*sizeof(Pair*));    //aloca uma linha a mais
        (*matriz).matrix[i] = NULL;                                  //inicializa a linha
        (*matriz).espcolumns = (int*) realloc((*matriz).espcolumns, ((*matriz).esplines)*sizeof(int)); //aloca um elemento a mais no vetor de colunas
        cont = 0;                       //conta quantos elementos não nulos tem na linha
        for(int j = 0; j < columns; j++){          
            scanf("%f", &aux);      
            if(aux != 0){           
                (*matriz).matrix[i] = (Pair*) realloc((*matriz).matrix[i], (cont+1)*sizeof(Pair));  //aloca um elemento a mais na linha
                (*matriz).matrix[i][j].index = j;                                         //atribui o indice do elemento
                (*matriz).matrix[i][j].val = aux;                                     //atribui o valor do elemento
                cont++;                                                         //incrementa o contador de elementos não nulos
            }
        }
        if(cont == 0)
            (*matriz).esplines--;                  //se a linha não tiver nenhum elemento não nulo, decrementa o contador de linhas não nulas
        else
            (*matriz).espcolumns[i] = cont;    //atribui o numero de elementos não nulos da linha
    }    
}


void setup(Matrix * matriz, int flag){
    if(flag == 0)       //densa
        scanf("%d %d",&(*matriz).lines, &(*matriz).columns);
    if(flag == 1){      // vetor denso
        scanf("%d",&(*matriz).lines);
        (*matriz).columns = 1;
    }
    if(flag == 2){} //esparça

    espacar((*matriz).lines, (*matriz).columns, matriz);

}




int main(){
    int flag;





    pthread_exit(NULL);
}