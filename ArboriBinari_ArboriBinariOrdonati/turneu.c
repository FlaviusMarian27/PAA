/*
Sistemul de tabele de turneu
Cerințe pentru finalizare
Un organizator de turnee sportive trebuie să gestioneze tabelele de eliminare 
pentru 64 de echipe. Fiecare meci elimină o echipă, iar câștigătorii avansează 
în runda următoare până când este stabilit un campion. Sistemul trebuie să:

Urmări ce echipe avansează în fiecare rundă
Determine calea pe care a parcurs-o fiecare echipă pentru a ajunge la poziția finală
Calculeze numărul minim de meciuri necesar
Găsească toate echipele care au fost eliminate de câștigătorul final
Proiecteze un algoritm pentru a reprezenta această structură de turneu și să 
implementeze funcții pentru a răspunde la întrebări despre progresul echipei și 
rezultatele meciurilor.

            3
        /       \
       3         5
     /   \      / \
    2      3   5   6
   / \    / \
  1   2  3   4

=========================================================================================
a.Structura de date aleasa
- Am ales să folosesc un arbore binar, în care fiecare nod reprezintă un meci, iar 
valoarea stocată este echipa câștigătoare. Fiecare nod are doi fii: stânga și dreapta
care sunt echipele participante la acel meci.

b.Justificare
- am ales sa ma folosesc de un arbore binar, deoarece reprezinta cel mai bine structura
unui turneu, practic un meci fiind disputat fix intre doua echipe ceea ce dupa la un
arbore binar. Astfel mereu trece o echipa mai departe din cele doua. Pentru structura 
de arbore binar ne folosim de implementarea listelor inlantuite, care sunt alocate
dinamic cu memoria necesare si dezalocate in final fara sa avem memory leaks-uri. Folosirea
listelor aduce un plus pentru performanta programului. Arbore mai permite vizualizare
sau urmarirea usoara a evolutiei echipelor pe parcursul turneului.

c.Exemplu
Input:
0 3 0
1 3 1
2 5 1
3 2 2
4 3 2
5 5 3
6 6 3
7 1 4
8 2 4
9 3 5
10 4 5

Output:
Traseul cu echipele invinse de 3:
5 2 4 
Numarul de meciuri necesar este de 5
Echipele care au trecut in fiecare runda:
Runda 1: 1 2 3 4 
Runda 2: 2 3 5 6 
Runda 3: 3 5 
Runda 4: 3 

d. Rationament
- citirea datelor se face din fisier, pe fiecare linie avand cheia nodului, valoarea
echipei si indexul parintelui, constructia arborelui incepand de la radacina cu pozitia
zero. Astfel pentru determinare unui traseu parcurs de o echipa se parcurge recursiv 
tot traseu si se afiseaza toate echipe invinse de catre aceasta. Numarul de meciuri
minime pentru orice numar de echipe este egal cu numarul total de echipe minus unu.
Pentru a afisa toate echipele care trec in fiecare runda determinam inaltimea totala
a arborelui, determinam toate nodurile de fiecare nivel si parcurgem recursiv tot
arborele.
*/

#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 64

typedef struct Node{
    int key;
    int echipa;
    struct Node* leftChild;
    struct Node* rightChild;
}Node;

Node* createNode(int key, int echipa){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL){
        perror("Eroare la alocarea memoriei!\n");
        exit(-1);
    }

    newNode->key = key;
    newNode->echipa = echipa;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    return newNode;
}

void freeMemory(Node* root){
    if(root == NULL){
        return;
    }

    freeMemory(root->leftChild);
    freeMemory(root->rightChild);
    free(root);
}

void inserare(Node* root, Node* child){
    if(root == NULL){
        return;
    }

    if(root->leftChild == NULL){
        root->leftChild = child;
    }else{
        root->rightChild = child;
    }
}

Node* readFromPath(const char* path, Node* nodes[]){
     FILE* fin = fopen(path,"r");
    if (fin == NULL) {
		perror("Eroare la deschiderea fisierului!\n");
		exit(-1);
	}

    Node* root = NULL;
    int indexParinte = 0;
    int key;
    int echipa = 0;
    int index = 1;
    while(fscanf(fin,"%d %d %d",&key,&echipa,&indexParinte) == 3){
        Node* newNode = createNode(key,echipa);
        nodes[index] = newNode;

        if(indexParinte == 0){
            root = newNode;
        }else{
            inserare(nodes[indexParinte],newNode);
        }
        index = index + 1;
    }
    

    if (fclose(fin) != 0) {
		perror("Eroare la inchiderea fisierului!\n");
		exit(-1);
	}

	return root;
}

void preordine(Node* root){
    if(root == NULL){
        return;
    }

    printf("%d ",root->echipa);
    preordine(root->leftChild);
    preordine(root->rightChild);
}

int numarMeciuri(Node* root){
    if(root == NULL){
        return 0;
    }
    
    if(root->leftChild == NULL && root->rightChild == NULL){
        return 1; 
    }

    return numarMeciuri(root->leftChild) + numarMeciuri(root->rightChild);
}

void traseuWinner(Node* root, int echipa){
    if(root == NULL){
        return;
    }

    if(root->echipa == echipa){
        if(root->leftChild != NULL && root->rightChild != NULL){
            if(root->echipa == root->leftChild->echipa){
                printf("%d ",root->rightChild->echipa);
            }else if(root->echipa == root->rightChild->echipa){
                printf("%d ",root->leftChild->echipa);
            }
        }
    }

    traseuWinner(root->leftChild,echipa);
    traseuWinner(root->rightChild,echipa);
}

int inaltimeaMaxima(Node* root){
    if(root == NULL){
        return 0;
    }

    int st = inaltimeaMaxima(root->leftChild);
    int dr = inaltimeaMaxima(root->rightChild);

    if(st > dr){
        return 1 + st;
    }else{
        return 1 + dr;
    }
}

void determinareNivel(Node* root, int nivel){
    if(root == NULL){
        return;
    }

    if(nivel == 0){
        printf("%d ",root->echipa);
    }else{
        determinareNivel(root->leftChild,nivel - 1);
        determinareNivel(root->rightChild, nivel - 1);
    }
}

void runde(Node* root) {
	if (root == NULL) {
		return;
	}

    int h = inaltimeaMaxima(root);
	int j = 0;
    for(int i = h - 1; i >= 0; i = i - 1){
        j = j + 1;
		printf("Runda %d: ", j);
		determinareNivel(root, i);
		printf("\n");
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    Node* nodes[MAX_NODES] = {NULL};
    Node* root = readFromPath(argv[1],nodes);
    preordine(root);
    printf("\n");

    printf("\nTraseul cu echipele invinse de %d:\n",root->echipa);
    traseuWinner(root,root->echipa);
    printf("\nNumarul de meciuri necesar este de %d\n",numarMeciuri(root) - 1);
    printf("\n\nEchipele care au trecut in fiecare runda:\n");
	runde(root);

    freeMemory(root);

    return 0;
}