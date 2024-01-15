#include "jackpot.h"


Bet::Bet(vector<int> ns, string p): numbers(ns), player(p) {}

vector<int> Bet::getNumbers() const {
    return numbers;
}

string Bet::getPlayer() const {
    return player;
}

Jackpot::Jackpot()  {}

unsigned Jackpot::getNumBets() const {
    return bets.size();
}


//=============================================================================
// Exercise 2: Jackpot
//=============================================================================
// Subexercise 2.1: Add Bet
//=============================================================================
// TODO
void Jackpot::addBet(const Bet& b) {
    if (bets.find(b) == bets.end())
        bets.insert(b);
}

//=============================================================================
// Subexercise 2.2: Bets in Number
//=============================================================================
unsigned Jackpot::betsInNumber(unsigned num) const {
    unsigned res = 0;
	for (const auto & bet : bets) {
        vector<int> numbers = bet.getNumbers();
        if (find(numbers.begin(), numbers.end(), num) != numbers.end())
            res++;
    }
    return res;
}

//=============================================================================
// Subexercise 2.3: Drawn Bets
//=============================================================================
vector<string> Jackpot::drawnBets(vector<int> draw) const {
    vector<string> res;
    for (const auto & bet : bets) {
        vector<int> numbers = bet.getNumbers();
        string player = bet.getPlayer();
        unsigned count = 0;
        for (const auto & num : draw) {
            if (find(numbers.begin(), numbers.end(), num) != numbers.end()) {
                count++;
            }
        }
        if (count >= 3)
            res.push_back(player);
    }
    return res;
}
