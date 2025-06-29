/*
Implementare kruskal si prim
*/
#include<stdio.h>
#include<stdlib.h>

#define MAX_NUMBERS_NODES 25
#define INF 999

void initializareMatrice(int size, int graph[][MAX_NUMBERS_NODES]){
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            graph[i][j] = 0;
        }
    }
}

int readFromPath(const char* path, int graph[][MAX_NUMBERS_NODES]){
    int size = 0;

    FILE* fin = fopen(path,"r");
    if(fin == NULL){
        perror("Eroare la deschiderea fisierului!\n");
        exit(-1);
    }

    if(fscanf(fin,"%d",&size) != 1 || size > MAX_NUMBERS_NODES){
        perror("Eroare la citirea dimensiunii!\n");
        exit(-1);
    }

    initializareMatrice(size,graph);
    int i = 0;
    int j = 0;
    int cost = 0;
    while(fscanf(fin,"%d %d %d",&i,&j,&cost) == 3){
        graph[i][j] = cost;
        graph[j][i] = cost;
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(-1);
    }

    return size;
}

void printMatrice(int size, int graph[][MAX_NUMBERS_NODES]) {
	printf("Matricea de adiacenta(%d x %d):\n", size, size);
	for (int i = 0; i < size; i = i + 1) {
		for (int j = 0; j < size; j = j + 1) {
			if (graph[i][j] > 9) {
				printf("%d ", graph[i][j]);
			}else{
				printf("%d  ", graph[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void prim(int size, int graph[][MAX_NUMBERS_NODES]){
    int check_list[MAX_NUMBERS_NODES] = { 0 };
	check_list[0] = 1;
	int cost_total = 0;

    for(int pas = 1; pas < size; pas = pas + 1){
        int cost_minim = INF;
        int minim_i = -1;
        int minim_j = -1;
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(graph[i][j] != 0 && cost_minim > graph[i][j] 
                    && check_list[i] != check_list[j]){
                        cost_minim = graph[i][j];
                        minim_i = i;
                        minim_j = j;
                }
            }
        }

        if(minim_i != -1 && minim_j != -1){
            check_list[minim_j] = 1;
            printf("%d - %d -> cost %d\n", minim_i, minim_j, graph[minim_i][minim_j]);
            cost_total = cost_total + graph[minim_i][minim_j];
        }
    }
    printf("Costul total este de %d!\n", cost_total);
}

void kruskal(int size, int graph[][MAX_NUMBERS_NODES]){
    int check_list[MAX_NUMBERS_NODES] = { 0 };
	int cost_total = 0;

    for(int i = 0; i < size; i = i + 1){
        check_list[i] = i;
    }

    for(int pas = 1; pas < size; pas = pas + 1){
        int cost_minim = INF;
        int minim_i = -1;
        int minim_j = -1;
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(graph[i][j] != 0 && cost_minim > graph[i][j]
                && check_list[i] != check_list[j]){
                    cost_minim = graph[i][j];
                    minim_i = i;
                    minim_j = j;
                }
            }
        }

        if(minim_i != -1 && minim_j != -1){
            printf("%d - %d -> cost %d\n", minim_i, minim_j, graph[minim_i][minim_j]);
            cost_total = cost_total + graph[minim_i][minim_j];
            int old = check_list[minim_j];
            int new = check_list[minim_i];
            for(int i = 0; i < size; i = i + 1){
                if(check_list[i] == old){
                    check_list[i] = new;
                }
            }
        }
    }

    printf("Costul total este de %d!\n", cost_total);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!(Trebuie ./p input.txt)\n");
        exit(-1);
    }

    int graph[MAX_NUMBERS_NODES][MAX_NUMBERS_NODES];
    int size = readFromPath(argv[1],graph);
    printMatrice(size,graph);

    printf("Algoritmul lui Prim:\n");
    prim(size,graph);

    printf("\nAlgoritmul lui Kruskal:\n");
    kruskal(size,graph);

    return 0;
}