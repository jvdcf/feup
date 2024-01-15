#include "funSortProblem.h"

FunSortProblem::FunSortProblem() {}


//=============================================================================
// Exercise 4: Min Difference
//=============================================================================
int FunSortProblem::minDifference(const vector<unsigned> &values, unsigned nc) {
    if (values.size() < nc) return -1;

    vector<unsigned> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    int lowest_diff = sorted[nc - 1] - sorted[0];
    for (int lowest = 1; lowest < values.size() - nc; lowest++) {
        int highest = lowest + nc - 1;
        int this_diff = sorted[highest] - sorted[lowest];
        if (this_diff < lowest_diff) lowest_diff = this_diff;
    }
    return lowest_diff;
}

//=============================================================================
// Exercise 6: Num Inversions (extra)
//=============================================================================
//TODO
unsigned FunSortProblem::numInversions(vector <int> v) {
    return 0;
}


//=============================================================================
// Exercise 7: Nuts and Bolts (extra)
//=============================================================================
// TODO
void FunSortProblem::nutsBolts(vector<Piece> &nuts, vector<Piece> &bolts) {
}
