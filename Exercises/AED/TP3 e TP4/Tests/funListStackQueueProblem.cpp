#include "funListStackQueueProblem.h"
#include "cell.h"

FunListStackQueueProblem::FunListStackQueueProblem() {}


//=============================================================================
// Exercise 1: Remove Higher
//=============================================================================
list<int> FunListStackQueueProblem::removeHigher(list<int> &values, int x) {
    list<int> l1;
    for (auto it = values.begin(); it != values.end();) {
        if (*it > x) {
            l1.push_back(*it);
            it = values.erase(it);
        } else it++;
    }

    return l1;
}


//=============================================================================
// Exercise 2: Overlapping Intervals
//=============================================================================
bool compare(pair<int, int> a, pair<int, int> b) {
    if (a.first == b.first) {
        return a.second < b.second;
    }
    return a.first < b.first;
}

list<pair<int,int>> FunListStackQueueProblem::overlappingIntervals(list<pair<int,int>> values) {
    list<pair<int,int>> res;
    values.sort(compare);
    res.push_back(values.front());
    int lastSecond = values.front().second;

    for (pair<int, int>& v: values) {
        if (v.first <= lastSecond) {
            if (v.second > res.back().second) res.back().second = v.second;
        } else {
            res.push_back(v);
        }
        lastSecond = v.second;
    }
    return res;
}


//=============================================================================
// Exercise 4: Binary Numbers
//=============================================================================
vector<string> FunListStackQueueProblem::binaryNumbers(int n) {
    // Note: It already exists a function to create a binary representation of a number:
    // #include <bitset>
    // std::bitset<32> binary(number);

    if (n == 0) return {};
    vector<string> res;
    queue<string> queue;
    queue.push("1");

    for (int i = 0; i < n; i++) {
        string last = queue.front();
        queue.pop();
        res.push_back(last);

        string lastingZero = last + "0";
        string lastingOne = last + "1";
        queue.push(lastingZero);
        queue.push(lastingOne);
    }

    return res;
}


//=============================================================================
// Exercise 5: Calculate Span
//=============================================================================
#include <limits.h>
vector<int> FunListStackQueueProblem::calculateSpan(vector<int> prices) {
    vector<int> res;
    stack<pair<int, int>> closestDay;   // Pair (value, numberOfDays)
    closestDay.push({INT_MAX, 0});      // Bottom of the stack (unremovable)

    for (int price: prices) {
        int days = 1;

        while (price > closestDay.top().first) {
            days += closestDay.top().second;
            closestDay.pop();
        }

        closestDay.push({price, days});
        res.push_back(days);
    }

    return res;
}


//=============================================================================
// Exercise 7: Knight Jumps
//=============================================================================
//TODO
int FunListStackQueueProblem::knightJumps(int initialPosx, int initialPosy, int targetPosx, int targetPosy, int n) {
    return -1;
}
