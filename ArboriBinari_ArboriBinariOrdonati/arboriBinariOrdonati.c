/*
Implementare operatii de baza arbori binari ordonati

5 2 8 1 4 6 9
         5
       /   \
      2     8
     / \   / \
    1   4 6   9

*/
#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 25

typedef struct Node{
    int key;
    struct Node* leftChild;
    struct Node* rightChild;
}Node;

Node* createNode(int key){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL){
        perror("Eroare alocarea memoriei!\n");
        exit(-1);
    }

    newNode->key = key;
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

void insertNode(Node* root, Node* child){
    if(root == NULL){
        perror("Parinte NULL!\n");
        return;
    }

    if(root->key > child->key){
        if(root->leftChild == NULL){
            root->leftChild = child;
        }else{
            insertNode(root->leftChild,child);
        }
    }else{
        if(root->rightChild == NULL){
            root->rightChild = child;
        }else{
            insertNode(root->rightChild,child);
        }
    }
}

Node* readFromPath(const char* path){
    FILE* fin = fopen(path,"r");
    if(fin == NULL){
        perror("Eroare la deschiderea fisierului!\n");
        exit(-1);
    }

    Node* root = NULL;
    int key = 0;
    int index = 1;
    while(fscanf(fin,"%d",&key) == 1){
        Node* newNode = createNode(key);

        if(index == 1){
            root = newNode;
        }else{
            insertNode(root,newNode);
        }
        index = index + 1;
    }

    if(fclose(fin) != 0){
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
    preordine(root->leftChild);
    preordine(root->rightChild);
}

void inordine(Node* root){
    if(root == NULL){
        return;
    }

    inordine(root->leftChild);
    printf("%d ",root->key);
    inordine(root->rightChild);
}

void postordine(Node* root){
    if(root == NULL){
        return;
    }

    postordine(root->leftChild);
    postordine(root->rightChild);
    printf("%d ",root->key);
}

int numarFrunze(Node* root){
    if(root == NULL){
        return 0;
    }

    if(root->leftChild == NULL && root->rightChild == NULL){
        return 1;
    }

    return numarFrunze(root->leftChild) + numarFrunze(root->rightChild);
}

int numarParinti(Node* root){
    if(root == NULL){
        return 0;
    }

    if(root->leftChild != NULL || root->rightChild != NULL){
        return 1 + numarParinti(root->leftChild) + numarParinti(root->rightChild);
    }else{
        return numarParinti(root->leftChild) + numarParinti(root->rightChild);
    }
}

int inaltimea(Node* root){
    if(root == NULL){
        return 0;
    }

    int st = inaltimea(root->leftChild);
    int dr = inaltimea(root->rightChild);
    if(st > dr){
        return 1 + st;
    }else{
        return 1 + dr;
    }
}

Node* deleteLeaf(Node* root, int key){
    if(root == NULL){
        return NULL;
    }

    if(key < root->key){
        root->leftChild = deleteLeaf(root->leftChild, key);
    }else if (key > root->key){
        root->rightChild = deleteLeaf(root->rightChild, key);
    }else{
        if (root->leftChild == NULL && root->rightChild == NULL) {
            free(root);
            return NULL;
        }else{
            return root;
        }
    }

    return root;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    Node* root = readFromPath(argv[1]);

    printf("Preordine:\n");
    preordine(root);

    printf("\nInordine:\n");
    inordine(root);

    printf("\nPostordine:\n");
    postordine(root);
    printf("\n");

    printf("Numarul de frunze este %d\n",numarFrunze(root));
    printf("Numarul de parinti este %d\n",numarParinti(root));
    printf("Inaltimea arborelui este %d\n",inaltimea(root));

    root = deleteLeaf(root,9);
    printf("\nDupa stergere:\n");
    preordine(root);
    printf("\n");

    freeMemory(root);

    return 0;
}