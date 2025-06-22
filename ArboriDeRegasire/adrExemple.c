#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define ALPHABET_SIZE 26
#define SIZE_BUFFER 100

typedef struct TrieNode{
    int frequency;
    char end_of_word;
    struct TrieNode* children[ALPHABET_SIZE];
}TrieNode;

TrieNode* createNode(){
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    if(newNode == NULL){
        perror("Eroare la alocare memoriei!\n");
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
        return;
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
        perror("Eroare la deschiderea fisierului!\n");
        exit(-1);
    }

    TrieNode* root = createNode();
    char buffer[SIZE_BUFFER];
    int frequency = 0;
    while(fscanf(fin,"%s %d",buffer,&frequency) == 2){
        insertWord(root,buffer,frequency);
    }

    if(fclose(fin) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(-1);
    }

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

void firstWord(TrieNode* root, char* buffer, int contor_adancime, int* found){
    if(root == NULL || *found == 1){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        printf("%s\n",buffer);
        *found = 1;
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            firstWord(root->children[i],buffer,contor_adancime + 1,found);
        }
    }
}

void lastWord(TrieNode* root, char* buffer, int contor_adancime, char* last_word){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        strcpy(last_word,buffer);
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            lastWord(root->children[i],buffer,contor_adancime + 1,last_word);
        }
    }
}

void shortWord(TrieNode* root, char* buffer, int contor_adancime, char* word){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        if(strlen(buffer) < strlen(word)){
            strcpy(word,buffer);
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            shortWord(root->children[i],buffer,contor_adancime + 1,word);
        }
    }
}

void longWord(TrieNode* root, char* buffer, int contor_adancime, char* word){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        if(strlen(buffer) > strlen(word)){
            strcpy(word,buffer);
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            longWord(root->children[i],buffer,contor_adancime + 1,word);
        }
    }
}

void findWord(TrieNode* root, char* buffer, int contor_adancime, int* found, char* word){
    if(root == NULL || *found == 1){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        if(strcmp(buffer,word) == 0){
            printf("%s - Gasit\n",buffer);
            *found = 1;
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            findWord(root->children[i],buffer,contor_adancime + 1,found, word);
        }
    }
}

void printWordsByLen(TrieNode* root, char* buffer, int contor_adancime, int x){
    if(root == NULL){
        return;
    }

    if(root->end_of_word == ']'){
        buffer[contor_adancime] = '\0';
        if(strlen(buffer) >= x){
            printf("%s\t%d\n",buffer,root->frequency);
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i = i + 1){
        if(root->children[i] != NULL){
            buffer[contor_adancime] = i + 'A';
            printWordsByLen(root->children[i],buffer,contor_adancime + 1,x);
        }
    }
}

int main(int argc, char* argv[]){
        if(argc != 2){
        perror("Eroare argumente insuficiente!\n");
        exit(-1);
    }

    TrieNode* root = readFromPath(argv[1]);
    char buffer[SIZE_BUFFER];
    printWords(root,buffer,0);

    int found = 0;
    printf("\nPrimul cuvant este:\n");
    firstWord(root,buffer,0,&found);

    char cuv[SIZE_BUFFER];
    lastWord(root,buffer,0,cuv);
    printf("Ultimul cuvant este:\n%s\n",cuv);

    shortWord(root,buffer,0,cuv);
    printf("Cel mai scurt cuvant este:\n%s\n",cuv);

    longWord(root,buffer,0,cuv);
    printf("Cel mai lung cuvant este:\n%s\n",cuv);

    char cuv1[SIZE_BUFFER] = "OM";
    found = 0;
    printf("Cuvantul cautat:\n");
    findWord(root,buffer,0,&found,cuv1);

    int x = 6;
    printf("Toate cuvintele de %d caractere:\n",x);
    printWordsByLen(root,buffer,0,x);

    freeMemory(root);

    return 0;
}