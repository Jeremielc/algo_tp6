/* main.c */
//Jérémie Leclerc & Pierrick Hue

#include "headers/header.h"

int main(int argc, char** argv) {
    GRAPH_L_ADJ graph;

    if (argc != 2) {
        printf("%s\n", "Veuillez specifier un fichier de graphe.");
        exit(-1);
    }

    graph = load_graph(argv[1]);

    //Marquage topologique
    printf("Nombre de predecesseur de chacun des noeuds du graphe : \n");
    for (int i = 0; i < graph.nbSom; i++) {
        printf("Noeud %d : %d\n", i, graph.predNumber[i]);
    }
    printf("\n");

    QUEUE_INT topologicalQueue = topologicalMarking(&graph);

    computeEarlyDate(&graph); //fournir date au plus tot
    computeLateDate(&graph); //date au plus tard
    computeTolerances(&graph); //marge de toutes les taches
    computeCritacalTasks(&graph, topologicalQueue); //liste des taches critiques
    //date de fin de travaux
    printf("Date de fin des travaux : %d", graph.nodeTab[graph.nbSom - 1]->lateDate); //Date au plus tard de omega.

    free(graph.predNumber);
    free(graph.nodeTab);
    free(graph.predTab);
    free(graph.succTab);

    return 0;
}

QUEUE_INT topologicalMarking(GRAPH_L_ADJ* graph) {
    if (graph == NULL) {
        QUEUE_INT emptyQueue = newEmptyQueue_int();
        return emptyQueue;
    }

    QUEUE_INT summitWithoutPredQueue, topologicalQueue;
    //bool topologyIsPossible = false;

    summitWithoutPredQueue = newEmptyQueue_int();
    topologicalQueue = newEmptyQueue_int();

    // 1 - Recherche des sommets sans prédecesseurs
    for (int i = 0; i < graph->nbSom; i++) {
        if (graph->predNumber[i] == 0) {
            summitWithoutPredQueue = add_int(i, summitWithoutPredQueue);
        }
    }

    // 2 - Reconstruction du graphe
    int value = 0;
    CELL* temp;

    while (!isEmpty_int(summitWithoutPredQueue)) {
        value = getHeadValue_int(summitWithoutPredQueue);
        temp = graph->succTab[value];

        /*if (temp) {
            printf("%s %d\n", "temp->extremity :", temp->extremity);
        }*/

        while (temp != NULL) {
            graph->predNumber[temp->extremity] = graph->predNumber[temp->extremity] - 1;

            if (graph->predNumber[temp->extremity] == 0) {
                summitWithoutPredQueue = add_int(temp->extremity, summitWithoutPredQueue);
            }

            temp = temp->next;
        }

        topologicalQueue = add_int(value, topologicalQueue);
        summitWithoutPredQueue = get_int(summitWithoutPredQueue); //Utiliser get pour retirer le noeud dans la file.
    }

    //displayList(&topologicalQueue);

    /*if (topologicalQueue.size == graph->nbSom) {
        topologyIsPossible = true;
    }*/

    //return topologyIsPossible;
    return topologicalQueue;
}

GRAPH_L_ADJ load_graph(char* fileName) {
    GRAPH_L_ADJ graph;
    int nbsom, nbar, ori, ext; //, val;
    FILE* canal;
    CELL* cell;

    canal = fopen(fileName, "rt");

    if (canal == NULL) {
        exit(-1);
    }

    fscanf(canal, "%d %d", &nbsom, &nbar);
    graph = init_ladj(nbsom, nbar);

    for (int i = 0; i < nbar; i++) {
        fscanf(canal, "%d %d", &ori, &ext);

        //Initialisation du tableau de successeurs
        cell = creer_cellule(ext, 0, graph.succTab[ori]);
        graph.succTab[ori] = cell; //on empile dans le tableau des successeurs

        //Initialisation du tableau de predecesseurs
        cell = creer_cellule(ori, 0, graph.predTab[ext]);
        graph.predTab[ext] = cell; //on empile dans le tableau des predecesseurs.

        graph.predNumber[ext] += 1; //Pour le marquage topologique.
    }

    char* alias = NULL;
    int id, duration;

    for (int i = 0; i < graph.nbSom; i++) {
        if (alias != NULL) {
            free(alias);
        }

        alias = (char*) calloc(30, sizeof (char));
        fscanf(canal, "%d %d %s", &id, &duration, alias);

        graph.nodeTab[i]->id = id;
        graph.nodeTab[i]->duration = duration;
        graph.nodeTab[i]->name = alias;
    }

    fclose(canal);

    return graph;
}

QUEUE_INT checkAndAddCriticalTask(TASK* node, QUEUE_INT criticalTasks) {
    if (node->earlyDate == node->lateDate) {
        criticalTasks = add_int(node->id, criticalTasks);
    }

    return criticalTasks;
}

void computeCritacalTasks(GRAPH_L_ADJ* graph, QUEUE_INT topologicalQueue) {
    QUEUE_INT criticalTemp = topologicalQueue;
    QUEUE_INT criticalTasks = newEmptyQueue_int();

    for (int i = 0; i < topologicalQueue.size; i++) {
        TASK* node = graph->nodeTab[getHeadValue_int(criticalTemp)];
        criticalTasks = checkAndAddCriticalTask(node, criticalTasks);

        criticalTemp = get_int(criticalTemp);
    }

    int size = criticalTasks.size;
    for (int i = 0; i < size; i++) {
        int taskName = getHeadValue_int(criticalTasks);
        printf("%s : %d\n", "Tache critique", taskName);

        criticalTasks = get_int(criticalTasks);
    }
}

void computeEarlyDate(GRAPH_L_ADJ* graph) {
    for (int i = 1; i < graph->nbSom; i++) {
        int temp;
        CELL* cell = graph->predTab[i];

        while (cell != NULL) {
            temp = graph->nodeTab[cell->extremity]->duration + graph->nodeTab[cell->extremity]->earlyDate;

            if (temp > graph->nodeTab[i]->earlyDate) {
                graph->nodeTab[i]->earlyDate = temp;
            }

            cell = cell->next;
        }
    }

    graph->nodeTab[graph->nbSom - 1]->lateDate = graph->nodeTab[graph->nbSom - 1]->earlyDate; //Date au plus tard de omega = date au plus tot.
    int temp = graph->nodeTab[graph->nbSom - 1]->lateDate;

    for (int i = 1; i < graph->nbSom; i++) {
        graph->nodeTab[i]->lateDate = temp;
    }

    for (int i = 0; i < graph->nbSom; i++) {
        printf("Date au plus tot de %d : %d\n", i, graph->nodeTab[i]->earlyDate);
    }
    printf("\n");
}

void computeLateDate(GRAPH_L_ADJ* graph) {
    for (int i = graph->nbSom - 1; i > 1; i--) {
        int temp;
        CELL* cell = graph->predTab[i];

        while (cell != NULL) {
            temp = graph->nodeTab[cell->extremity]->lateDate - graph->nodeTab[i]->duration;

            if (temp < graph->nodeTab[i]->lateDate) {
                graph->nodeTab[cell->extremity]->lateDate = temp;
            }

            cell = cell->next;
        }
    }

    for (int i = 0; i < graph->nbSom; i++) {
        printf("Date au plus tard de %d : %d\n", i, graph->nodeTab[i]->lateDate);
    }
    printf("\n");
}

void computeTolerances(GRAPH_L_ADJ* graph) {
    for (int i = 0; i < graph->nbSom; i++) {
        graph->nodeTab[i]->tolerance = graph->nodeTab[i]->lateDate - graph->nodeTab[i]->earlyDate;
    }

    for (int i = 0; i < graph->nbSom; i++) {
        printf("Marge d'erreur de la tache %d : %d\n", i, graph->nodeTab[i]->tolerance);
    }
    printf("\n");
}