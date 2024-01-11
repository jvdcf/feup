#include "funWithSearch.h"


//=============================================================================
// Exercise 1: Elementary Search
//=============================================================================
// Subexercise 1.1: Linear Search
//=============================================================================
int FunWithSearch::searchLinear(const vector<int> &v, int key) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == key) {
            return i;
        }
    }
    return -1;
}

//=============================================================================
// Subexercise 1.2: Binary Search
//=============================================================================
int FunWithSearch::searchBinary(const vector<int> & v, int key) {
    int low = 0;
    int high = v.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (v[mid] == key) {
            return mid;
        } else if (v[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

//=============================================================================
// Exercise 2: Facing Sun
//=============================================================================
int FunWithSearch::facingSun(const vector<int> & values) {
    // Assuming the vector is not empty
    int max = values[0];
    int count = 1;

    for (int entry: values) {
        if (entry > max) {
            max = entry;
            count++;
        }
    }

    return count;
}


//=============================================================================
// Exercise 3: Square Root
//=============================================================================
int FunWithSearch::squareR(int num) {
    int low = 0;
    int high = num;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (mid * mid == num) {
            return mid;
        } else if (mid * mid < num) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return low - 1;
}


//=============================================================================
// Exercise 5: MinPages (extra)
//=============================================================================
// TODO
int FunWithSearch::minPages(const vector<int> & values, int numSt) {
    return 0;
}
