#include "../include/timer.h"
#include "../include/utils.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

long next_in_seq(vector<long> seq) {
    if (count(seq.begin(), seq.end(), 0) == seq.size()) {
        return 0;
    } else {
        vector<long> diffs(seq.size() - 1);
        for (int i = 1; i < seq.size(); ++i) {
            diffs[i - 1] = seq[i] - seq[i - 1];
        }
        long next_in_diffs = next_in_seq(diffs);
        return seq.back() + next_in_diffs;
    }
}

long part1(const vector<vector<long>>& sequences) {
    Timer timer;
    long result = 0;
    for (const auto& seq : sequences) {
        result += next_in_seq(seq);
    }
    return result;
}

long part2(vector<vector<long>>& sequences) {
    Timer timer;
    long result = 0;
    for (auto& seq : sequences) {
        std::reverse(seq.begin(), seq.end());
        result += next_in_seq(seq);
    }
    return result;
}

int main() {
    vector<string> lines = read_lines("../input/day9.txt");
    vector<vector<long>> sequences;
    for (const string& line : lines) {
        istringstream ss(line);
        vector<long> line_results;
        long x;
        while (ss >> x) {
            line_results.push_back(x);
        }
        sequences.push_back(line_results);
    }

    cout << part1(sequences) << endl;
    cout << part2(sequences) << endl;
}
