
#include "../include/timer.h"
#include "../include/utils.h"

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

const map<char, int> ranking {
    {'*', -1},
    {'2', 0},
    {'3', 1},
    {'4', 2},
    {'5', 3},
    {'6', 4},
    {'7', 5},
    {'8', 6},
    {'9', 7},
    {'T', 8},
    {'J', 9},
    {'Q', 10},
    {'K', 11},
    {'A', 12},
};

struct Hand {
    string cards;
    int bid;
    int strength;
};

int cal_strength(const string& cards) {
    unordered_map<char, int> card_freqs;
    for (char c : cards) {
        ++card_freqs[c];
    }

    int jokers = card_freqs['*'];
    card_freqs.erase('*');

    vector<int> freqs(card_freqs.size());
    auto iter = card_freqs.begin();
    for (int i = 0; i < card_freqs.size(); ++i) {
        freqs[i] = (iter++)->second;
    }
    sort(freqs.begin(), freqs.end());

    int considered = 0;
    int top = jokers;
    if (!freqs.empty()) {
        top += freqs.back();
        freqs.pop_back();
    }
    int next = freqs.empty() ? 0 : freqs.back();

    if (top == 5) return 6;
    else if (top == 4) return 5;
    else if (top == 3) {
        if (next == 2) return 4;
        else return 3;
    } else if (top == 2) {
        if (next == 2) return 2;
        else return 1;
    } else return 0;
}

bool compare_hand(const Hand& h1, const Hand& h2) {
    if (h1.strength != h2.strength) 
        return h1.strength < h2.strength;
    else {
        for (int i = 0; i < h1.cards.size(); ++i) {
            char l, r;
            int l_rank = ranking.at(h1.cards[i]);
            int r_rank = ranking.at(h2.cards[i]);
            if (l_rank != r_rank) return l_rank < r_rank;
        }
        return false;
    }
}

long solve(const vector<string>& input, bool use_jokers) {
    Timer timer;
    vector<Hand> hands(input.size());
    for (int i = 0; i < input.size(); ++i) {
        string line = input[i];
        istringstream ss(line);
        string hand;
        int bid;
        ss >> hand >> bid;
        if (use_jokers) {
            for (auto& c : hand) {
                if (c == 'J') c = '*';
            }
        }
        int strength = cal_strength(hand);
        hands[i] = {hand, bid, strength};
    }
    sort(hands.begin(), hands.end(), compare_hand);
    long result = 0;
    for (int i = 0; i < hands.size(); ++i) {
        result += hands[i].bid * (i + 1);
    }
    return result;
}

int main() {
    vector<string> lines = read_lines("../input/day7.txt");
    cout << solve(lines, false) << endl;
    cout << solve(lines, true) << endl;
}