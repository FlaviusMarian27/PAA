#include<stdio.h>
#include<stdlib.h>

#define MAX_NODES 25

typedef struct Node{
    char key;
    struct Node* firstChild;
    struct Node* rightSibling;
}Node;

Node* createNode(char key){
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL){
        perror("Eroare la alocarea memoriei!\n");
        exit(-1);
    }

    newNode->key = key;
    newNode->firstChild = NULL;
    newNode->rightSibling = NULL;

    return newNode;
}

void freeMemory(Node* root){
    if(root == NULL){
        return;
    }

    freeMemory(root->firstChild);
    freeMemory(root->rightSibling);
    free(root);
}

void insertNode(Node* root, Node* child){
    if(root == NULL){
        perror("Parinte NULL!\n");
        exit(-1);
    }

    if(root->firstChild == NULL){
        root->firstChild = child;
    }else{
        Node* temp = root->firstChild;
        while(temp->rightSibling != NULL){
            temp = temp->rightSibling;
        }
        temp->rightSibling = child;
    }
}

Node* readFromPath(const char* path, Node* nodes[]){
    FILE* fin = fopen(path, "r");
	if (fin == NULL) {
		perror("Eroare la deschiderea fisierului!\n");
		exit(-1);
	}

    Node* root = NULL;
    char key;
    int indexP = 0;
    int index = 1;
    while(fscanf(fin," %c %d",&key,&indexP) == 2){
        Node* newNode = createNode(key);
        nodes[index] = newNode;

        if(indexP == 0){
            root = newNode;
        }else{
            insertNode(nodes[indexP],newNode);
        }
        //printf("%c ",key);
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

    printf("%c ", root->key);
    Node* temp = root->firstChild;
    while(temp != NULL){
        preordine(temp);
        temp = temp->rightSibling;
    }
}

void inordine(Node* root){
    if(root == NULL){
        return;
    }

    Node* temp = root->firstChild;
    if(temp != NULL){
        inordine(temp);
        temp = temp->rightSibling;
    }

    printf("%c ", root->key);
    while(temp != NULL){
        inordine(temp);
        temp = temp->rightSibling;
    }
}

void postordine(Node* root){
    if(root == NULL){
        return;
    }

    Node* temp = root->firstChild;
    while(temp != NULL){
        postordine(temp);
        temp = temp->rightSibling;
    }

    printf("%c ", root->key);
}

int numarFrunze(Node* root){
    if(root == NULL){
        return 0;
    }

    if(root->firstChild == NULL){
        return 1;
    }

    int count = 0;
    Node* temp = root->firstChild;
    while(temp != NULL){
        count = count + numarFrunze(temp);
        temp = temp->rightSibling;
    }

    return count;
}

int numarParinti(Node* root){
    if(root == NULL){
        return 0;
    }

    if(root->rightSibling != NULL){
        return 1;
    }

    int count = 0;
    Node* temp = root->firstChild;
    while(temp != NULL){
        count = count + numarParinti(temp);
        temp = temp->rightSibling;
    }

    return count;
}

int inaltime(Node* root) {
	if (root == NULL) {
		return 0;
	}

    int h_max = 0;
	Node* temp = root->firstChild;
    while(temp != NULL){
        int h = inaltime(temp);
        if(h > h_max){
            h_max = h;
        }

        temp = temp->rightSibling;
    }

    return 1 + h_max;
}

int gradMaxim(Node* root){
    if(root == NULL){
        return 0;
    }

    int maxGrad = 0;
    Node* temp = root->firstChild;
    while(temp != NULL){
        int grad = 0;
        Node* copil = temp;
        while(copil != NULL){
            grad = grad + 1;
            copil = copil->rightSibling;
        }

        if (grad > maxGrad) {
            maxGrad = grad;
        }

        // mergem recursiv la fiecare copil
        int gradCopil = gradMaxim(temp);
        if (gradCopil > maxGrad) {
            maxGrad = gradCopil;
        }

        temp = temp->rightSibling;
    }

    return maxGrad;
}

int main(int argc, char* argv[]){
    if (argc != 2) {
		perror("Argumente insuficiente!\n");
		exit(-1);
	}

    Node* nodes[MAX_NODES] = {NULL};
    Node* root = readFromPath(argv[1],nodes);
    
    printf("Preordine:\n");
    preordine(root);

    printf("\nInordine:\n");
    inordine(root);

    printf("\nPostordine:\n");
    postordine(root);

    printf("\nNumarul de frunze este %d\n",numarFrunze(root));
    printf("Numarul de parinti este %d\n",numarParinti(root));
    printf("Inaltimea arborelui este %d\n",inaltime(root));
    printf("Gradul maxim este %d\n",gradMaxim(root));

    free(root);

    return 0;
}