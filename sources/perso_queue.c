/* perso_queue.c */
// Jérémie Leclerc & Pierrick Hue

#include "headers/header.h"

void displayList(QUEUE_INT* queue) {
    CELL_INT* cell_int = queue->head;

    while (cell_int != NULL) {
        printf("Numero de sommet : %d\n", cell_int->value);
        cell_int = cell_int->next;
    }
}

QUEUE_INT newEmptyQueue_int(void) {
    QUEUE_INT queue = {0, NULL, NULL};
    return queue;
}

int isEmpty_int(QUEUE_INT queue) {
    return !queue.head;
}

QUEUE_INT add_int(int element, QUEUE_INT queue) {
    CELL_INT* cell_int = (CELL_INT*) malloc(sizeof (CELL_INT));
    cell_int->value = element;
    cell_int->next = NULL;

    if (isEmpty_int(queue)) {
        queue.head = queue.tail = cell_int; /* la cellule créée est à la fois tête et queue */
    } else {
        queue.tail = queue.tail->next = cell_int; /* la cellule créée est la nouvelle queue	*/
    }

    queue.size += 1;
    return queue;
}

int getHeadValue_int(QUEUE_INT queue)
/* ATTENTION: consulter la tête d'une File vide n'a pas de sens */ {
    if (isEmpty_int(queue)) {
        printf("Error - The queue is empty\n");
        exit(-1);
    }

    return queue.head->value; /* la File n'est pas modifiée */
}

QUEUE_INT get_int(QUEUE_INT queue)
/* ATTENTION: supprimer la tête d'une File vide n'a pas de sens */ {
    CELL_INT* cell_int = queue.head;

    if (isEmpty_int(queue)) {
        printf("Error - The queue is empty\n");
        exit(-1);
    }

    if (queue.head == queue.tail) {
        queue = newEmptyQueue_int(); /* la File n'avait plus qu'une seule cellule */
    } else {
        queue.head = queue.head->next; /* la queue ne change pas */
    }

    free(cell_int);
    queue.size -= 1;

    return queue;
}