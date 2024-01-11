#include "funPQProblem.h"
#include <queue>

using namespace std;

FunPQProblem::FunPQProblem() {}


//=============================================================================
// Exercise 2: Minimum Cost
//=============================================================================
int FunPQProblem::minCost(const vector<int> &ropes) {
  priority_queue<int, vector<int>, greater<int>> sortedRopes;
  int cost = 0;

  for (int r: ropes) sortedRopes.push(r);

  while (!sortedRopes.empty()) {
    int rope1 = sortedRopes.top();
    sortedRopes.pop();
    if (sortedRopes.empty()) break;
    int rope2 = sortedRopes.top();
    sortedRopes.pop();

    cost += rope1 + rope2;
    sortedRopes.push(rope1 + rope2);
  }

  return cost;
}
