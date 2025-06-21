#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 25
#define INF 9999

void initializareMatrice(int size, int graph[][MAX_NODES]){
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            graph[i][j] = 0;
        }
    }
}

int readFromPath(const char* path, int graph[][MAX_NODES]){
    int size = 0;

    FILE* fin = fopen(path,"r");
    if(fin == NULL){
        perror("Eroare la deschiderea fisierului!\n");
        exit(-1);
    }

    if(fscanf(fin,"%d",&size) != 1 || size > MAX_NODES){
        perror("Eroare la citirea dimensiunii!\n");
        exit(-1);
    }

    initializareMatrice(size,graph);
    int i = 0;
    int j = 0;
    int cost = 0;
    while(fscanf(fin,"%d %d %d",&i,&j,&cost) == 3){
        graph[i][j] = cost;
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(-1);
    }

    return size;
}

void printMatrice(int size, int graph[][MAX_NODES]) {
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

void determinareCostDrum(int size, int graph[][MAX_NODES], int start){
    int distance[MAX_NODES];
    int check_list[MAX_NODES] = {0};
    int predecesor[MAX_NODES];

    for(int i = 0; i < size; i = i + 1){
        distance[i] = INF;
        predecesor[i] = -1;
    }

    distance[start] = 0;
    for(int pas = 0; pas < size - 1; pas = pas + 1){
        int cost_minim = INF;
        int u = -1;
        for(int i = 0; i < size; i = i + 1){
            if(check_list[i] != 1 && cost_minim > distance[i]){
                cost_minim = distance[i];
                u = i;
            }
        }

        if(u == -1){
            break;
        }

        check_list[u] = 1;
        for(int v = 0; v < size; v = v + 1){
            if(graph[u][v] != 0 && distance[v] > distance[u] + graph[u][v]){
                distance[v] = distance[u] + graph[u][v];
                predecesor[v] = u;
            }
        }
    }

    printf("\nLista cu toate drumurile de la orasul %d:\n",start);
    for(int i = 0; i < size; i = i + 1){
        if(distance[i] == INF){
            printf("Orasul de start: %d <---> Orasul %d - cost INF\n",start,i);
        }else{
            printf("Orasul de start: %d <---> Orasul %d - cost %d\t(Orasele: ",start,i,distance[i]);
            int drum[MAX_NODES];
            int len = 0;
            int j = i;
            while(j != -1){
                drum[len++] = j;
                j = predecesor[j];
            }

            for(int k = len - 1; k >= 0; k = k - 1){
                printf("%d", drum[k]);
				if (k > 0) {
					printf(" -> ");
				}
            }
            printf(")\n");
        }
    }
}

void determinareCelMaiScurtDrum(int size, int graph[][MAX_NODES], int start, int end){
    int distance[MAX_NODES];
    int predecesor[MAX_NODES];
    int check_list[MAX_NODES] = {0};

    for(int i = 0 ; i < size; i = i + 1){
        distance[i] = INF;
        predecesor[i] = -1;
    }

    distance[start] = 0;//mereu initializat
    for(int pas = 0; pas < size - 1; pas = pas + 1){
        int cost_minim = INF;//mereu pe INF
        int u = -1;
        for(int i = 0; i < size; i = i + 1){
            if(check_list[i] == 0 && cost_minim > distance[i]){
                cost_minim = distance[i];
                u = i;
            }
        }

        if(u == -1){
            break;
        }

        check_list[u] = 1;//marcat ca vizitat
        for(int v = 0; v < size; v = v + 1){
            if(graph[u][v] != 0 && distance[v] > distance[u] + graph[u][v]){
                distance[v] = distance[u] + graph[u][v];
                predecesor[v] = u;
            }
        }
    }

    printf("\nCel mai scurt drum de la %d la %d este:\n",start,end);
    for(int i = 0; i < size; i = i + 1){
        if(distance[i] != INF && i == end){
            printf("Costul: %d => ",distance[i]);
            int drum[MAX_NODES];
            int len = 0;
            int j = i;
            
            while(j != -1){
                drum[len++] = j;
                j = predecesor[j];
            }

            for(int k = len - 1; k >= 0; k = k - 1){
                printf("%d",drum[k]);
                if(k > 0){
                    printf(" -> ");
                }
            }
            printf("\n");
        }else if(distance[i] == INF && i == end){
            printf("Nu exista un drum! Probabil avem un drum izolat!\n");
        }
    }
}

void dfsAllPaths(int size, int graph[][MAX_NODES], int start, int end, int check_list[], int path[], int path_len){
    check_list[start] = 1;
    path[path_len++] = start;
    
    if(start == end){
        for(int i = 0; i < path_len; i = i + 1){
            printf("%d", path[i]);
            if(i < path_len - 1){
                printf(" -> ");
            }
        }
        printf("\n");
    }else{
        for(int j = 0; j < size; j = j + 1){
            if(check_list[j] == 0 && graph[start][j] != 0){
                dfsAllPaths(size,graph,j,end,check_list,path,path_len);
            }
        }
    }

    check_list[start] = 0;//pentru a reseta mereu drumul
}

int main(int argc, char *argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    int graph[MAX_NODES][MAX_NODES];
    int size = readFromPath(argv[1],graph);
    printMatrice(size,graph);

    int start = 0;
    determinareCostDrum(size,graph,start);

    int end = 7;
    determinareCelMaiScurtDrum(size,graph,start,end);

    int visited[MAX_NODES] = {0};
    int path[MAX_NODES];

    printf("\nToate drumurile posibile de la %d la %d:\n", start, end);
    dfsAllPaths(size, graph, start, end, visited, path, 0);

    return 0;
}