#include "funWithComplexity.h"

//=============================================================================
// Exercise 4: Analyzing a river
//=============================================================================
int FunWithComplexity::river(const vector<int> & v, int k, int t) {
    int regionsCount = 0;
    int lb = 0;
    int ub = k - 1;

    for (int i = 0; i < v.size() - k + 1; i++) {
        int locationsCount = 0;
        for (int j = lb + i; j <= ub + i; j++) {
            if (v[j] >= t) locationsCount++;
        }
        if (locationsCount * 2 >= k) regionsCount++;
    }

    return regionsCount;
}

//=============================================================================
// Exercise 5: Spiral of Numbers
//=============================================================================
#include "cmath"
pair<int, int> FunWithComplexity::spiral(int n) {
    if (n == 1) return {0, 0};

    pair<int, int> ans = {0, 0};
    int number = 1;
    int nearest_squared = 1;

    // Skipping layers
    while (n >= pow(nearest_squared + 2, 2)) {
        nearest_squared += 2;
        number = (int) pow(nearest_squared, 2);
        ans.first++;
        ans.second++;
    }

    if (n == number) return ans;

    // Going 1 right
    ans.first++;
    number++;

    // Going down
    int travel_allowed = nearest_squared;
    if (number + travel_allowed < n) {
        ans.second -= travel_allowed;
        number += travel_allowed;
    } else {
        ans.second -= n - number;
        return ans;
    }

    // Going left
    travel_allowed++;
    if (number + travel_allowed < n) {
        ans.first -= travel_allowed;
        number += travel_allowed;
    } else {
        ans.first -= n - number;
        return ans;
    }

    // Going up
    if (number + travel_allowed < n) {
        ans.second += travel_allowed;
        number += travel_allowed;
    } else {
        ans.second += n - number;
        return ans;
    }

    // Going right (guaranteed)
    ans.first += n - number;
    return ans;
}


//=============================================================================
// Exercise 7: Squared notebook
//=============================================================================
// TODO
long long FunWithComplexity::gridSum(int a, int b) {
    return 0;
}