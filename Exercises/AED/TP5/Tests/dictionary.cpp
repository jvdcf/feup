#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "dictionary.h"

using namespace std;

WordMean::WordMean(string w, string m): word(w), meaning(m) {}

string WordMean::getWord() const {
    return word;
}

string WordMean::getMeaning() const {
    return meaning;
}

void WordMean::setMeaning(string m) {
    meaning = m;
}

void WordMean::setWord(string w) {
    word = w;
}

set<WordMean> Dictionary::getWords() const {
	return words;
}

void Dictionary::addWord(WordMean wm)  {
    words.insert(wm);
}


//=============================================================================
// Exercise 1: Dictionary
//=============================================================================
bool WordMean::operator< (const WordMean& wm2) const {
     return this->word < wm2.word;
}

//=============================================================================
// Subexercise 1.1: Create Dictionary
//=============================================================================
void Dictionary::readFile(ifstream &f) {
    string word;
    string meaning;
    while (getline(f, word)) {
        getline(f, meaning);
        WordMean wm = WordMean(word, meaning);
        addWord(wm);
    }
}

//=============================================================================
// Subexercise 1.2: Print Dictionary
//=============================================================================
void Dictionary::print() const {
    for (const WordMean& wm: words) {
        cout << wm.getWord() << '\n';
        cout << wm.getMeaning() << '\n';
    }
}

//=============================================================================
// Subexercise 1.3: Consult Dictionary
//=============================================================================
string Dictionary::consult(string w1, WordMean& previous, WordMean& next) const {
    auto itPrevious = words.end();
    auto itNext = words.end();
    string meaning;

    for (auto it = words.begin(); it != words.end(); it++) {
        if (w1 == it->getWord()) {
            meaning = it->getMeaning();

            it++;
            itNext = it;
            it--;
            it--;
            itPrevious = it;
            break;
        }
        // Not Found
        if (w1 < it->getWord()) {
            meaning = "word not found";

            itNext = it;
            it--;
            itPrevious = it;
            break;
        }
    }

    if (itPrevious != words.end()) previous = *itPrevious;
    else previous = WordMean("", "");

    if (itNext != words.end()) next = *itNext;
    else next = WordMean("", "");

    return meaning;

}

//=============================================================================
// Subexercise 1.4: Update Dictionary
//=============================================================================
bool Dictionary::update(string w1, string m1) {
    WordMean newNode(w1, m1);
    bool returnValue = false;

    for (auto it = words.begin(); it != words.end(); it++) {
        if (w1 == it->getWord()) {
            it = words.erase(it);
            returnValue = true;
            break;
        }
        if (w1 < it->getWord()) break;
    }

    words.insert(newNode);
    return returnValue;

}
