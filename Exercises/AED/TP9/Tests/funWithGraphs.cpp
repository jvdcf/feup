#include "funWithGraphs.h"


//=============================================================================
// Exercise 2: Vertex degrees
//=============================================================================
// Subexercise 2.1: outDegree
//=============================================================================
int FunWithGraphs::outDegree(const Graph<int> g, const int &v) {
    Vertex<int>* vertex = g.findVertex(v);
    if (vertex == NULL) return -1;
    return vertex->getAdj().size();
}


//=============================================================================
// Subexercise 2.2: inDegree
//=============================================================================
int FunWithGraphs::inDegree(const Graph<int> g, const int &v) {
    Vertex<int>* vertex = g.findVertex(v);
    if (vertex == NULL) return -1;
    int count = 0;
    for (Vertex<int>* vertex1 : g.getVertexSet()) {
        for (Edge<int> edge : vertex1->getAdj()) {
            if (edge.getDest() == vertex) count++;
        }
    }
    return count;
}

//=============================================================================
// Subexercise 2.3: weightedOutDegree
//=============================================================================
int FunWithGraphs::weightedOutDegree(const Graph<int> g, const int &v) {
    Vertex<int>* vertex = g.findVertex(v);
    if (vertex == NULL) return -1;
    int count = 0;
    for (Edge<int> edge : vertex->getAdj()) {
        count += edge.getWeight();
    }
    return count;
}
