/*
O companie de transport operează rute între mai multe orașe. Unele rute au 
costuri pozitive (taxe de drum, combustibil), iar altele pot avea costuri 
negative (subvenții, reduceri, rambursări din sistem).

Graful este:
- orientat
- costurile pot fi pozitive, zero sau negative
- pot exista cicluri, dar nu trebuie să existe cicluri negative

🎯 Cerințe:
- Să se determine costul minim pentru a ajunge dintr-un oraș de pornire start în toate 
celelalte.
- Să se afișeze traseul complet pentru fiecare oraș.
- Dacă există ciclu de cost negativ, să se afișeze un mesaj și să se oprească execuția.

==========================================================================
a. Structura de date folosită
- am ales folosirea unei matrice de adiacenta pentru reprezentarea grafului orientat 
ponderat. Pentru Bellman Ford am folosit un tablou de distante[] pt a salva costul minim 
si un tablou de predecsori pt a reprezenta drumul optim format din nodurile parcurse.

b. Justificarea alegerii
- am ales reprezentare grafului orientat prin intermediul unei matrice de adiacenta,
deoarece numarul nodurilor nu se modifica des si avem un numar mic de noduri totale,
iar folosirea unei liste simplu inlantuite nu aducea un plus de performanta. Un alt 
avatanj ar fi ca nu folosim alocarea dinamic, deci nu exista memory leak- uri, dimensiuniele
fiind fixe.

c.Exemplu
Input:
5
0 1 -2
0 2 2
1 2 -9
3 0 -5
3 4 6
1 3 -7
4 1 -1
4 2 4

Output:
Graful contine ciclu de cost negativ!
--------------------------------------------------------------------

Input:
5
0 1 -2
0 2 2
1 2 -9
3 0 -5
3 4 6
1 3 7
4 1 -1
4 2 4

Output:
Toate rutele cele mai scurte din 0:
Oras de start 0 <---> Orasul 0: Cost 0(0)
Oras de start 0 <---> Orasul 1: Cost -2(0 -> 1)
Oras de start 0 <---> Orasul 2: Cost -11(0 -> 1 -> 2)
Oras de start 0 <---> Orasul 3: Cost 5(0 -> 1 -> 3)
Oras de start 0 <---> Orasul 4: Cost 11(0 -> 1 -> 3 -> 4)

d. Raționamentul pe scurt
- am ales algoritmul lui Bellman Ford pentru determinarea celui mai scurt drum si 
determianrea ciclurilor negative. Practic initializam toate distantele pe INF,
relaxam continuu distantele si cand determinam un ciclu negativ ne oprim din parcurgerea 
nodurilor(Dacă la final mai există o relaxare posibilă, înseamnă că există un ciclu 
negativ). Folosim vectorul de predecesori pt a putea afisa drum parcurs de la nodul
x la nodul y.

*/

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
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void bellmanFord(int size, int graph[][MAX_NODES], int start){
    int distance[MAX_NODES];
    int predecesor[MAX_NODES];

    for(int i = 0; i < size; i = i + 1){
        distance[i] = INF;
        predecesor[i] = -1;
    }

    distance[start] = 0;//costul 0 la el insusi
    for(int k = 0; k < size - 1; k = k + 1){
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(graph[i][j] != 0 && distance[i] != INF 
                    && distance[j] > distance[i] + graph[i][j]){
                    distance[j] = distance[i] + graph[i][j];
                    predecesor[j] = i;
                }
            }
        }
    }

    for(int k = 0; k < size - 1; k = k + 1){
        for(int i = 0; i < size; i = i + 1){
            for(int j = 0; j < size; j = j + 1){
                if(graph[i][j] != 0 && distance[i] != INF
                    && distance[j] > distance[i] + graph[i][j]){
                    printf("Graful contine ciclu de cost negativ!\n");
				    return;
                }
            }
        }
    }

    printf("\nToate rutele cele mai scurte din %d:\n",start);
    for(int i = 0; i < size; i = i + 1){
        if(distance[i] == INF){
            printf("Oras de start %d <---> Orasul %d: Cost INF\n",start,i);
        }else{
            printf("Oras de start %d <---> Orasul %d: Cost %d(",start,i,distance[i]);
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
            printf(")\n");
        }
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!(Trebuie ./p input3.txt)\n");
        exit(-1);
    }

    int graph[MAX_NODES][MAX_NODES];
    int size = readFromPath(argv[1],graph);
    printMatrice(size,graph);

    int start = 0;
    bellmanFord(size,graph,start);
    return 0;
}