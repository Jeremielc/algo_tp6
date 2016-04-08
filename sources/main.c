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
    
    //Marquage topologique//////////////////////////////////////////////////////
    for (int i = 0; i < graph.nbSom; i++) {
        printf("Noeud %d : %d\n", i, graph.predNumber[i]);
    }
    printf("\n");
    
    QUEUE_INT topologicalQueue = topologicalMarking(&graph);
    ////////////////////////////////////////////////////////////////////////////
    
    //fournir date au plus tot//////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    
    //date au plus tard/////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    
    //marge de toutes les taches////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    
    //liste des taches critiques////////////////////////////////////////////////
    QUEUE_INT criticalTemp = topologicalQueue;
    QUEUE_INT criticalTasks = newEmptyQueue_int();
    
    for (int i = 0; i < topologicalQueue.size; i++) {
        TASK* node = graph.nodeTab[getHeadValue_int(criticalTemp)];
        criticalTasks = checkAndAddCriticalTask(node, criticalTasks);
        
        criticalTemp = get_int(criticalTemp);
    }
    
    int size = criticalTasks.size;
    for (int i = 0; i < size; i++) {
        int taskName = getHeadValue_int(criticalTasks);
        printf("%s %d : %d\n", "Critical task number", i, taskName);
        
        criticalTasks = get_int(criticalTasks);
    }
    ////////////////////////////////////////////////////////////////////////////
    
    //date de fin de travaux////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    
    free(graph.predNumber);
    free(graph.nodeTab);

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
        temp = graph->tab[value];

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
    GRAPH_L_ADJ graph_pred, graph_succ;
    int nbsom, nbar, ori, ext, val;
    FILE* canal;
    CELL* cell;

    canal = fopen(fileName, "rt");

    if (canal == NULL) {
        exit(-1);
    }
    
    fscanf(canal, "%d %d", &nbsom, &nbar);
    graph_succ = init_ladj(nbsom, nbar);
    
    for (int i = 0; i < nbar; i++) {
        fscanf(canal, "%d %d", &ori, &ext);
        cell = creer_cellule(ext, 0, graph_pred.tab[ori]);
        graph_pred.tab[ori] = cell; //on empile
        
        //utiliser le tableau de predecesseur

        graph_pred.predNumber[ext] += 1;
    }

    fclose(canal);
    
    return graph_pred;
}

QUEUE_INT checkAndAddCriticalTask(TASK* node, QUEUE_INT criticalTasks) {
    if (node->earlyDate == node->lateDate) {
        criticalTasks = add_int(node->id, criticalTasks);
    }
    
    return criticalTasks;
}