#include <stdio.h>
#include <stdlib.h>

#define N   2
#define NN (N * 2)

typedef int TipCheie;

typedef struct Nod {
    TipCheie cheie;
    struct Pagina* p;
    int contor;
} Nod;

typedef struct Pagina {
    int m;
    struct Pagina* p0;
    Nod e[NN + 1];
} Pagina;

Nod v;

Pagina* insereaza(Pagina *pag, TipCheie x, Nod *nod);
Pagina* suprima(Pagina *pag, TipCheie x, Nod *nod);
void afisare(Pagina *arbore, int nivel);
int cautare(Pagina *pag, TipCheie x);

// ------------------ INSERARE ------------------
Pagina* insereaza(Pagina *pag, TipCheie x, Nod *nod) {
    int i, s, d, mij;
    Pagina *q, *r;

    if (!nod) {
        v.cheie = x;
        v.contor = 1;
        v.p = NULL;
    } else {
        v = *nod;
    }

    if (pag == NULL) {
        pag = (Pagina*)calloc(1, sizeof(Pagina));
        pag->e[++pag->m] = v;
        return pag;
    }

    s = 1; d = pag->m;
    while (s <= d) {
        mij = (s + d) / 2;
        if (pag->e[mij].cheie == x) {
            pag->e[mij].contor++;
            return pag;
        }
        if (x < pag->e[mij].cheie) d = mij - 1;
        else s = mij + 1;
    }

    if (pag->p0 == NULL || nod) {
        if (pag->m < NN) {
            ++pag->m;
            for (i = pag->m; i > d + 1; i--)
                pag->e[i] = pag->e[i - 1];
            pag->e[i] = v;
        } else {
            Pagina *a = pag;
            Pagina *b = (Pagina*)calloc(1, sizeof(Pagina));
            pag = (Pagina*)calloc(1, sizeof(Pagina));

            for (i = 1; i <= N; i++)
                b->e[i] = a->e[i + N];
            a->m = b->m = N;

            if (v.cheie > a->e[N].cheie && v.cheie < b->e[1].cheie)
                pag->e[++pag->m] = v;
            else {
                if (v.cheie < a->e[N].cheie) {
                    pag->e[++pag->m] = a->e[N];
                    for (i = a->m; i > 1 && a->e[i - 1].cheie > v.cheie; i--)
                        a->e[i] = a->e[i - 1];
                    a->e[i] = v;
                } else {
                    pag->e[++pag->m] = b->e[1];
                    for (i = 1; i < b->m && b->e[i + 1].cheie < v.cheie; i++)
                        b->e[i] = b->e[i + 1];
                    b->e[i] = v;
                }
            }

            b->p0 = pag->e[1].p;
            pag->p0 = a;
            pag->e[1].p = b;
        }
    } else {
        if (d == 0) q = pag->p0;
        else q = pag->e[d].p;
        r = insereaza(q, x, NULL);
        if (r != q) {
            pag = insereaza(pag, r->e[1].cheie, &r->e[1]);
            free(r);
        }
    }

    return pag;
}

// ------------------ AFISARE ------------------
void afisare(Pagina *arbore, int nivel) {
    int i;
    if (!arbore) return;
    afisare(arbore->p0, nivel + 1);
    for (i = 1; i <= arbore->m; i++)
        afisare(arbore->e[i].p, nivel + 1);

    printf("Nivel %d: ", nivel);
    for (i = 1; i <= arbore->m; i++)
        printf("%d ", arbore->e[i].cheie);
    printf("\n");
}

// ------------------ CAUTARE ------------------
int cautare(Pagina *pag, TipCheie x) {
    if (pag == NULL) return 0;
    int s = 1, d = pag->m, mij;
    while (s <= d) {
        mij = (s + d) / 2;
        if (x == pag->e[mij].cheie) return 1;
        if (x < pag->e[mij].cheie) d = mij - 1;
        else s = mij + 1;
    }
    if (d == 0) return cautare(pag->p0, x);
    return cautare(pag->e[d].p, x);
}

// ------------------ MAIN ------------------
int main() {
    Pagina* rad = NULL;
    int val;

    printf("Introduceti valori (0 pt oprire):\n");
    while (1) {
        printf("Valoare: ");
        scanf("%d", &val);
        if (val == 0) break;
        rad = insereaza(rad, val, NULL);
    }

    printf("\nArbore B:\n");
    afisare(rad, 0);

    return 0;
}
