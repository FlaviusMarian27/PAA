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

void dijkstra(int size, int graph[][MAX_NODES], int start){
    int distance[MAX_NODES];
	int check_list[MAX_NODES] = { 0 };
	int predecesor[MAX_NODES];

    for(int i = 0; i < size; i = i + 1){
        distance[i] = INF;
        predecesor[i] = -1;
    }

    distance[start] = 0;//distanta de la el insusi
    for(int pas = 0; pas < size; pas = pas + 1){
        int cost_minim = INF;
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

        check_list[u] = 1;
        for(int v = 0; v < size; v = v + 1){
            if(graph[u][v] != 0 && distance[v] > distance[u] + graph[u][v]){
                distance[v] = distance[u] + graph[u][v];
                predecesor[v] = u;
            }
        }
    }

    printf("Distantele minime de la nodul %d:\n", start);
	for (int i = 0; i < size; i = i + 1) {
        if (distance[i] == INF) {
			printf("start %d - nod %d - cost INF \n", start, i);
        }else{
            printf("start %d - nod %d - cost %d\t(", start,i, distance[i]);

			int drum[MAX_NODES];
			int len = 0;
			int j = i;

			while (j != -1) {
				drum[len++] = j;
				j = predecesor[j];
			}

			for (int k = len - 1; k >= 0; k = k - 1) {
				printf("%d", drum[k]);
				if (k > 0) {
					printf(" -> ");
				}
			}

			printf(")\n");
        }
    }
}

void bellmanFord(int size, int graph[][MAX_NODES], int start){
    int distance[MAX_NODES];
	int predecesor[MAX_NODES];

    for(int i = 0; i < size; i = i + 1){
        distance[i] = INF;
        predecesor[i] = -1;
    }

    distance[start] = 0;
    //Un drum simplu între două noduri are cel mult n-1 muchii.
    //avem relaxari repetate
    for(int k = 0; k < size - 1; k = k + 1){
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(graph[i][j] != 0 && distance[i] != INF &&
                    distance[j] > distance[i] + graph[i][j]){
                    distance[j] = distance[i] + graph[i][j];
                    predecesor[j] = i;
                }
            }
        }
    }

    for(int k = 0; k < size - 1; k = k + 1){
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(graph[i][j] != 0 && distance[i] != INF &&
                    distance[j] > distance[i] + graph[i][j]){
                    printf("Graful contine ciclu de cost negativ!\n");
				    return;
                }
            }
        }
    }

    printf("Bellman-Ford (start %d):\n", start);
	printf("Nod   Cost   Traseu\n");
	for (int i = 0; i < size; i = i + 1) {
		if (distance[i] == INF) {
			printf("start %d - nod %d - cost INF \n", start, i);
		}
		else {
			if (distance[i] > 9) {
				printf("start %d - nod %d - cost %d (", start, i, distance[i]);
			}
			else {
				printf("start %d - nod %d - cost %d  (", start, i, distance[i]);
			}

			int drum[MAX_NODES];
			int len = 0;
			int j = i;
			while (j != -1) {
				drum[len++] = j;
				j = predecesor[j];
			}

			for (int k = len - 1; k >= 0; k = k - 1) {
				printf("%d", drum[k]);
				if (k > 0) {
					printf(" -> ");
				}
			}
			printf(")\n");
		}
	}
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
	dijkstra(size, graph, start);

    printf("\n");
	bellmanFord(size, graph, 0);
    return 0;
}