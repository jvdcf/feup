#include "funWithCycles.h"
using namespace std;

//=============================================================================
// Exercise 1: Palindromes
//=============================================================================
bool FunWithCycles::palindrome(const std::string & s) {
    if (s.length() <= 1) return true;
    if (s.at(0) == s.at(s.length() - 1)) {
        string trimmed;
        for (int i = 1; i < s.length() - 1; i++) {
            trimmed += s.at(i);
        }
        return palindrome(trimmed);
    } else return false;
}


//=============================================================================
// Exercise 2: Palindrome Sentences
//=============================================================================
#include <ctype.h>

bool FunWithCycles::palindromeSentence(const std::string & s) {
    string lowercase;
    for (char c: s) if (isalpha(c)) lowercase += tolower(c);
    return palindrome(lowercase);
}


//=============================================================================
// Exercise 3: Interesting Numbers
//=============================================================================
int FunWithCycles::nextInterestingNumber(int n, int sum) {
    int n_sum;
    do {
        n++;
        n_sum = digitSum(n);
    } while (sum != n_sum);
    return n;
}

// This auxiliary function should return the sum of the digits of n
int FunWithCycles::digitSum(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}


//=============================================================================
// Exercise 4: Winter is coming
//=============================================================================
int FunWithCycles::winter(const vector<int> & v) {
    // Create a vector of differences
    int before = v[0];
    vector<int> differences;
    for (int i = 1; i < v.size(); i++) {
        differences.push_back(v[i] - before);
        before = v[i];
    }

    // Determine the longest sequence
    int count = 0;
    int max = 0;
    for (int diff: differences) {
        if (diff < 0) count++;
        else {
            if (count > max) max = count;
            count = 0;
        }
    }
    if (count > max) max = count;

    return max;
}


//=============================================================================
// Exercise 5: Playing with vectors
//=============================================================================

// a) count
int FunWithCycles::count(const vector<int> & v, int n) {
    int occurrences = 0;
    for (int number: v) {
        if (number == n) occurrences++;
    }
    return occurrences;
}

// b) hasDuplicates
#include <algorithm>
bool FunWithCycles::hasDuplicates(const vector<int> & v) {
    vector<int> orderedV = v;
    sort(orderedV.begin(), orderedV.end());

    int before = orderedV[0];
    for (int i = 1; i < orderedV.size(); i++) {
        if (before == orderedV[i]) return true;
        before = orderedV[i];
    }

    return false;
}

// c) removeDuplicates
void FunWithCycles::removeDuplicates(vector<int> & v) {
    vector<int> uniques;
    for (int integer: v) {
        if (std::find(uniques.begin(), uniques.end(), integer) == uniques.end()) {
            uniques.push_back(integer);
        }
    }
    v = uniques;
}

// d) merge
vector<int> FunWithCycles::merge(const vector<int> & v1, const vector<int> & v2) {
    // Inefficient version
    vector<int> ans;
    for (int i: v1) ans.push_back(i);
    for (int i: v2) ans.push_back(i);
    sort(ans.begin(), ans.end());
    return ans;
}


//=============================================================================
// Exercise 6: Playing with prime numbers
//=============================================================================

// a) isPrime
bool FunWithCycles::isPrime(int n) {
    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// b) factorize
vector<int> FunWithCycles::factorize(int n) {
    vector<int> ans;
    int factor = 2;
    while (n > 1) {
        if (n % factor == 0) {
            ans.push_back(factor);
            n /= factor;
        } else {
            factor++;
        }
    }

    return ans;
}

// c) listPrimes
#include <vector>
vector<int> FunWithCycles::listPrimes(int n) {
    // 1. Create a list from 2 through n
    vector<int> ans;
    for (int i = 2; i <= n; i++) ans.push_back(i);

    // 2. p as the smallest prime number
    int p = 0;  // p is an index, not a number

    while (p < ans.size() - 1) {
        // 3. Remove the multiples of p
        for (int i = 0; i < ans.size(); i++) {
            if ((ans[i] % ans[p] == 0) && (ans[i] != ans[p])) {
                ans.erase(remove(ans.begin(), ans.end(), ans[i]), ans.end());
                // Remove from the vector
            }
        }

        // 4. Find the next number not marked
        p++;
    }

    return ans;
}
