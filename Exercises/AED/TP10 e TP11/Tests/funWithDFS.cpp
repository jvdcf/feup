#include "funWithDFS.h"


//=============================================================================
// Exercise 2.1: Nodes at distance k
//=============================================================================
// Subexercise 2.1.1: Nodes at distance k (DFS)
//=============================================================================
void nodesAtDistanceDFSVisit(const Graph<Person> *g, Vertex<Person> *v, int k, vector<Person> &res);
vector<Person> FunWithDFS::nodesAtDistanceDFS(const Graph<Person> *g, const Person &source, int k) {
    for (auto v : g->getVertexSet()) v->setVisited(false);
    vector<Person> res;
    Vertex<Person> *v = g->findVertex(source);
    nodesAtDistanceDFSVisit(g, v, k, res);
    return res;
}

void nodesAtDistanceDFSVisit(const Graph<Person> *g, Vertex<Person> *v, int k, vector<Person> &res) {
    v->setVisited(true);
    if (k <= 0) {
        res.push_back(v->getInfo());
        return;
    }

    for (auto & e : v->getAdj()) {
        if (!e.getDest()->isVisited()) {
            nodesAtDistanceDFSVisit(g, e.getDest(), k - 1, res);
        }
    }
}


//=============================================================================
// Exercise 2.3: Directed Acyclic Graph
//=============================================================================
bool dfsIsDAG(Vertex<int> *v);
enum Color {white, gray, black};
template <class T>
Color getColor(const Vertex<T> *v);
template <class T>
void setColor(Vertex<T> *v, Color c);

/*
 * Adapted from this pseudocode:
 * color[v ∈ V ] ← white
 * For all nodes v of the graph do
 * If color [v ] = white then
 * dfs(v )
 */
bool FunWithDFS::isDAG(Graph<int> g) {
    for (auto v : g.getVertexSet()) setColor(v, white);

    for (auto v : g.getVertexSet()) {
        if (getColor(v) == white) {
            if (!dfsIsDAG(v)) {
                return false;
            }
        }
    }

    return true;
}

/*
 * Adapted from this pseudocode:
 * color[v ] ← gray
 * For all neighbors w of v do
 * If color[w ] = gray then
 * write(”Cycle found!”)
 * Else if color[w ] = white then
 * dfs(w )
 * color[v ] ← black
 */
bool dfsIsDAG(Vertex<int> *v) {
    setColor(v, gray);

    for (auto & e : v->getAdj()) {
        if (getColor(e.getDest()) == gray) {
            return false;
        } else if (getColor(e.getDest()) == white) {
            if (!dfsIsDAG(e.getDest())) {
                return false;
            }
        }
    }

    setColor(v, black);
    return true;
}

template <class T>
Color getColor(const Vertex<T> *v) {
    if (v->isVisited()) {
        return black;
    } else if (v->isProcessing()) {
        return gray;
    } else {
        return white;
    }
}

template <class T>
void setColor(Vertex<T> *v, Color c) {
    switch (c) {
        case white:
            v->setVisited(false);
            v->setProcessing(false);
            break;
        case gray:
            v->setVisited(false);
            v->setProcessing(true);
            break;
        case black:
            v->setVisited(true);
            v->setProcessing(false);
            break;
    }
}

