#include "funWithGraphs.h"

//=============================================================================
// Exercise 2.1: Connected Components
//=============================================================================
void dfsCC(Vertex<int> *v);
int funWithGraphs::connectedComponents(Graph<int> *g) {
    for (Vertex<int>* v: g->getVertexSet()) v->setVisited(false);

    int counter = 0;
    for (Vertex<int>* v: g->getVertexSet()) {
        if (!v->isVisited()) {
            counter++;
            dfsCC(v);
        }
    }

    return counter;
}

void dfsCC(Vertex<int> *v) {
    v->setVisited(true);
    for (auto & e : v->getAdj()) {
        auto w = e.getDest();
        if ( ! w->isVisited() )
            dfsCC(w);
    }
}



//=============================================================================
// Exercise 2.2: Giant Component
//=============================================================================
int dfsGC(Vertex<int> *v);
int funWithGraphs::giantComponent(Graph<int> *g) {
    for (Vertex<int>* v: g->getVertexSet()) v->setVisited(false);

    int maxCount = 0;
    for (Vertex<int>* v: g->getVertexSet()) {
        if (!v->isVisited()) {
            maxCount = max(dfsGC(v), maxCount);
        }
    }

    return maxCount;
}

int dfsGC(Vertex<int> *v) {
    int verticesCount = 1;
    v->setVisited(true);

    for (const Edge<int> e: v->getAdj()) {
        Vertex<int>* a = e.getDest();
        if (!a->isVisited()) {
            verticesCount += dfsGC(a);
        }
    }

    return verticesCount;
}


//=============================================================================
// Exercise 2.3: Strongly Connected Components
//=============================================================================
void dfs_scc(Graph<int> *g, Vertex<int> *v, stack<int> &s, list<list<int>> &l, int &i);
bool stackHasVertex(stack<int> s, Vertex<int>* v);

list<list<int>> funWithGraphs::scc(Graph<int> *g){
    for (Vertex<int>* v: g->getVertexSet()) {
        v->setVisited(false);
    }
    list<list<int>> res;
    int index = 1;


    for (Vertex<int>* v: g->getVertexSet()) {
        if (!v->isVisited()) {
            stack<int> s;
            dfs_scc(g, v, s, res, index);
        }
    }
    return res;
}

void dfs_scc(Graph<int> *g, Vertex<int> *v, stack<int> &s, list<list<int>> &l, int &i) {
    v->setLow(i);
    v->setNum(v->getLow());
    i++;
    s.push(v->getInfo());
    v->setVisited(true);

    for (const Edge<int> e: v->getAdj()) {
        Vertex<int>* w = e.getDest();
        if (!w->isVisited()) {
            // Tree edge | Forward
            dfs_scc(g, w, s, l, i);
            v->setLow(min(w->getLow(), v->getLow()));

        } else if (stackHasVertex(s, w)) {
            // Back edge | Backwards
            v->setLow(min(w->getNum(), v->getLow()));
        }
    }

    // Root of SCC
    if (v->getNum() == v->getLow()) {
        list<int> scc;
        int w = s.top();
        while (w != v->getInfo()) {
            s.pop();
            scc.push_back(w);
            w = s.top();
        }
        s.pop();
        scc.push_back(v->getInfo());

        l.push_back(scc);
    }
}

bool stackHasVertex(stack<int> s, Vertex<int>* v) {
    while (!s.empty()) {
        if (s.top() == v->getInfo()) return true;
        s.pop();
    }
    return false;
}


//=============================================================================
// Exercise 2.4: Articulation Points
//=============================================================================
// TODO
void dfs_art(Graph<int> *g, Vertex<int> *v, stack<int> &s, unordered_set<int> &res, int &i);
unordered_set<int> funWithGraphs::articulationPoints(Graph<int> *g) {
    for (Vertex<int>* v: g->getVertexSet()) {
        v->setVisited(false);
    }
    unordered_set<int> res;
    int index = 0;


    for (Vertex<int>* v: g->getVertexSet()) {
        if (!v->isVisited()) {
            stack<int> s;
            dfs_art(g, v, s, res, index);
        }
    }
    return res;
}

void dfs_art(Graph<int> *g, Vertex<int> *v, stack<int> &s, unordered_set<int> &l, int &i) {
    int children = 0;
    v->setLow(++i);
    v->setNum(v->getLow());
    v->setVisited(true);
    s.push(v->getInfo());

    for (const Edge<int> e: v->getAdj()) {
        Vertex<int>* w = e.getDest();
        if (!w->isVisited()) {                                              // Tree edge
            children++;
            dfs_art(g, w, s, l, i);
            v->setLow(min(w->getLow(), v->getLow()));

            if ((v->getNum() != 1) && (w->getLow() >= v->getNum())) {       // Articulation point
                l.insert(v->getInfo());
            }

        } else if (w->isVisited()) {                                        // Back edge
            v->setLow(min(w->getNum(), v->getLow()));
        }
    }

    if ((v->getNum() == 1) && (children > 1)) {                             // Root special case
        l.insert(v->getInfo());
    }

    s.pop();
}