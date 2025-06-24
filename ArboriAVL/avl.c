#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int valoare;
    int inaltime;
    struct AVLNode* copilStanga;
    struct AVLNode* copilDreapta;
} AVLNode;

AVLNode* creeazaNod(int valoare) {
    AVLNode* nod = (AVLNode*)malloc(sizeof(AVLNode));
    if (nod != NULL) {
        nod->valoare = valoare;
        nod->inaltime = 1;
        nod->copilStanga = NULL;
        nod->copilDreapta = NULL;
    }
    return nod;
}


// Calculează maximul dintre două numere
int maxim(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Înălțimea unui nod
int inaltime(AVLNode* nod) {
    if (nod == NULL) {
        return 0;
    } else {
        return nod->inaltime;
    }
}

// Factor de echilibru al unui nod
int factorEchilibru(AVLNode* nod) {
    if (nod == NULL) {
        return 0;
    } else {
        return inaltime(nod->copilStanga) - inaltime(nod->copilDreapta);
    }
}

// Rotație la dreapta
AVLNode* rotatieDreapta(AVLNode* nodY) {
    AVLNode* nodX = nodY->copilStanga;
    AVLNode* subarboreDreaptaX = nodX->copilDreapta;

    nodX->copilDreapta = nodY;
    nodY->copilStanga = subarboreDreaptaX;

    nodY->inaltime = 1 + maxim(inaltime(nodY->copilStanga), inaltime(nodY->copilDreapta));
    nodX->inaltime = 1 + maxim(inaltime(nodX->copilStanga), inaltime(nodX->copilDreapta));

    return nodX;
}

// Rotație la stânga
AVLNode* rotatieStanga(AVLNode* nodX) {
    AVLNode* nodY = nodX->copilDreapta;
    AVLNode* subarboreStangaY = nodY->copilStanga;

    nodY->copilStanga = nodX;
    nodX->copilDreapta = subarboreStangaY;

    nodX->inaltime = 1 + maxim(inaltime(nodX->copilStanga), inaltime(nodX->copilDreapta));
    nodY->inaltime = 1 + maxim(inaltime(nodY->copilStanga), inaltime(nodY->copilDreapta));

    return nodY;
}

// Inserare într-un arbore AVL
AVLNode* insereaza(AVLNode* radacina, int valoare) {
    if (radacina == NULL) {
        return creeazaNod(valoare);
    }

    if (valoare < radacina->valoare) {
        radacina->copilStanga = insereaza(radacina->copilStanga, valoare);
    } else if (valoare > radacina->valoare) {
        radacina->copilDreapta = insereaza(radacina->copilDreapta, valoare);
    } else {
        // Valoare duplicat – nu inserăm
        return radacina;
    }

    radacina->inaltime = 1 + maxim(inaltime(radacina->copilStanga), inaltime(radacina->copilDreapta));

    int balans = factorEchilibru(radacina);

    // LL
    if (balans > 1 && valoare < radacina->copilStanga->valoare) {
        return rotatieDreapta(radacina);
    }

    // RR
    if (balans < -1 && valoare > radacina->copilDreapta->valoare) {
        return rotatieStanga(radacina);
    }

    // LR
    if (balans > 1 && valoare > radacina->copilStanga->valoare) {
        radacina->copilStanga = rotatieStanga(radacina->copilStanga);
        return rotatieDreapta(radacina);
    }

    // RL
    if (balans < -1 && valoare < radacina->copilDreapta->valoare) {
        radacina->copilDreapta = rotatieDreapta(radacina->copilDreapta);
        return rotatieStanga(radacina);
    }

    return radacina;
}

// Afișare în preordine
void afiseazaPreordine(AVLNode* radacina) {
    if (radacina != NULL) {
        printf("%d ", radacina->valoare);
        afiseazaPreordine(radacina->copilStanga);
        afiseazaPreordine(radacina->copilDreapta);
    }
}

// Eliberare memorie
void elibereaza(AVLNode* radacina) {
    if (radacina != NULL) {
        elibereaza(radacina->copilStanga);
        elibereaza(radacina->copilDreapta);
        free(radacina);
    }
}

// ---------- MAIN ----------
int main() {
    int valori[] = {5, 4, 8, 6, 9, 7};
    int numar = sizeof(valori) / sizeof(valori[0]);
    int i;

    AVLNode* radacina = NULL;

    for (i = 0; i < numar; i++) {
        radacina = insereaza(radacina, valori[i]);
    }

    printf("Arbore AVL (preordine):\n");
    afiseazaPreordine(radacina);
    printf("\n");

    elibereaza(radacina);
    return 0;
}