/*
Se dă un graf orientat care descrie rețeaua de drumuri dintre orașele unui județ.
Fiecare drum are un cost pozitiv (durată, km, taxă). Graful este furnizat într-un 
fișier text astfel:

n                // numărul total de orașe (0 .. n-1), n ≤ 25
u v c            // drum orientat de la orașul u la v, cu cost c (>0)
u v c
…

Pentru această rețea cere-se:
Orașe cu grad intern maxim

Să se afișeze toate orașele care au cel mai mare număr de drumuri de intrare.
Mesaj pentru fiecare:

MULTE DRUMURI DUC LA <id_oras>

Orașe dominante
Se definesc drept orașele care îndeplinesc simultan:

au grad extern maxim (cel mai mare număr de drumuri de ieșire);
au gradExtern > 1.5 × gradIntern.

Pentru fiecare astfel de oraș se va afișa:

ORASUL: <id> COST INTERN <gradExtern> , COST EXTERN <gradIntern>
*/

#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 20

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
			if(graph[i][j] > 9){
                printf("%d ", graph[i][j]);
            }else{
                printf("%d  ", graph[i][j]);
            }
		}
		printf("\n");
	}
	printf("\n");
}

int determinareGradInternMaxim(int size, int graph[][MAX_NODES]){
    int maxim = 0;
    int count = 0;

    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[j][i] != 0){
                count = count + 1;
            }
        }

        if(maxim < count){
            maxim = count;
        }
    }

    return maxim;
}

void determinareOraseGrad(int size, int graph[][MAX_NODES]){
    int maxim = determinareGradInternMaxim(size,graph);
    int count = 0;
    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[j][i] != 0){ // practic aici tu iei 2-1 3-1 4-1....
                count = count + 1;
            }
        }

        if(count == maxim){
            printf("MULTE DRUMURI DUC LA %d\n", i);
        }
    }
}

int determinareGradExternMaxim(int size, int graph[][MAX_NODES]){
    int maxim = 0;
    int count = 0;

    for(int i = 0; i < size; i = i + 1){
        count = 0;
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] != 0){
                count = count + 1;
            }
        }

        if(maxim < count){
            maxim = count;
        }
    }

    return maxim;
}

int determinareGradExtern(int size, int graph[][MAX_NODES], int k){
    int count = 0;

    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            if(graph[i][j] != 0 && i == k){
                count = count + 1;
            }
        }
    }

    return count;
}

int determinareGradIntern(int size, int graph[][MAX_NODES], int k){
    int count = 0;

    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            if(graph[j][i] != 0 && i == k){
                count = count + 1;
            }
        }
    }

    return count;
}


void determinareVarfDeLance(int size, int graph[][MAX_NODES]){
    int grad_extern = determinareGradExternMaxim(size,graph);

    for(int i = 0; i < size; i = i + 1){
        if(determinareGradExtern(size,graph,i) > 1.5 * determinareGradIntern(size,graph,i)
            && determinareGradExtern(size,graph,i) == grad_extern){
                printf("ORASUL: %d COST INTERN %d, COST EXTERN %d\n",
				i, determinareGradExtern(size, graph, i), 
				determinareGradIntern(size, graph, i));
        }
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!(Trebuie ./p input.txt)\n");
        exit(-1);
    }

    int graph[MAX_NODES][MAX_NODES];
    int size = readFromPath(argv[1],graph);
    printMatrice(size,graph);

    printf("Orasele care au grad intern maxim:\n");
    determinareOraseGrad(size,graph);

    printf("\nOrasele care au grad extern maxim si respecta conditia:\n");
    determinareVarfDeLance(size,graph);
    return 0;
}