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

int readFromPath(const char* path, int graph[][NODES_MAX]){
    int size = 0;

    FILE* fin = fopen(path,"r");
    if(fin == NULL){
        perror("Eroare la deschidere!\n");
        exit(-1);
    }

    if(fscanf(fin," %d",&size) != 1){
        perror("Eroare la citirea dimensiunii!\n");
        exit(-1);
    }

    initializare(size,graph);

    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            if(fscanf(fin,"%d",&graph[i][j]) != 1){
                perror("Eroare citirea matrice!\n");
                exit(-1);
            }
        }
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchidere!\n");
        exit(-1);
    }

    return size;
}

void printMatrice(int size, int graph[][NODES_MAX]){
    printf("Matricea(%d x %d):\n",size,size);
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            printf("%d ", graph[i][j]);
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

void analizaProduse(int size, int graph[][NODES_MAX]){
    int ok = 0;
    for(int j = 0; j < size; j = j + 1){
        printf("Produsul %d poate fi afectat de furnizorii: ", j + 1);
        ok = 0;
        for(int i = 0; i < size; i = i + 1){
            if(graph[i][j] == 1){
                printf("%d ",i + 1);
                ok = 1;
            }
        }

        if(ok == 0){
            printf("Niciunul");
        }
        printf("\n");
    }
}

void analizaFurnizori(int size, int graph[][NODES_MAX]){
    int ok = 0;

    for(int i = 0; i < size; i = i + 1){
        printf("Dacă furnizorul %d cade, se afectează: ", i + 1);
        ok = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] == 1){
                printf("%d ",j + 1);
                ok = 1;
            }
        }   
        
        if(ok == 0){
            printf("Niciunul");
        }
        printf("\n");
    }
}

void analizaVulnerabilitate(int size, int graph[][NODES_MAX]){
    int count = 0;
    int maxim = 0;

    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] == 1){
                count = count + 1;
            }
        }

        if(maxim < count){
            maxim = count;
        }
    }

    printf("\n");
    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] == 1){
                count = count + 1;
            }
        }

        if(maxim == count){
            printf("Furnizorii %d are vulnerabilitatea maxima (%d noduri afectate): ",i + 1, maxim);
            for (int k = 0; k < size; k = k + 1) {
				if (graph[i][k] == 1) {
					printf("%d ", k + 1);
				}
			}
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    int graph[NODES_MAX][NODES_MAX];
    int size = readFromPath(argv[1],graph);
    printMatrice(size,graph);

    int distance[NODES_MAX][NODES_MAX];
    floydWarshall(size,graph,distance);

    analizaProduse(size,distance);

    printf("\n");
    analizaFurnizori(size,distance);

    analizaVulnerabilitate(size,distance);
    return 0;
}