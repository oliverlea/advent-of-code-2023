#include "../include/utils.h"

#include <istream>
#include <iostream>
#include <numeric>
#include <string>
#include <set>
#include <vector>

using namespace std;

void part2(vector<string> input) {
    vector<int> wins(input.size(), 0);
    for (int i = 0; i < input.size(); ++i) {
        string line = input[i];
        istringstream ss{line};

        string prefix;
        ss >> prefix >> prefix;

        string winning;
        getline(ss, winning, '|');
        istringstream winning_ss{winning};
        set<int> winning_set;
        int winning_number;
        while (winning_ss >> winning_number) {
            winning_set.insert(winning_number);
        }

        string have;
        int have_number;
        int winning_exp = -1;
        while (ss >> have_number) {
            if (winning_set.find(have_number) != winning_set.end()) {
                ++wins[i];
            }
        }
    }
    vector<int> counts(input.size(), 1);
    for (int i = 0; i < input.size(); ++i) {
        int card_wins = wins[i];
        int card_count = counts[i];
        for (int w = 1; w <= card_wins; ++w) {
            counts[i + w] += card_count;
        }
    }
    int result = accumulate(counts.begin(), counts.end(), 0);
    cout << result << endl;
}

void part1(vector<string>& input) {
    int winning_points = 0;
    for (string line : input) {
        string prefix;
        istringstream ss{line};
        ss >> prefix >> prefix;

        string winning;
        getline(ss, winning, '|');
        istringstream winning_ss{winning};
        set<int> winning_set;
        int winning_number;
        while (winning_ss >> winning_number) {
            winning_set.insert(winning_number);
        }

        string have;
        int have_number;
        int winning_exp = -1;
        while (ss >> have_number) {
            if (winning_set.find(have_number) != winning_set.end()) {
                ++winning_exp;
            }
        }
        if (winning_exp != -1) winning_points += pow(2, winning_exp);
    }
    cout << winning_points << endl;
}

int main() {
    vector<string> input = read_lines("../input/day4.txt");

    part1(input);

    part2(input);
}