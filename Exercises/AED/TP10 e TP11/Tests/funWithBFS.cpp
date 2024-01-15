#include "funWithBFS.h"


//=============================================================================
// Exercise 2.1: Nodes at distance k
//=============================================================================
// Subexercise 2.1.2: Nodes at distance k (BFS)
//=============================================================================
// TODO
vector<Person> FunWithBFS::nodesAtDistanceBFS(const Graph<Person> *g, const Person &source, int k) {
    for (auto v : g->getVertexSet()) v->setVisited(false);
    vector<Person> res;
    Vertex<Person> *v = g->findVertex(source);
    queue<Vertex<Person> *> q;
    q.push(v);
    int countThisLevel = 1;
    while (!q.empty()) {
        if (countThisLevel == 0) {
            countThisLevel = (int) q.size();
            k--;
        }

        v = q.front();
        q.pop();
        countThisLevel--;

        if (!v->isVisited()) {
            v->setVisited(true);
            if (k <= 0) {
                res.push_back(v->getInfo());
            } else {
                for (auto &e : v->getAdj()) {
                    if (!e.getDest()->isVisited()) {
                        q.push(e.getDest());
                    }
                }
            }
        }
    }
    return res;
}

//=============================================================================
// Exercise 2.2: Max New Children
//=============================================================================
// TODO
int FunWithBFS::maxNewChildren(const Graph<Person> *g, const Person &source, Person &info) {
    for (auto v : g->getVertexSet()) v->setVisited(false);

    Vertex<Person> *v = g->findVertex(source);
    queue<Vertex<Person> *> q;
    int maxChildren = 0;

    v->setVisited(true);
    q.push(v);

    while (!q.empty()) {
        int thisMaxChildren = 0;
        v = q.front();
        q.pop();


        for (auto &e : v->getAdj()) {
            if (!e.getDest()->isVisited()) {
                thisMaxChildren++;
                e.getDest()->setVisited(true);
                q.push(e.getDest());
            }
        }

        if (thisMaxChildren > maxChildren) {
            maxChildren = thisMaxChildren;
            info = v->getInfo();
        }
    }

    return maxChildren;
}
