/* L_ADJ.c */
//C.Porquet

#include "headers/header.h"

CELL* creer_cellule(int extr, int val, CELL* suiv) {
    CELL* p;

    p = (CELL*) malloc(sizeof (CELL));
    if (p == NULL) {
        exit(-1);
    }

    p->extremity = extr;
    p->value = val;
    p->next = suiv;

    return p;
}

void affiche_liste(CELL* p) {
    while (p) {
        printf("<%d-%d>\t", p->extremity, p->value);
        p = p->next;
    };
    
    printf("\n");
}

GRAPH_L_ADJ init_ladj(int nsom, int nar) /* allocation dynamique et initialisation */ {
    GRAPH_L_ADJ graph;

    graph.nbSom = nsom;
    graph.nbArc = nar;
    graph.tab = (CELL**) calloc(nsom, sizeof (CELL*));
    
    //Initialisation du tableau du nombre de predecesseurs.
    graph.predNumber = (int*) malloc(graph.nbSom * sizeof (int));
    for (int i = 0; i < graph.nbSom; i++) {
        graph.predNumber[i] = 0;
    }
    
    //Initialisation du tableau du nombre de noeuds.
    graph.nodeTab = (TASK**) malloc(nsom * sizeof(TASK*));
    TASK* nodeTemp = NULL;
    
    for (int i = 0; i < graph.nbSom; i++) {
        nodeTemp = (TASK*) malloc (sizeof(TASK));
        nodeTemp->name = i;
        nodeTemp->earlyDate = 0;
        nodeTemp->lateDate = 0;
        
        graph.nodeTab[i] = nodeTemp;
    }

    if (graph.tab == NULL) {
        exit(-1);
    }

    return graph;
}

void affiche_graphe(GRAPH_L_ADJ graph) {
    int i;

    for (i = 0; i < graph.nbSom; i++) {
        if (graph.tab[i]) {
            printf("Successeurs de %d : ", i);
            affiche_liste(graph.tab[i]);
        }
    }
    printf("\n");
}

GRAPH_L_ADJ charge_graphe(char* nom_fichier) {
    GRAPH_L_ADJ graph;
    int nsom, nar, i, ori, ext, val;
    FILE* fp;
    CELL* p;

    fp = fopen(nom_fichier, "rt");
    
    if (fp == NULL) {
        exit(-1);
    }
    
    fscanf(fp, "%d %d", &nsom, &nar);
    graph = init_ladj(nsom, nar);
    graph.predNumber = (int*) calloc(nsom, sizeof(int));
    
    for (i = 0; i < nar; i++) {
        fscanf(fp, "%d %d %d", &ori, &ext, &val);
        p = creer_cellule(ext, val, graph.tab[ori]);
        graph.tab[ori] = p; /* on empile */
    }
    
    fclose(fp);
    
    return graph;
}

GRAPH_L_ADJ inverse(GRAPH_L_ADJ graph) {
    GRAPH_L_ADJ graph_inverse;
    int i;
    CELL* p, *q;

    graph_inverse = init_ladj(graph.nbSom, graph.nbArc);
    
    for (i = 0; i < graph.nbSom; i++) {
        p = graph.tab[i];
        
        while (p) {
            q = creer_cellule(i, p->value, graph_inverse.tab[p->extremity]);
            graph_inverse.tab[p->extremity] = q; /* on empile */
            p = p->next;
        }
    }
    
    return graph_inverse;
}
