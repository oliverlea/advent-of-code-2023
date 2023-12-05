#include "../include/utils.h"

#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct GridNumber {
    int value;
    vector<pair<int, int>> coords;

    GridNumber(int value, vector<pair<int, int>> coords) : value(value), coords(coords) {}
};

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }
        return hash1;
    }
};

vector<unique_ptr<GridNumber>> find_numbers(vector<string>& grid) {
    int width = grid[0].size();
    vector<unique_ptr<GridNumber>> results;
    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < width; ++col) {
            char cur = grid[row][col];
            if (isdigit(cur)) {
                // greedy take
                int start = col;
                int end = start;
                while (end < width && isdigit(grid[row][end])) {
                    ++end;
                }
                string num_str = grid[row].substr(start, end - start);
                vector<pair<int, int>> coords(end - start);
                for (int i = start; i < end; ++i) {
                    coords[i - start] = {row, i};
                }
                auto ptr = make_unique<GridNumber>(stoi(num_str), coords);
                results.push_back(std::move(ptr));
                // move for-pointer
                col = end;
            }
        }
    }
    return results;
}

vector<pair<int, int>> adjacent(vector<string>& grid, int row, int col) {
    set<pair<int, int>> adj = {
        {row - 1, col - 1},
        {row - 1, col},
        {row - 1, col + 1},
        {row, col - 1},
        {row, col + 1},
        {row + 1, col - 1},
        {row + 1, col},
        {row + 1, col + 1}
    };
    vector<pair<int, int>> remove;
    for (auto coord : adj) {
        int row = coord.first;
        int col = coord.second;
        if (row < 0 || col < 0 || row >= grid.size() || col >= grid[0].size()) {
            remove.push_back(coord);
        }
    }
    for (auto coord : remove) {
        adj.erase(coord);
    }
    return vector<pair<int, int>>(adj.begin(), adj.end());
}

pair<int, int> solve(vector<string>& grid) {
    vector<unique_ptr<GridNumber>> grid_nums = find_numbers(grid);
    unordered_map<pair<int, int>, shared_ptr<GridNumber>, hash_pair> coord_lookup;
    for (auto& ptr : grid_nums) {
        shared_ptr<GridNumber> shared = std::move(ptr);
        for (auto& coord : shared->coords) {
            coord_lookup[coord] = shared;
        }
    }

    int width = grid[0].size();
    int part2 = 0;
    set<shared_ptr<GridNumber>> attached;
    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < width; ++col) {
            char cur = grid[row][col];
            if (cur == '.' || isdigit(cur)) continue;
            vector<pair<int, int>> adj = adjacent(grid, row, col);
            set<shared_ptr<GridNumber>> gear_attached;
            for (auto& coord : adj) {
                if (coord_lookup.find(coord) != coord_lookup.end()) {
                    attached.insert(coord_lookup[coord]);
                    gear_attached.insert(coord_lookup[coord]);
                }
            }
            if (cur == '*' && gear_attached.size() == 2) {
                int mult = 1;
                for (auto gn_ptr : gear_attached) {
                    mult *= gn_ptr->value;
                }
                part2 += mult;
            }
        }
    }
    int part1 = 0;
    for (auto gn_ptr : attached) {
        part1 += gn_ptr->value;
    }
    return {part1, part2};
}


int main() {
    vector<string> sample = read_lines("../input/sample3.txt");
    auto [sample_p1, sample_p2] = solve(sample);
    cout << "p1: " << sample_p1 << " -> " <<  "p2: " << sample_p2 << endl;

    vector<string> input = read_lines("../input/day3.txt");
    auto [input_p1, input_p2] = solve(input);
    cout << "p1: " << input_p1 << " -> " <<  "p2: " << input_p2 << endl;
}