#include "funSetProblem.h"

FunSetProblem::FunSetProblem() {}


//=============================================================================
// Exercise 2: FunSetProblem
//=============================================================================
// TODO
pair<int,int> FunSetProblem::pairSum(const vector<int> &values, int sum) {
    set<int> binaryTree;
    for (int v: values) {
        if (binaryTree.find(sum - v) != binaryTree.end()) {
            return make_pair(v, sum - v);
        }
        binaryTree.insert(v);
    }
    return make_pair(0, 0);
}
