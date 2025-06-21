#include<stdio.h>
#include<stdlib.h>

#define NODES_MAX 20

void initializare(int size, int graph[][NODES_MAX]){
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            graph[i][j] = 0;
        }
    }
}

void readFromPath(const char* path, int graph[][NODES_MAX], int* size_current, int* sizeMax){

    FILE* fin = fopen(path,"r");
    if(fin == NULL){
        perror("Eroare la deschidere!\n");
        exit(-1);
    }

    if(fscanf(fin,"%d", sizeMax) != 1){
        perror("Eroare la citirea permisiuniilor maxime!\n");
        exit(-1);
    }

    if(fscanf(fin,"%d", size_current) != 1 || *size_current > 20){
        perror("Eroare la citirea permisiuniilor existente!\n");
        exit(-1);
    }

    initializare(*size_current,graph);

    for(int i = 0; i < *size_current; i = i + 1){
        for(int j = 0; j < *size_current; j = j + 1){
            if(fscanf(fin,"%d",&graph[i][j]) != 1){
                perror("Eroare la citirea matricei!\n");
                exit(-1);
            }
        }
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchidere!\n");
        exit(-1);
    }

}

void printMatrice(int size, int graph[][NODES_MAX]){
    printf("Matricea(%d x %d):\n",size,size);
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            printf("%d ",graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void floydWarshall(int size, int graph[][NODES_MAX], int distance[][NODES_MAX]){
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            distance[i][j] = graph[i][j];
        }
    }

    for(int k = 0; k < size; k = k + 1){
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(distance[i][k] == 1 && distance[k][j] == 1){
                    distance[i][j] = 1;
                }
            }
        }
    }

    printf("\nMatricea de dependente(%d x %d):\n",size,size);
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            printf("%d ",distance[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void afisarePermisiuniMaxime(int size, int graph[][NODES_MAX]){
    int maxim = 0;
    int count = 0;

    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] == 1 && i != j){
                count = count + 1;
            }
        }
        
        if(count > maxim){
            maxim = count;
        }
    }

    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] == 1 && i != j){
                count = count + 1;
            }
        }
        
        if(count == maxim){
            printf("Permisiunea maxima: %d\n", i + 1);
        }
    }

}

void afisarePermisiuniRisc(int size, int prag_risc, int distance[][NODES_MAX]){
    int count = 0;
    int ok = 0;
    
    printf("\nPermisiuni care depasesc pragul de risc (%d):\n", prag_risc);
    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(distance[i][j] == 1 && i != j){
                count = count + 1;
            }
        }

        if(count > prag_risc){
            ok = 1;
            printf("%d ", i + 1);
        }
    }

    if(ok == 0){
        printf("Nu exista prag de risc!\n");
    }

    printf("\n");
}

int main( int argc, char* argv[] ){
    if(argc != 2){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    int permisiuni_maxime = 0;
    int permisiuni_curente = 0;
    int graph[NODES_MAX][NODES_MAX];
    readFromPath(argv[1],graph,&permisiuni_curente,&permisiuni_maxime);
    printMatrice(permisiuni_curente,graph);

    int distance[NODES_MAX][NODES_MAX];
    floydWarshall(permisiuni_curente,graph,distance);

    afisarePermisiuniMaxime(permisiuni_curente,distance);

    afisarePermisiuniRisc(permisiuni_curente,permisiuni_maxime,distance);

    return 0;
}