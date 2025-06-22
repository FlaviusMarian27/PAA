/*
Motor de căutare cu completare automată
Cerințe pentru finalizare
Un motor de căutare trebuie să implementeze o funcție de completare automată 
pentru milioane de interogări de căutare. Utilizatorii tastează interogări 
parțiale și se așteaptă să vadă instantaneu cele mai relevante completări. 
Sistemul trebuie:

Să gestioneze potrivirea prefixelor pentru orice intrare parțială
Să returneze primele 10 completări cele mai frecvente pentru fiecare prefix
Să suporte actualizări în timp real atunci când termenii de căutare noi devin populari
Să stocheze eficient interogările care au prefixe comune
Având în vedere un set de date de interogări de căutare cu frecvența lor, proiectați 
un algoritm pentru a construi și interoga acest sistem de completare automată. Luați 
în considerare eficiența memoriei, deoarece prefixe comune precum „cum să” apar în mii 
de interogări.

==========================================================================================
a. Structura de date folosita
- am ales sa folosesc ca si structura de data un arbore de regasire, implementat prin
intermediul unei liste simplu inlantuite. Astfel fiecare nod are practic 26 de copii
asociati acestuia de la A la Z, o frecventa asociata pentru fiecare cuvant si respectiv
un caracter special pentru a marca finalul cuvantului.

b Justificare alegere
- am ales aceasta structura pentru ca faciliteaza cautarea rapida a prefixelor date sau
a cuvintelor dorite.Astfel folosirea unor liste simplu inlantuite aduc performanta in 
plus, fiind mult mai optime si chiar daca avem alocari dinamice atat pentru creearea unui 
nod cat si pentru wordsEntry, avem grija sa alocam strictul necesar de blocuri si in 
acelasi timp sa si eliberam complet fiecare bloc alocat, neavant memory leaks uri.

c Exemplu
AR
ARA
ARAD
PI
PIAN
            |A....P..|
            /      \
        |.R.]|    |.I...]|
         /           \
       .....         .....
--------------------------------------------------
AR      8
ARA     2
ARAC    3
ARAD    4
ARADEAN 2
AVION   6
BANANA  2
BIBLIOTECA      1
CUPTOR  2
FOC     4
MASINA  7
MINGE   5
MORCOV  5
OM      5
ORA     4
PI      6
PIAN    3
RAMURA  3
SALAM   2
TRAMVAI 1
VAPOR   1
VULPE   2
ZEBRA   2

Lista cuvintelor care incep cu AR:
AR      8
ARA     2
ARAC    3
ARAD    4
ARADEAN 2

Top  5 cele mai frecvente cuvinte : 
AR      8
MASINA  7
AVION   6
PI      6
MINGE   5

d. Rationament
- se parcurge tot fisierul stiind ca pe fiecare linie avem cuvantul, un spatiu si 
frecventa, urmand ca fiecare cuvant sa fie inserat in arborele nostru de regasire.
Daca un nod nu exista, practic alocam memorie pentru creearea acestuia, iar cand
ajungem la final de cuvant practic marcam cu caracterul ']' finalul cuvantului si
salvam frecventa respectivului cuvant. Pentru prefix ne folosim de parcurgerea
in adancime, iar pentru a determina celor mai cautate cuvinte ne folosim de structura
noua de cuvant si frecventa, in care copiem tot intr un nou tablou si in cele din urma
ne folosim de qsort pentru a putea sa le sortam descrescator dupa frecventa de cautare.

*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define ALPHABET_SIZE 26
#define SIZE_BUFFER 100
#define MAX_WORDS 1000

typedef struct TrieNode{
    int frequency;
    char end_of_word;
    struct TrieNode* children[ALPHABET_SIZE];
}TrieNode;

typedef struct {
	char word[SIZE_BUFFER];
	int frequency;
}WordEntry;

TrieNode* createNode(){
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    if(newNode == NULL){
        perror("Eroare la alocarea memoriei!\n");
        exit(-1);
    }

    newNode->frequency = 0;
    newNode->end_of_word = 0;
    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        newNode->children[i] = NULL;
    }

    return newNode;
}

void freeMemory(TrieNode* root){
    if(root == NULL){
        return;
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        freeMemory(root->children[i]);
    }

    free(root);
}

void insertWord(TrieNode* root, const char* word, int frequency){
    if(root == NULL){
        perror("Parinte NULL!\n");
        exit(-1);
    }

    TrieNode* temp = root;
    while(*word){
        char ch = toupper(*word);
        if(isupper(ch) == 0){
            word = word + 1;
            continue;
        }

        int position = ch - 'A';
        if(temp->children[position] == NULL){
            temp->children[position] = createNode();
        }

        temp = temp->children[position];
        word = word + 1;
    }

    temp->frequency = frequency;
    temp->end_of_word = ']';
}

TrieNode* readFromPath(const char* path){
    FILE* fin = fopen(path,"r");
    if(fin == NULL){
        perror("Eroare la dechiderea fisierului!\n");
        exit(-1);
    }

    TrieNode* root = NULL;
    root = createNode();
    
    char buffer[SIZE_BUFFER];
    int frequency = 0;
    while(fscanf(fin,"%s %d",buffer,&frequency) == 2){
        insertWord(root,buffer,frequency);
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(-1);
    }

    //printf("Succes!\n");
    return root;
}

void printWords(TrieNode* root, char* buffer, int contor_adancime){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        printf("%s\t%d\n",buffer,root->frequency);
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            printWords(root->children[i],buffer,contor_adancime + 1);
        }
    }
}

void prefixWords(TrieNode* root, char* buffer, int contor_adancime, const char* prefix){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        if(strncmp(buffer,prefix,strlen(prefix)) == 0){
            buffer[contor_adancime] = '\0';
            printf("%s\t%d\n",buffer,root->frequency);
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            prefixWords(root->children[i],buffer,contor_adancime + 1,prefix);
        }
    }
}

void copyWords(TrieNode* root, char* buffer, int contor_adancime, WordEntry* words, int* count){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        strcpy(words[*count].word,buffer);
        words[*count].frequency = root->frequency;
        *count = *count + 1;
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            copyWords(root->children[i],buffer,contor_adancime + 1,words,count);
        }
    }
}

int desc_cmp(const void* a, const void* b){
    WordEntry* w1 = (WordEntry*) a;
    WordEntry* w2 = (WordEntry*) b;
    return w2->frequency - w1->frequency;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Eroare argumente insuficiente!\n");
        exit(-1);
    }

    TrieNode* root = readFromPath(argv[1]);
    char buffer[SIZE_BUFFER];
    printWords(root,buffer,0);

    char prefix[SIZE_BUFFER] = "AR";
    printf("\nLista cuvintelor care incep cu %s:\n",prefix);
    prefixWords(root,buffer,0,prefix);

    WordEntry* words = (WordEntry*)malloc(MAX_WORDS * sizeof(WordEntry));
    if(words == NULL){
        perror("Eroare la alocarea memoriei!\n");
        exit(-1);
    }

    int count = 0;
    copyWords(root,buffer,0,words,&count);
    qsort(words,count,sizeof(WordEntry),desc_cmp);
    int x = 5;
	printf("\nTop % d cele mai frecvente cuvinte : \n",x);
	for (int i = 0; i < x; i = i + 1) {
		printf("%s\t%d\n", words[i].word, words[i].frequency);
	}

    free(words);
    freeMemory(root);

    return 0;
}