#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
    int maxcolumns; //maxcolumns
}Matrix;

Matrix defaultzin = {NULL, 0, 0, 0, NULL, 0};
Matrix aux = {NULL, 0, 0, 0, NULL, 0};
Matrix result = {NULL, 0, 0, 0, NULL, 0};

void espsarinput(int lines, int columns, Matrix * matriz){

    int cont, flag = 0;
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
        if(cont == 0){
            (*matriz).esplines--; //se a linha não tiver nenhum elemento não nulo, decrementa o contador de linhas não nulas
            if(i == lines-1) 
                flag = 1; //se a linha for a ultima, seta a flag para 1)                
        }
        else{
            (*matriz).espcolumns[i] = cont;    //atribui o numero de elementos não nulos da linha
            if(cont > (*matriz).maxcolumns)    //se o numero de elementos não nulos da linha for maior que o maximo, atualiza o maximo
                (*matriz).maxcolumns = cont;
        }
    }    
        if(flag){ // se a flag for 1, significa que a ultima linha não tem nenhum elemento não nulo
            Pair *aux_free_line = (*matriz).matrix[(*matriz).esplines]; //auxiliar para liberar a ultima linha
            (*matriz).matrix = (Pair**) realloc((*matriz).matrix, ((*matriz).esplines)*sizeof(Pair*)); //realoca o vetor de linhas para o tamanho correto
            free(aux_free_line);
            (*matriz).espcolumns = (int*) realloc((*matriz).espcolumns, ((*matriz).esplines)*sizeof(int)); //realoca o vetor de colunas para o tamanho correto
        }
}
   void espasarwithoutinput(int lines, int columns, Matrix * matriz, Matrix * esparsa){
        int cont, flag = 0;
        float aux;
        for(int i = 0; i < lines; i++){
            (*esparsa).esplines++;                             //conta quantas linhas não nulas tem na matriz
            (*esparsa).matrix = (Pair**) realloc((*matriz).matrix, ((*esparsa).esplines)*sizeof(Pair*));    //aloca uma linha a mais
            (*esparsa).matrix[i] = NULL;                                  //inicializa a linha
            (*esparsa).espcolumns = (int*) realloc((*matriz).espcolumns, ((*esparsa).esplines)*sizeof(int)); //aloca um elemento a mais no vetor de colunas
            cont = 0;                       //conta quantos elementos não nulos tem na linha
            for(int j = 0; j < columns; j++){          
                if(((*matriz).matrix[i][j].val) != 0){           
                    (*esparsa).matrix[i] = (Pair*) realloc((*esparsa).matrix[i], (cont+1)*sizeof(Pair));  //aloca um elemento a mais na linha
                    (*esparsa).matrix[i][j].index = j;                                         //atribui o indice do elemento
                    (*esparsa).matrix[i][j].val = (*matriz).matrix[i][j].val;                                     //atribui o valor do elemento
                    cont++;                                                         //incrementa o contador de elementos não nulos
                }
            }
            if(cont == 0){
                (*esparsa).esplines--; //se a linha não tiver nenhum elemento não nulo, decrementa o contador de linhas não nulas
                if(i == lines-1) 
                    flag = 1; //se a linha for a ultima, seta a flag para 1)                
            }
            else{
                (*esparsa).espcolumns[i] = cont;    //atribui o numero de elementos não nulos da linha
                if(cont > (*esparsa).maxcolumns)    //se o numero de elementos não nulos da linha for maior que o maximo, atualiza o maximo
                    (*esparsa).maxcolumns = cont;
            }
        }    
            if(flag){ // se a flag for 1, significa que a ultima linha não tem nenhum elemento não nulo
                Pair *aux_free_line = (*matriz).matrix[(*matriz).esplines]; //auxiliar para liberar a ultima linha
                (*esparsa).matrix = (Pair**) realloc((*matriz).matrix, ((*matriz).esplines)*sizeof(Pair*)); //realoca o vetor de linhas para o tamanho correto
                free(aux_free_line);
                (*esparsa).espcolumns = (int*) realloc((*matriz).espcolumns, ((*matriz).esplines)*sizeof(int)); //realoca o vetor de colunas para o tamanho correto
            }
    }

void setup(int flag, int flag2, int columns){
    if(flag == 0){       //matriz densa
        
        if(flag2 == 0){  // apenas ler.
            printf("Digite o numero de linhas da matriz densa: ");        
            scanf("%d",&defaultzin.lines);
            printf("Digite o numero de colunas matriz densa: ");
            scanf("%d",&defaultzin.columns);
            espsarinput(defaultzin.lines, defaultzin.columns, &defaultzin);
        }
        if(flag2 == 1){           // ler com objetivo de usar para multi.
            printf("Digite o numero de linhas  da matriz densa (deve ser maior ou igual a %d): ", columns);        
            scanf("%d",&aux.lines);
            while(aux.lines < columns){
                printf("Numero de linhas invalido (deve ser maior ou igual a %d). Digite novamente: ", columns);
                scanf("%d",&aux.lines);
            }
            printf("Digite o numero de colunas da matriz densa: ");
            scanf("%d",&aux.columns);
            for(int i = 0; i < aux.lines; i++){                     //leitura da matriz
                aux.matrix = (Pair**) realloc(aux.matrix, (i+1)*sizeof(Pair*));
                aux.matrix[i] = NULL;
                for(int j = 0; j < aux.columns; j++){
                    aux.matrix[i] = (Pair*) realloc(aux.matrix[i], (j+1)*sizeof(Pair));
                    printf("Digite o elemento da linha %d e coluna %d: ", i, j);
                    scanf("%f", &aux.matrix[i][j].val);
                    aux.matrix[i][j].index = j;
                }
                printf("\n");
            }
        }
    }

    if(flag == 1){      // vetor denso
        printf("Digite o numero de linhas do vetor denso (deve ser maior ou igual a %d): ", columns);
        scanf("%d",&aux.lines);
        aux.columns = 1;
        for(int i = 0; i < aux.lines; i++){                    //leitura do vetor
            aux.matrix = (Pair**) realloc(aux.matrix, (i+1)*sizeof(Pair*));
            aux.matrix[i] = NULL;
            aux.matrix[i] = (Pair*) realloc(aux.matrix[i], sizeof(Pair));
            printf("Digite o elemento da linha %d: ", i);
            scanf("%f", &aux.matrix[i][0].val);
            aux.matrix[i][0].index = 0;
        }
    }
    
    if(flag == 2){ // matriz esparsa
        printf("Digite o numero de linhas da matriz esparsa: ");
        scanf("%d",&aux.lines);
        printf("Digite o numero de colunas da matriz esparsa (tamanho da linha com mais elementos): ");
        scanf("%d",&aux.columns);
        printf("Digite os elementos da matriz esparsa (caso seja nulo, digite 0): \n");
        espsarinput(aux.lines, aux.columns, &aux);
    } 
}

void* multiply(void* threadid) {
    int id = *((int*)threadid);
    for(int j = 0; j < aux.columns; j++) {
        result.matrix[id][j].val = 0; // Resultado da multiplicação
        for(int k = 0; k < aux.columns; k++) {
            if( id > (defaultzin.esplines - 1) || k > (defaultzin.espcolumns[id] -1) || (defaultzin.matrix[id][k].index) != k)
            result.matrix[id][j].val += 0;
            else
            result.matrix[id][j].val += defaultzin.matrix[id][k].val * aux.matrix[k][j].val;
        }
    }
    pthread_exit(NULL);
}

void* multiply_sparse_vector(void* threadid) {
    int id = *((int*)threadid);
    for (int j = 0; j < aux.columns; j++) {
        result.matrix[id][j].val = 0; // Inicializa o resultado da multiplicação
        for (int k = 0; k < aux.columns; k++) {
            if (id > (aux.esplines - 1) || k > (aux.espcolumns[id] - 1) || (aux.matrix[id][k].index) != k)
                continue; // Se os índices não correspondem ou são zero, pule para a próxima iteração
            result.matrix[id][j].val += aux.matrix[id][k].val * aux.matrix[k][0].val; // Multiplicação
        }
    }
    pthread_exit(NULL);
}


void freer(Matrix *matriz){
    for(int i = 0; i < (*matriz).esplines; i++){
        free((*matriz).matrix[i]);
    }
    free((*matriz).matrix);
    free((*matriz).espcolumns);
}


int main(){
    int option;
    pthread_t* threads = NULL;
    int* thread_ids = NULL;
    while(1){
        printf("----------Menu----------\n");
        printf("Digite o tipo de matriz que deseja usar: \n");
        printf("1 - ler Matriz densa padrão\n");
        printf("2 - multiplicar a Matriz padrão por um Vetor denso\n");
        printf("3 - multiplicar a Matriz padrão por uma Matriz esparsa\n");
        printf("4 - multiplicar a Matriz padrão por uma Matriz densa\n");
        printf("5 - Sair\n");

        scanf("%d", &option);

        switch (option)
        {
        case 1:
            setup(0, 0, defaultzin.columns);
            break;
        case 2:
            setup( 1, 0, defaultzin.columns);
            threads = malloc(aux.lines * sizeof(pthread_t));
            thread_ids = malloc(aux.lines * sizeof(int));
            result.matrix = (Pair *) malloc(defaultzin.esplines *sizeof(Pair));
            for(int i = 0; i < defaultzin.esplines; i++)
                result.matrix[i] = (Pair *) malloc(1*sizeof(Pair));
            result.lines = defaultzin.esplines;
            result.columns = 1;
            for(int i = 0; i < aux.lines; i++) {   
                thread_ids[i] = i;
                pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
            }
            for(int i = 0; i < aux.lines; i++) {
                pthread_join(threads[i], NULL);
            }
            break;
        case 3:
            setup(2, 0, 0);
            threads = malloc(aux.esplines*sizeof(pthread_t));
            
            break;
        case 4:
            setup( 0, 1, defaultzin.columns);
            threads = malloc(aux.lines * sizeof(pthread_t));
            thread_ids = malloc(aux.lines * sizeof(int));
            result.lines = defaultzin.lines;
            result.columns = aux.columns;
            for(int i = 0; i < aux.lines; i++) {  
                thread_ids[i] = i;
                pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
            }
            for(int i = 0; i < aux.lines; i++) {
                pthread_join(threads[i], NULL);
            }
            break;
        case 5:
            pthread_exit(NULL);
            break;
        default:
            printf("Opção inválida\n");
            break;
        }
            Matrix result_esparsa;
            espasarwithoutinput()
            Matrix aux_free = defaultzin;
            Matrix aux_free2 = aux;
            int * id_free = thread_ids;
            pthread_t * thread_free = threads;
            threads = NULL;
            thread_ids = NULL;
            defaultzin = result;
            freer(&aux_free);
            freer(&aux_free2);
            free(id_free);
            free(thread_free); 
            result.matrix = NULL;   // criar dois novos nodes
            


            
            // Assume que você deseja adicionar dois nós na primeira linha de result.matrix
            result.matrix = (Pair**)realloc(result.matrix, (result.esplines + 2) * sizeof(Pair*));

            // Aloca espaço para os dois novos nós
            result.matrix[result.esplines] = (Pair*)malloc(2 * sizeof(Pair));

            // Define os valores dos nós
            result.matrix[result.esplines][0].index = 0;
            result.matrix[result.esplines][0].val = 1.0; // Defina o valor desejado para o primeiro nó

            result.matrix[result.esplines][1].index = 1;
            result.matrix[result.esplines][1].val = 2.0; // Defina o valor desejado para o segundo nó

            // Atualiza as informações da matriz result
            result.esplines += 1;
            result.maxcolumns = 2;



            result.lines = 0;
            result.columns = 0;
            result.esplines = 0;
            result.espcolumns = NULL;
            result.maxcolumns = 0;
            aux.matrix = NULL;
            aux.lines = 0;
            aux.columns = 0;
            aux.esplines = 0;
            aux.espcolumns = NULL;
            aux.maxcolumns = 0;
            
    }
}