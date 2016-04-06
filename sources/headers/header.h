/* header.h */
//Jérémie Leclerc & Pierrick Hue

#ifndef HEADER_H
#define HEADER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXSOMMET       250
#define MAXVALUATION    20.0
#define MAXSUCC         10

typedef int ELEMENT;

typedef struct CELL {
    int extremity;
    int value;
    struct CELL* next;
} CELL;

typedef struct CELL_INT {
    int value;
    struct CELL_INT* next;
} CELL_INT;

typedef struct NODE {
    int name;
    int earlyDate;
    int lateDate;
}NODE;

typedef struct LADJ /* listes d'adjacence */ {
    int nbSom;
    int nbArc;
    CELL** tab;
    int* predNumber;
    int* succNumber;
    NODE** nodeTab;
} LADJ;

typedef struct QUEUE {
    int size;
    CELL* head;
    CELL* tail;
} QUEUE;

typedef struct QUEUE_INT {
    int size;
    CELL_INT* head;
    CELL_INT* tail;
} QUEUE_INT;

typedef struct REALATED_COMPONENTS {
    int* relatedComponents;
    int nbComponents;
} REALATED_COMPONENTS;

typedef struct GRAPH {
    int nbSummit;
    int nbArc;
    float** matrix; /* matrice d'adjacence */
    char* nodeStatus;
    REALATED_COMPONENTS componentInfo;
} GRAPH;

typedef struct t_ens {
    int* parent;
    int nbElem;
    int nbClass;
} t_ens;

//L_ADJ.c
void    affiche_graphe(LADJ);
void    affiche_liste(CELL*);
LADJ    charge_graphe(char*);
CELL*   creer_cellule(int, int, CELL*);
LADJ    init_ladj(int, int);
LADJ    inverse(LADJ);

//main.c
QUEUE_INT topologicalMarking(LADJ*);
LADJ load_graph(char*);
QUEUE_INT checkAndAddCriticalTask(NODE*, QUEUE_INT);

//queue.c
QUEUE   add(ELEMENT, QUEUE);
ELEMENT add_2(QUEUE*);
void    concat(QUEUE*, QUEUE*);
void    displayQueue(char*, QUEUE);
QUEUE   get(QUEUE);
ELEMENT getHeadValue(QUEUE);
int     isEmpty(QUEUE);
QUEUE   newEmptyQueue();

//perso_queue.c
QUEUE_INT   add_int(int, QUEUE_INT);
void        displayList(QUEUE_INT*);
int         getHeadValue_int(QUEUE_INT);
QUEUE_INT   get_int(QUEUE_INT);
int         isEmpty_int(QUEUE_INT);
QUEUE_INT   newEmptyQueue_int();

#endif	// HEADER_H

 
