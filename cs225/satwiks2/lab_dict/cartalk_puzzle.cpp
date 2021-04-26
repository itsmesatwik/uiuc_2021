/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>
#include <iostream>
#include <algorithm>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                    const string& word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;
    ifstream words(word_list_fname);
    string s;
    if (words.is_open()) {
        while (getline(words, s)) {
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
            string a = s;
            string b = s;
            a.erase(0, 1);
            b.erase(1, 1);
            if (d.homophones(s,a) && d.homophones(s,b)) {
                std::transform(s.begin(), s.end(), s.begin(), ::tolower);
                std::transform(a.begin(), a.end(), a.begin(), ::tolower);
                std::transform(b.begin(), b.end(), b.begin(), ::tolower);
                ret.emplace_back(s, a, b);
            }
        }
    }

    /* Your code goes here! */
    return ret;
}
