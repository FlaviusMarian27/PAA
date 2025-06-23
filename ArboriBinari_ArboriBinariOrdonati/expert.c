/*
1. Implementare C99 + explicații pe foaie
Ai un fișier cu un arbore de întrebări (sistemul expert) — fiecare nod are două 
ieșiri: una pentru „NU” și una pentru „DA”.

Ai un alt fișier cu răspunsurile pacientului (șir de 0 și 1).
Trebuie să simulezi consultația și să determini:
dacă se ajunge la un diagnostic (frunză, adică nod fără alte ramuri)
altfel, câte întrebări maxim ar mai fi necesare ca să ajungi garantat la un diagnostic.

2. Determinare traseu maxim (independent de răspunsuri)
Se cere să afișezi numărul maxim de întrebări care pot fi parcurse din rădăcină până la 
un diagnostic (adică adâncimea maximă a arborelui de decizie).

                            1
                           / \
                          /   \
                         /     \
                        /       \
                       /         \
                      /           \
                     2             3 
                   /   \         /   \
                  4     5        6     7
                /\    /    \    / \   / \ 
               8  9  10    11  12 13 14 15
                    / \   /  \
                  16  17 18  19

=================================================================================
a.Structura de date aleasa
- structura de date pe care am folosit o este cea de arbore binar,reprezentat prin liste alocate dinamic
astfel fiecare nod cu execeptia unui nod frunza reprezinta o intrebare, iar fiecare 
legatura cu un nod fiu reprezinta un raspuns, stanga fiind raspunsul NU, iar dreapta 
raspunsul DA. Nodurile frunza reprezinta practic diagnosticul/raspunsul.

b.Justificare
- am ales sa folosesc structura unui arbore binar pentru ca stim ca avem mereu doua
raspunsuri, NU sau DA, iar structura reflecta direct logica structuri de sistem expert.
Astfel permite o parcurgere eficienta, pe baza raspunsurilor, totodata o determinare
rapida a frunzelor si este usor de construit din fisierul dat unde stim mereu index 
parintelui pentru fiecare nod.

C.Exemplu
intrebari.txt
19
0 1 1 2 2 3 3 4 4 5 5 6 6 7 7 10 10 12 12

client.txt
0 1

Output:
Consultatie incompleta, mai sunt necesare maxim 2 intrebari.
Numarul maxim de intrebari pana la diagnostic este: 4

d. Rationament
- astfel construim arbore plecand de fisierul dat, dupa care citim valorile din fisierul
clientului unde se afla raspunsurile si clientului. Astfel toate valorile sunt scrise
intr un tablou de caractere, iar in cele din urma parcurgem tablou dupa valorile 0 si 1
si odata epuizate intrebari verificam daca nodul curent are copii. Daca nu are atunci 
inseamna ca avem un diagnostic, iar daca are atunci ne folosim de funct. de adancimea 
maxima si verificam cate raspunsuri mai avem nevoie. Pentru a ne determina numarul 
maxim de intrebari pe care il poate primi un client de la doctor, ne folosim tot
de functie care ne calculeaza adancimea maxima a arborelui.

*/
#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 30

typedef struct Node{
    int key;
    struct Node* left; //no
    struct Node* right; // yes
}Node;

Node* createNode(int key){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL){
        perror("Eroare la alocarea memoriei!\n");
        exit(-1);
    }

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void freeMemory(Node* root){
    if(root == NULL){
        return;
    }

    freeMemory(root->left);
    freeMemory(root->right);
    free(root);
}

void insertNode(Node* root, Node* child){
    if(root == NULL){
        perror("Parinte NULL!\n");
        exit(-1);
    }

    if(root->left == NULL){
        root->left = child;
    }else{
        root->right = child;
    }
}

Node* readFromPath(const char* path, Node* nodes[]){
    FILE* fin = fopen(path,"r");
    if (fin == NULL) {
		perror("Eroare la deschiderea fisierului!\n");
		exit(-1);
	}

    int size;
    if(fscanf(fin,"%d",&size) != 1){
        perror("Eroare la citirea dimensiunii!\n");
        exit(-1);
    }

    Node* root = NULL;
    int indexParinte = 0;
    int index = 1;
    while(fscanf(fin,"%d ",&indexParinte) == 1 && index <= size){
        Node* newNode = createNode(index);
        nodes[index] = newNode;

        if(indexParinte == 0){
            root = newNode;
        }else{
            insertNode(nodes[indexParinte],newNode);
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

    printf("%d ",root->key);
    preordine(root->left);
    preordine(root->right);
}

int esteFrunza(Node* root){
    if(root == NULL){
        return 0;
    }

    if(root->left == NULL && root->right == NULL){
        return 1;
    }

    return 0;
}

int adancimeMaxima(Node* root){
    if(root == NULL){
        return 0;
    }

    int adancimeStanga = adancimeMaxima(root->left);
    int adancimeDreapta = adancimeMaxima(root->right);

    if(adancimeStanga > adancimeDreapta){
        return 1 + adancimeStanga;
    }else{
        return 1 + adancimeDreapta;
    }
}

void consultatie(Node* root, const char* raspunsuri){
    if(root == NULL){
        return;
    }

    Node* temp = root;
    int i = 0;

    while(temp != NULL && raspunsuri[i] != '\0'){
        if(raspunsuri[i] == '0'){
            temp = temp->left;
        }else if(raspunsuri[i] == '1'){
            temp = temp->right;
        }else{
            printf("Raspunsul %c este unul invalid!\n",raspunsuri[i]);
            return;
        }
        i = i + 1;
    }

    if (temp == NULL) {
        printf("Consultatie imposibila, traseu invalid.\n");
    }else if(esteFrunza(temp) == 1){
        printf("Diagnostic atins in nodul: %d\n", temp->key);
    }else{
        int ramas = adancimeMaxima(temp) - 1;
        printf("Consultatie incompleta, mai sunt necesare maxim %d intrebari.\n", ramas);
    }
}

int citesteRaspunsuri(const char* path, char* raspunsuri, int maxLen){
    FILE* fin = fopen(path,"r");
    if (fin == NULL) {
		perror("Eroare la deschiderea fisierului!\n");
		exit(-1);
	}

    int val, index = 0;
    while(fscanf(fin,"%d",&val) == 1 && index < maxLen - 1){
        if (val != 0 && val != 1) {
            fprintf(stderr, "Valoare invalida in fisier: %d\n", val);
            fclose(fin);
            return -1;
        }
        raspunsuri[index] = val + '0'; 
        index = index + 1;
    } 

    raspunsuri[index] = '\0'; 

    if (fclose(fin) != 0) {
		perror("Eroare la inchiderea fisierului!\n");
		exit(-1);
	}
    return 0;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    Node* nodes[MAX_NODES] = {NULL};
    Node* root = readFromPath(argv[1],nodes);
    
    //printf("Preordinea:\n");
    //preordine(root);
    //printf("\n");

    char raspunsuri[MAX_NODES];
    if (citesteRaspunsuri(argv[2], raspunsuri, MAX_NODES) != 0) {
        exit(-1);
    }

    //printf("%s",raspunsuri);
    consultatie(root,raspunsuri);

    int adancime = adancimeMaxima(root) - 1;
    printf("Numarul maxim de intrebari pana la diagnostic este: %d\n", adancime);
    freeMemory(root);

    return 0;
}