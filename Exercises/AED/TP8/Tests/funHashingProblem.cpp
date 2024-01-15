#include "funHashingProblem.h"

FunHashingProblem::FunHashingProblem() {}


//=============================================================================
// Exercise 1: Find Duplicates
//=============================================================================
vector<int> FunHashingProblem::findDuplicates(const vector<int>& values, int k) {
    vector<int> res;
    unordered_set<int> s;
    for (int i = 0; i < values.size(); i++) {
        // If element already exists in hash set, update result
        if (s.find(values[i]) != s.end()) {
            res.push_back(values[i]);
        }
        // Insert this element to hash set
        s.insert(values[i]);

        // Remove the k+1 distant element (max size of k)
        if (i >= k) {
            s.erase(values[i - k]);
        }
    }
    return res;
}
