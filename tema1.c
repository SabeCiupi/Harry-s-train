/*CIUPITU Alexandra-Isabela - 311CC*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*DEFINIRE STRUCTURI*/
// definire structura vagon - lista dublu inlantuita
typedef struct vagon {
    char element;
    struct vagon* anterior;
    struct vagon* urmator;
} vagon;

// definire structura pentru tren - santinela (locomotiva), poz_curenta(mecanic)
typedef struct {
    vagon* locomotiva;
    vagon* mecanic;
} tren;

// structura ajutatoare pentru coada - lista dublu inlantuita
typedef struct nod {
    char info[20];
    struct nod* ant;
    struct nod* urm;
} nod;

// definire structura pentru coada comenzi
typedef struct {
    nod* prim;
    nod* ultim;
} coada;

/*OPERATII PENTRU COADA*/
// initializare coada (vida)
void initCoada(coada* c) {
    c->prim = NULL;
    c->ultim = NULL;
}

// functie pentru adaugarea unui element in coada
void introducere(coada* c, char* s) {
    nod* nou = (nod*)malloc(sizeof(nod));

    // se copiaza continutul sirului, in informatia cozii
    strcpy(nou->info, s);
    nou->urm = NULL;

    // se verifica daca e primul element din coada
    if (c->prim == NULL) {
        c->prim = nou;
        c->ultim = nou;
        c->prim->ant = NULL;
        c->prim->urm = NULL;
        c->ultim->urm = NULL;
    } else {
        c->ultim->urm = nou;
        nou->ant = c->ultim;
        c->ultim = nou;
        c->ultim->urm = NULL;
    }
}

// functie de extragere a primului element
char* extragere(coada* c) {
    nod* temp = c->prim;
    char* sir = malloc(strlen(temp->info) + 1);

    // copiem in variabila sir, informatia elementului si stergem nodul
    strcpy(sir, temp->info);
    c->prim = c->prim->urm;

    if (c->prim == NULL) {
        c->ultim = NULL;
    } else {
        c->prim->ant = NULL;
    }

    free(temp);
    temp = NULL;

    return sir;
}

// functie de eliberare a continutului din coada
void elibereazaCoada(coada** c) {
    // ruleaza cat timp coada nu e goala
    while ((*c)->prim != NULL) {
        nod* temp = (*c)->prim;
        (*c)->prim = (*c)->prim->urm;

        if ((*c)->prim == NULL) {
            (*c)->ultim = NULL;
        } else {
            (*c)->prim->ant = NULL;
        }

        free(temp);
    }

    free(*c);
}

/*FUNCTIE PENTRU INITIALIZAREA TRENULUI*/
tren* init(void) {
    // alocare memorie
    tren* t = (tren*)malloc(sizeof(tren));

    // programare defensiva
    if (t == NULL) {
        printf("Alocarea dinamica a esuat! :(");
        return NULL;
    }

    // alocam memorie pentru locomotiva
    t->locomotiva = (vagon*)malloc(sizeof(vagon));

    // initializare locomotiva
    t->locomotiva->element = ' ';
    t->locomotiva->anterior = t->locomotiva;
    t->locomotiva->urmator = t->locomotiva;

    // initializam primul vagon
    t->mecanic = (vagon*)malloc(sizeof(vagon));
    t->mecanic->element = '#';
    t->mecanic->anterior = t->locomotiva;
    t->mecanic->urmator = t->locomotiva;
    t->locomotiva->anterior = t->mecanic;
    t->locomotiva->urmator = t->mecanic;

    return t;
}

/* OPERATII DE TIP UPDATE */
// mutare spre stanga (MOVE_LEFT)
void mutaStanga(tren* t) {
    if (t->mecanic->anterior == t->locomotiva) {
        t->mecanic = t->locomotiva->anterior;
    } else {
        t->mecanic = t->mecanic->anterior;
    }
}

// mutare spre dreapta (MOVE_RIGHT)
void mutaDreapta(tren* t) {
    if (t->mecanic->urmator == t->locomotiva) {
        // crearea unui nou vagon daca ne aflam in ultimul vagon
        vagon* nou = (vagon*)malloc(sizeof(vagon));

        // programare defensiva
        if (nou == NULL) {
            printf("Alocarea dinamica a esuat! :(");
            return;
        }

        // initializari si refacere de legaturi
        nou->element = '#';
        nou->anterior = t->mecanic;
        nou->urmator = t->locomotiva;
        t->locomotiva->anterior->urmator = nou;
        t->locomotiva->anterior = nou;
        t->mecanic = nou;
    } else {
        t->mecanic = t->mecanic->urmator;
    }
}

// schimba inscriptia vagonului in care se afla mecanicul (WRITE)
void scrie(tren* t, char c) {
    t->mecanic->element = c;
}

// stergere vagon (CLEAR_CELL)
void stergereVagon(tren* t) {
    // verificam daca vagonul pe care dorim sa-l stergem este primul vagon sau
    // ultimul vagon pentru a face legaturile corespunzatoare
    if (t->mecanic->anterior == t->locomotiva) {
        t->locomotiva->urmator = t->mecanic->urmator;
        t->mecanic->urmator->anterior = t->locomotiva;
        vagon* temp = t->mecanic;
        t->mecanic = t->locomotiva->anterior;
        free(temp);
    } else if (t->mecanic->urmator == t->locomotiva) {
        t->mecanic->anterior->urmator = t->locomotiva;
        t->locomotiva->anterior = t->mecanic->anterior;
        vagon* temp = t->mecanic;
        t->mecanic = t->mecanic->anterior;
        free(temp);
    } else {
        t->mecanic->anterior->urmator = t->mecanic->urmator;
        t->mecanic->urmator->anterior = t->mecanic->anterior;
        vagon* temp = t->mecanic;
        t->mecanic = t->mecanic->anterior;
        free(temp);
    }
}

// aduce trenuletul la starea sa initiala (CLEAR_ALL)
void stergereCompleta(tren** t) {
    vagon* curent = (*t)->locomotiva;
    vagon* urm;

    do {
        urm = curent->urmator;
        free(curent);
        curent = urm;
    } while (curent != (*t)->locomotiva);

    free(*t);

    *t = init();
}

// functie care elibereaza memoria pentru tren definitiv
void elibereazaTren(tren** t) {
    vagon* curent = (*t)->locomotiva;
    vagon* urm;

    do {
        urm = curent->urmator;
        free(curent);
        curent = urm;
    } while (curent != (*t)->locomotiva);

    free(*t);
}

// inserare stanga (INSERT_LEFT)
void inserareStanga(tren* t, char c) {
    // nu putem insera la stanga primului vagon
    if (t->mecanic->anterior == t->locomotiva) {
        FILE* file2 = fopen("tema1.out", "a");
        fprintf(file2, "ERROR\n");
        fclose(file2);
    } else {
        // se adauga un nou vagon
        vagon* nou = (vagon*)malloc(sizeof(vagon));

        // programare defensiva
        if (nou == NULL) {
            printf("Alocarea dinamica a esuat! :(");
            return;
        }

        nou->element = c;
        nou->anterior = t->mecanic->anterior;
        nou->urmator = t->mecanic;

        t->mecanic->anterior->urmator = nou;
        t->mecanic->anterior = nou;
        t->mecanic = t->mecanic->anterior;
    }
}

// inserare dreapta (INSERT_RIGHT)
void inserareDreapta(tren* t, char c) {
    // se verifica daca e ultimul vagon pentru a face legaturile cu locomotiva
    if (t->mecanic->urmator == t->locomotiva) {
        vagon* nou = (vagon*)malloc(sizeof(vagon));

        // programare defensiva
        if (nou == NULL) {
            printf("Alocarea dinamica a esuat! :(");
            return;
        }

        nou->element = c;
        nou->anterior = t->mecanic;
        nou->urmator = t->locomotiva;
        t->mecanic->urmator = nou;
        t->locomotiva->anterior = nou;
        t->mecanic = t->mecanic->urmator;
    } else {
        vagon* nou = (vagon*)malloc(sizeof(vagon));

        // programare defensiva
        if (nou == NULL) {
            printf("Alocarea dinamica a esuat! :(");
            return;
        }

        nou->element = c;
        nou->anterior = t->mecanic;
        nou->urmator = t->mecanic->urmator;
        t->mecanic->urmator->anterior = nou;
        t->mecanic->urmator = nou;
        t->mecanic = t->mecanic->urmator;
    }
}

/*OPERATII DE TIP SEARCH*/
// functia care cauta circular un sir format de vagoanele trenuletului (SEARCH)
void cautare(tren* t, char* s) {
    vagon* santi = t->mecanic;  // retinem pozitia initiala
    vagon* prim = NULL;
    int ind = 0;
    int gasit = 0;
    int lungime = strlen(s);

    // ruleaza cat timp pozitia curenta este diferita de pozitia initiala
    do {
        // verificam daca elementul corespunde cu caracterul din sir
        if (t->mecanic->element == s[ind]) {
            // verificam daca sirul are doar un caracter
            if (lungime == 1) {
                gasit = 1;
                prim = t->mecanic;
                break;
            }

            // verificam daca s-a atins lungimea sirului
            if (ind == lungime - 1) {
                gasit = 1;
                break;
            }

            // reinitializam prim
            if (ind == 0) {
                prim = t->mecanic;
            }

            ind++;
        } else {
            ind = 0;

            // verificam daca elementul curent nu este egal cu primul caracter
            if (t->mecanic->element == s[0]) {
                prim = t->mecanic;
                ind++;
            }

            prim = t->mecanic;
        }

        // mutam pozitia curenta
        if (t->mecanic->urmator == t->locomotiva) {
            t->mecanic = t->locomotiva->urmator;
        } else {
            t->mecanic = t->mecanic->urmator;
        }
    } while (t->mecanic != santi);

    // verificam daca sirul nu a fost gasit
    if (!gasit) {
        FILE* file2 = fopen("tema1.out", "a");
        fprintf(file2, "ERROR\n");
        fclose(file2);
        t->mecanic = santi;
    } else {
        t->mecanic = prim;
    }
}

// functia care cauta la stanga (SEARCH_LEFT)
void cautareStanga(tren* t, char* s) {
    vagon* santi = t->mecanic;

    int ind = 0;
    int gasit = 0;
    int lungime = strlen(s);

    // cauta pana cand ajunge la locomotiva
    while (t->mecanic != t->locomotiva) {
        if (t->mecanic->element == s[ind]) {
            if (ind == lungime - 1) {
                gasit = 1;
                break;
            }

            ind++;
            t->mecanic = t->mecanic->anterior;
        } else if (t->mecanic->element != s[ind]) {
            ind = 0;
            t->mecanic = t->mecanic->anterior;
        }
    }

    // daca nu a gasit, afiseaza un mesaj si muta mecanicul pe pozitia initiala
    if (!gasit) {
        FILE* file2 = fopen("tema1.out", "a");
        fprintf(file2, "ERROR\n");
        fclose(file2);
        t->mecanic = santi;
    }
}

// functia care cauta la dreapta (SEARCH_RIGHT)
void cautareDreapta(tren* t, char* s) {
    vagon* santi = t->mecanic;

    int ind = 0;
    int gasit = 0;
    int lungime = strlen(s);

    // cauta cat timp mecanicul nu se afla in locomotiva
    while (t->mecanic != t->locomotiva) {
        if (t->mecanic->element == s[ind]) {
            if (lungime == 1) {
                gasit = 1;
                break;
            }
            if (ind == lungime - 1) {
                gasit = 1;
                break;
            }

            ind++;
            t->mecanic = t->mecanic->urmator;
        } else if (t->mecanic->element != s[ind]) {
            ind = 0;
            t->mecanic = t->mecanic->urmator;
        }
    }

    // daca nu a gasit, afiseaza un mesaj si muta mecanicul in pozitia initiala
    if (!gasit) {
        FILE* file2 = fopen("tema1.out", "a");
        fprintf(file2, "ERROR\n");
        fclose(file2);
        t->mecanic = santi;
    }
}

/*OPERATII DE TIP QUERY*/
// functia care arata vagonul unde se afla mecanicul (SHOW_CURRENT)
void arataMecanic(tren* t) {
    FILE* file2 = fopen("tema1.out", "a");
    fprintf(file2, "%c\n", t->mecanic->element);
    fclose(file2);
}

// functia care afiseaza trenul tinand cont de pozitia mecanicului (SHOW)
void afisareTren(tren* t) {
    vagon* tmp = t->locomotiva;
    tmp = tmp->urmator;
    FILE* file2 = fopen("tema1.out", "a");

    while (tmp != t->locomotiva) {
        if (tmp == t->mecanic) {
            fprintf(file2, "|%c|", tmp->element);
        } else {
            fprintf(file2, "%c", tmp->element);
        }

        tmp = tmp->urmator;
    }
    fprintf(file2, "\n");

    fclose(file2);
}

// functia care schimba ordinea operatiilor deja aflate in coada (SWITCH)
void schimbaOrd(coada* c) {
    if (c == NULL || c->prim == c->ultim) {
        return;
    }

    nod* curent = c->prim;
    nod* temp = NULL;

    while (curent != NULL) {
        temp = curent->urm;
        curent->urm = curent->ant;
        curent->ant = temp;

        curent = temp;
    }

    temp = c->prim;
    c->prim = c->ultim;
    c->ultim = temp;
}

int main(void) {
    // initializare tren
    tren* t = init();

    // initializare coada
    coada* c = malloc(sizeof(coada));
    initCoada(c);

    // deschidere fisier
    FILE* file = fopen("tema1.in", "r");
    if (file == NULL) {
        printf("Eroare la deschidere");
        return 1;
    }

    // initializari
    int nr, i, lung;
    char sir[20], *sir2;
    char car;

    // citire
    fgets(sir, sizeof(sir), file);
    lung = strlen(sir);
    if (lung > 0 && sir[lung - 1] == '\n') {
        sir[lung - 1] = '\0';
    }

    nr = atoi(sir);

    // stergere continut fisier.out
    FILE* file2 = fopen("tema1.out", "w");
    fclose(file2);

    file2 = fopen("tema1.out", "a");

    // parcurgere comenzi din fisierul file
    for (i = 0; i < nr; i++) {
        fgets(sir, sizeof(sir), file);
        int lung = strlen(sir);
        if (lung > 0 && sir[lung - 1] == '\n') {
            sir[lung - 1] = '\0';
        }

        // verificam daca comanda este execute, in caz afirmativ extragem din
        // coada prima comanda si o executam. In alte cazuri, verificam daca
        // comanda este una care se executa direct
        if (!strcmp(sir, "EXECUTE")) {
            sir2 = extragere(c);

            if (sir != NULL) {
                if (!strcmp(sir2, "MOVE_LEFT")) {
                    mutaStanga(t);
                } else if (!strcmp(sir2, "MOVE_RIGHT")) {
                    mutaDreapta(t);
                } else if (strstr(sir2, "WRITE")) {
                    car = sir2[6];
                    scrie(t, car);
                } else if (!strcmp(sir2, "CLEAR_CELL")) {
                    stergereVagon(t);
                } else if (!strcmp(sir2, "CLEAR_ALL")) {
                    stergereCompleta(&t);
                } else if (strstr(sir2, "INSERT_LEFT")) {
                    car = sir2[12];
                    inserareStanga(t, car);
                } else if (strstr(sir2, "INSERT_RIGHT")) {
                    car = sir2[13];
                    inserareDreapta(t, car);
                } else if (strstr(sir2, "SEARCH_LEFT")) {
                    char sir3[30];
                    strcpy(sir3, sir2 + strlen("SEARCH_LEFT "));
                    cautareStanga(t, sir3);
                } else if (strstr(sir2, "SEARCH_RIGHT")) {
                    char sir3[30];
                    strcpy(sir3, sir2 + strlen("SEARCH_RIGHT "));
                    cautareDreapta(t, sir3);
                } else if (strstr(sir2, "SEARCH")) {
                    char sir3[30];
                    strcpy(sir3, sir2 + strlen("SEARCH "));
                    cautare(t, sir3);
                }

                free(sir2);
            }
        } else if (!strcmp(sir, "SHOW_CURRENT")) {
            arataMecanic(t);
        } else if (!strcmp(sir, "SHOW")) {
            afisareTren(t);
        } else if (!strcmp(sir, "SWITCH")) {
            schimbaOrd(c);
        } else {
            introducere(c, sir);
        }
    }

    // eliberare memorie si inchidere fisiere
    elibereazaTren(&t);
    elibereazaCoada(&c);
    fclose(file);
    fclose(file2);

    return 0;
}