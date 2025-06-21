/*
Analiza Rețelelor Sociale
Cerințe pentru finalizare
O platformă de socializare dorește să analizeze conexiunile utilizatorilor pentru 
a înțelege formarea comunității. Având în vedere datele despre prieteniile dintre 
utilizatori, platforma trebuie să:

Identifice grupuri izolate de prieteni care nu au conexiuni cu alte grupuri
Determine dimensiunea celui mai mare cerc de prieteni
Găsească utilizatori care ar putea servi drept punți între diferite comunități
Calculeze câte clustere sociale separate există în rețea
Vi se oferă un set de date cu prieteniile utilizatorilor, fiecare prietenie fiind 
bidirecțională. Proiectați un algoritm pentru a analiza aceste conexiuni sociale 
și a răspunde la întrebări despre structura comunității.
========================================================================================
1) Structura de date aleasa
- structura de date aleasa a fost o matrice de adiacenta pentru a putea reprezenta
un graf neorientat ponderat. Astfel doua noduri pot fi prieteni daca ponderea dintre cele 
2 este egala cu 1.

2)Justificare
- am ales folosirea unei matrice de adiacenta pentru ca numarul de noduri nu se modifica
des, avem un numar mic de noduri, iar un alt avantaj ar fi ca nu folosim liste simplu 
inlantuite pentru ca avem un numar mic de noduri, deci nu ar putea sa aduca un plus pt 
performanta rezultand in cele din urma ca nu folosim alocari dinamice, deci rezulta ca 
nu vom avea memory leaks uri. Matricea de adiacenta mai o ofera un avantaj pentru 
vizualizarea mai usoara a legaturilor dintre noduri, codul este unul simplu, accesul 
la valori fiind O(1).

*/
#include<stdio.h>
#include<stdlib.h>

#define MAX_NUMBERS_NODES 25

void initializare(int size, int graph[][MAX_NUMBERS_NODES]){
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

    initializare(size,graph);
    int i = 0;
    int j = 0;
    while(fscanf(fin,"%d %d",&i,&j) == 2){
        graph[i][j] = 1;
        graph[j][i] = 1;
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(-1);
    }

    return size;
}

void printMatrice(int size, int graph[][MAX_NUMBERS_NODES]){
    printf("Matricea de adiacenta(%d x %d):\n",size,size);
    for(int i = 0; i < size; i = i + 1){
        for(int j = 0; j < size; j = j + 1){
            printf("%d ",graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//1)==============================================================
void DFS(int size, int graph[][MAX_NUMBERS_NODES], int start, int check_list[MAX_NUMBERS_NODES]){
    check_list[start] = 1;
    for(int i = 0; i < size; i = i + 1){
        if(check_list[i] == 0 && graph[start][i] != 0){
            DFS(size,graph,i,check_list);
        }
    }   
}

int determinareNrGrupuri(int size, int graph[][MAX_NUMBERS_NODES]){
    int check_list[MAX_NUMBERS_NODES] = {0};
    int nr_comp = 0;
    for(int i = 0; i < size; i = i + 1){
        if(check_list[i] == 0){
            DFS(size,graph,i,check_list);//apelam de fiecare functia ca sa putem sa parcurgem
            nr_comp = nr_comp + 1;
        }
    }

    return nr_comp;
}

//2)==================================================================
int determinareDimensiuneGrup(int size, int graph[][MAX_NUMBERS_NODES], 
    int start, int check_list[MAX_NUMBERS_NODES]){
    check_list[start] = 1;
    int nr_comp = 1;//presupunem ca deja exista un nod
    for(int i = 0; i < size; i = i + 1){
        if(check_list[i] == 0 && graph[start][i] != 0){
            nr_comp = nr_comp + determinareDimensiuneGrup(size,graph,i,check_list);
        }
    }

    return nr_comp;
}

int determinareCelMaiMareGrup(int size, int graph[][MAX_NUMBERS_NODES]){
    int maxim = 0;
    int check_list[MAX_NUMBERS_NODES] = {0};

    for(int i = 0; i < size; i = i + 1){
        int current = determinareDimensiuneGrup(size,graph,i,check_list);
        if(maxim < current){
            maxim = current;
        }
    }

    return maxim;
}

//3)=================================================
int determinareNrCompDupaStergere(int size, int graph[][MAX_NUMBERS_NODES], int interzis){
    int check_list[MAX_NUMBERS_NODES] = {0};
    check_list[interzis] = 1;
    int nr_comp = 0;   

    for(int i = 0; i < size; i = i + 1){
        if(check_list[i] == 0){
            DFS(size,graph,i,check_list);
            nr_comp = nr_comp + 1;
        }
    }

    return nr_comp;
}

void determinarePuncteDeArticulatie(int size, int graph[][MAX_NUMBERS_NODES]){
    int nr_comp = determinareNrGrupuri(size,graph);
    int gasit = 0;

    printf("3) Punctele de articulatie (utilizatori care sunt punti):\n");
    for(int i = 0; i < size; i = i + 1){
        int current = determinareNrCompDupaStergere(size,graph,i);
        if(nr_comp < current){
            printf("Utilizatorul %d este un punct de articulatie\n", i);
			gasit = 1;
        }
    }

    if (gasit == 0) {
		printf("Nu exista puncte de articulatie in retea.\n");
	}
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!(Trebuie ./p input3.txt)\n");
        exit(-1);
    }

    int graph[MAX_NUMBERS_NODES][MAX_NUMBERS_NODES];
    int size = readFromPath(argv[1],graph);
    printMatrice(size,graph);

    printf("1) Numarul de grupuri este %d\n",determinareNrGrupuri(size,graph));
    printf("2) Grupul cel mai mare are %d prieteni\n",determinareCelMaiMareGrup(size,graph));
    determinarePuncteDeArticulatie(size,graph);

    return 0;
}