#include "../include/timer.h"
#include "../include/utils.h"

#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>

using namespace std;

typedef unordered_map<string, vector<string>> adjlist;

const string start_node = "AAA";
const string end_node = "ZZZ";

struct Input {
    string instructions;
    adjlist adj;
};

Input parse(vector<string> lines) {
    string moves = lines[0];

    adjlist adj;
    for (int i = 2; i < lines.size(); ++i) {
        const string& line = lines[i];
        istringstream line_ss(line);

        string source;
        line_ss >> source;

        string tmp;
        getline(line_ss, tmp, '(');
        getline(line_ss, tmp, ')');

        string l1 = tmp.substr(0, 3);
        string l2 = tmp.substr(5, 3);
        adj[source] = {l1, l2};
    }
    return {moves, adj};
}

long part1(adjlist adj, string directions) {
    Timer timer;
    string cur = start_node;
    int direction_index = 0;
    long moves = 0;
    while (cur != end_node) {
        cur = directions[direction_index] == 'L' ? adj[cur][0] : adj[cur][1];
        direction_index = (direction_index + 1) % directions.size();
        ++moves;
    }
    return moves;
}

long long part2(adjlist adj, string instructions) {
    Timer timer;
    vector<string> starts;
    for (const auto& it : adj) {
        if (it.first.back() == 'A') {
            starts.push_back(it.first);
        }
    }
    vector<int> cycle_sizes(starts.size());
    for (int i = 0; i < starts.size(); ++i) {
        const string* cur = &starts[i];
        int count = 0;
        int instruction_i = 0;
        while (cur->back() != 'Z') {
            ++count;
            cur = &(instructions[instruction_i] == 'L' ? adj[*cur][0] : adj[*cur][1]);
            instruction_i = (instruction_i + 1) % instructions.size();
        }
        cycle_sizes[i] = count;
    }
    long long mul = cycle_sizes[0];
    for (int i = 1; i < cycle_sizes.size(); ++i) {
        mul = std::lcm<long long, long long>(mul, cycle_sizes[i]);
    }
    return mul;
}

int main() {
    vector<string> lines = read_lines("../input/day8.txt");
    Input input = parse(lines);
    cout << part1(input.adj, input.instructions) << endl;
    cout << part2(input.adj, input.instructions) << endl;
}