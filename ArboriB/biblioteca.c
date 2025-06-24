/*
Sistem de Catalog al Bibliotecii
Cerințe pentru finalizare
O bibliotecă universitară folosește un sistem computerizat de catalog pentru a 
organiza milioane de cărți. Primiți o copie a structurii indexului lor în următorul f
ormat:

[m=5] „Ficțiune”|[„Aventură”, „Biografie”]|[„Istorie”, „Matematică”, „Fizică”]|[„Artă”, 
„Chimie”]|[„Dramă”, „Fantezie”]|[„Geologie”, „Medicină”]|[„Poezie”, „Știință”, 
„Tehnologie”, „Zoologie”]

Formatul reprezintă categorii de cărți, unde m=5 indică numărul maxim de categorii per 
nod, șirurile ghilite sunt categorii interne ale nodului, iar listele între paranteze 
sunt colecții de pagini.

Sarcina dvs. este să:

Analizați și validați această structură de catalog
Implementați o funcție de căutare pentru a găsi ce secțiune conține o anumită categorie 
de carte
Calculați numărul maxim de comparații necesare pentru a găsi orice categorie
Determinați dacă structura este optim echilibrată pentru operațiunile de căutare
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N   2
#define NN (N*2)
#define MAX_CATEG 64 // lungimea maximă a unei categorii

typedef char TipCheie[MAX_CATEG];

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

// Forward declarations
Pagina* insereaza(Pagina* pag, TipCheie x, Nod* nod);
void afisare(Pagina* arbore, int nivel);
Pagina* suprima(Pagina* pag, TipCheie x, Nod* nod);
int cautare(Pagina* pag, const char* x);

Nod v;

// ========== INSERARE ==========
Pagina* insereaza(Pagina* pag, TipCheie x, Nod* nod) {
    int i, s, d, mij;
    Pagina* q, * r;

    if (!nod) {
        strcpy(v.cheie, x);
        v.contor = 1;
        v.p = NULL;
    }
    else
        v = *nod;

    if (pag == NULL) {
        pag = (Pagina*)calloc(sizeof(Pagina), 1);
        pag->e[++pag->m] = v;
        return pag;
    }

    // cautare binara pozitie
    s = 1; d = pag->m;
    while (s <= d) {
        mij = (s + d) / 2;
        int cmp = strcmp(pag->e[mij].cheie, x);
        if (cmp == 0) {
            pag->e[mij].contor++;
            return pag;
        }
        if (cmp > 0)
            d = mij - 1;
        else
            s = mij + 1;
    }

    if (pag->p0 == NULL || nod) {
        if (pag->m < NN) {
            ++pag->m;
            for (i = pag->m; i > d + 1; i--)
                pag->e[i] = pag->e[i - 1];
            pag->e[i] = v;
        }
        else {
            Pagina* a = pag;
            Pagina* b = (Pagina*)calloc(sizeof(Pagina), 1);
            pag = (Pagina*)calloc(sizeof(Pagina), 1);
            for (i = 1; i <= N; i++)
                b->e[i] = a->e[i + N];
            a->m = b->m = N;

            if (strcmp(v.cheie, a->e[N].cheie) > 0 && strcmp(v.cheie, b->e[1].cheie) < 0)
                pag->e[++pag->m] = v;
            else {
                if (strcmp(v.cheie, a->e[N].cheie) < 0) {
                    pag->e[++pag->m] = a->e[N];
                    for (i = a->m; i > 1 && strcmp(a->e[i - 1].cheie, v.cheie) > 0; i--)
                        a->e[i] = a->e[i - 1];
                    a->e[i] = v;
                }
                else {
                    pag->e[++pag->m] = b->e[1];
                    for (i = 1; i < b->m && strcmp(b->e[i + 1].cheie, v.cheie) < 0; i++)
                        b->e[i] = b->e[i + 1];
                    b->e[i] = v;
                }
            }
            b->p0 = pag->e[1].p;
            pag->p0 = a;
            pag->e[1].p = b;
        }
    }
    else {
        if (d == 0)
            q = pag->p0;
        else
            q = pag->e[d].p;
        r = insereaza(q, x, NULL);
        if (r != q) {
            pag = insereaza(pag, r->e[1].cheie, &r->e[1]);
            free(r);
        }
    }
    return pag;
}

// ========== AFISARE ==========
void afisare(Pagina* arbore, int nivel) {
    int i;
    if (!arbore)
        return;
    afisare(arbore->p0, nivel + 1);
    for (i = 1; i <= arbore->m; i++)
        afisare(arbore->e[i].p, nivel + 1);

    printf("Nivel %d: ", nivel);
    for (i = 1; i <= arbore->m; i++)
        printf("%s ", arbore->e[i].cheie);
    printf("\n");
}

// ========== CAUTARE ==========
int cautare(Pagina* pag, const char* x) {
    int s, d, mij;
    if (pag == NULL)
        return 0;
    s = 1; d = pag->m;
    while (s <= d) {
        mij = (s + d) / 2;
        int cmp = strcmp(x, pag->e[mij].cheie);
        if (cmp == 0)
            return 1;
        if (cmp < 0)
            d = mij - 1;
        else
            s = mij + 1;
    }
    if (d == 0)
        return cautare(pag->p0, x);
    return cautare(pag->e[d].p, x);
}

// ========== STERGERE ==========
void vecinStang(Pagina* pag, Pagina* st, Pagina* r, int d) {
    int i;
    if (st->m == N) {
        st->e[N + 1] = pag->e[d];
        st->e[N + 1].p = r->p0;
        st->m = NN;
        for (i = N + 2; i <= NN; i++)
            st->e[i] = r->e[i - N - 1];
        for (i = d; i < pag->m; i++)
            pag->e[i] = pag->e[i + 1];
        pag->m--;
        free(r);
    }
    else {
        r->m = N;
        for (i = N; i > 1; i--)
            r->e[i] = r->e[i - 1];
        r->e[1] = pag->e[d];
        r->e[1].p = r->p0;
        r->p0 = st->e[st->m].p;
        pag->e[d] = st->e[st->m];
        pag->e[d].p = r;
        st->m--;
    }
}

void vecinDrept(Pagina* pag, Pagina* dr, Pagina* r, int d) {
    int i;
    r->e[N] = pag->e[d + 1];
    r->e[N].p = dr->p0;
    r->m = N;
    if (dr->m == N) {
        r->m = NN;
        for (i = N + 1; i <= NN; i++)
            r->e[i] = dr->e[i - N];
        for (i = 1; i < pag->m; i++)
            pag->e[i] = pag->e[i + 1];
        pag->m--;
        free(dr);
    }
    else {
        pag->e[d + 1] = dr->e[1];
        pag->e[d + 1].p = dr;
        dr->p0 = dr->e[1].p;
        for (i = 1; i < dr->m; i++)
            dr->e[i] = dr->e[i + 1];
        dr->m--;
    }
}

Pagina* suprima(Pagina* pag, TipCheie x, Nod* nod) {
    int i, s, d, mij;
    Pagina* q, * r;
    if (pag == NULL || pag->m == 0)
        return NULL;
    s = 1; d = pag->m;
    while (s <= d) {
        mij = (s + d) / 2;
        int cmp = strcmp(x, pag->e[mij].cheie);
        if (cmp == 0)
            break;
        if (cmp < 0)
            d = mij - 1;
        else
            s = mij + 1;
    }
    if (s <= d && strcmp(x, pag->e[mij].cheie) == 0) {
        if (pag->p0 == NULL) {
            if (nod) {
                q = nod->p;
                *nod = pag->e[pag->m];
                nod->p = q;
            }
            for (i = mij; i < pag->m; i++)
                pag->e[i] = pag->e[i + 1];
            pag->m--;
            for (q = pag; pag && pag->m == 0; free(q), q = pag)
                pag = pag->p0;
            return pag;
        }
        // Găsește cheia maximă mai mică (folosește funcție ajutătoare dacă vrei)
        return suprima(pag, pag->e[mij].cheie, &pag->e[mij]);
    }
    if (d == 0)
        q = pag->p0;
    else
        q = pag->e[d].p;
    r = suprima(q, x, nod);
    if (r == NULL) {
        if (nod)
            return suprima(pag, pag->e[d].cheie, nod);
        else
            return pag;
    }
    if (r->m < N) {
        Pagina* st, * dr;
        if (d == 1)
            st = pag->p0;
        else
            st = pag->e[d - 1].p;
        dr = pag->e[d + 1].p;
        if (r == pag->p0)
            vecinDrept(pag, dr, r, d);
        else if (d == pag->m)
            vecinStang(pag, st, r, d);
        else {
            if (dr->m > st->m)
                vecinDrept(pag, dr, r, d);
            else
                vecinStang(pag, st, r, d);
        }
    }
    for (q = pag; pag && pag->m == 0; free(q), q = pag)
        pag = pag->p0;
    return pag;
}

// ========== MAIN EXAMPLE ==========
int main() {
    Pagina* radacina = NULL;
    char categorii[][MAX_CATEG] = {
        "Fictiune", "Aventura", "Biografie", "Istorie", "Matematica",
        "Fizica", "Arta", "Chimie", "Drama", "Fantezie",
        "Geologie", "Medicina", "Poezie", "Stiinta", "Tehnologie", "Zoologie"
    };
    int n = sizeof(categorii) / MAX_CATEG, i;

    // Inserare categorii
    for (i = 0; i < n; i++)
        radacina = insereaza(radacina, categorii[i], NULL);

    printf("Arborele-B initial (categorii):\n");
    afisare(radacina, 1);
    printf("\n");

    // Căutare demo
    const char* test = "Drama";
    printf("Caut categoria '%s': %s\n", test, cautare(radacina, test) ? "gasita" : "NU exista");

    // Ștergere demo
    TipCheie x;
    strncpy(x, "Drama", MAX_CATEG);
    x[MAX_CATEG-1] = 0;
    radacina = suprima(radacina, x, NULL);
    afisare(radacina, 1);

    // Căutare după ștergere
    printf("Caut categoria '%s': %s\n", test, cautare(radacina, test) ? "gasita" : "NU exista");

    return 0;
}
