
#include "../include/timer.h"
#include "../include/utils.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <vector>

using namespace std;


enum Dir {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

class Grid {
public:
    int start_x, start_y;

    Grid(vector<string> input) : input(input) {
        for (int row = 0; row < input.size(); ++row) {
            for (int col = 0; col < input[0].size(); ++col) {
                if (at(col, row) == 'S') {
                    start_x = col;
                    start_y = row;
                    return;
                }
            }
        }
    }

    int get_width() const { 
        return input[0].size();
    }

    int get_height() const {
        return input.size();
    }

    char at(int x, int y) const {
        return input[y][x];
    }

    pair<int, int> go(int x, int y, Dir dir) const {
        int target_x = x, target_y = y;
        switch (dir) {
            case NORTH:
                target_y -= 1;
                break;
            case SOUTH:
                target_y += 1;
                break;
            case EAST:
                target_x += 1;
                break;
            case WEST:
                target_x -= 1;
                break;
        }
        return {target_x, target_y};
    }

    vector<pair<int, int>> valid_directions(int x, int y) const {
        vector<Dir> possible_dirs;
        char cur = at(x, y);
        switch (cur) {
            case '|':
                possible_dirs = {NORTH, SOUTH};
                break;
            case '-':
                possible_dirs = {EAST, WEST};
                break;
            case 'L':
                possible_dirs = {NORTH, EAST};
                break;
            case 'J':
                possible_dirs = {NORTH, WEST};
                break;
            case '7':
                possible_dirs = {WEST, SOUTH};
                break;
            case 'F':
                possible_dirs = {SOUTH, EAST};
                break;
            case '.':
                break;
            case 'S':
                possible_dirs = {NORTH, SOUTH, EAST, WEST};
                break;
        }

        vector<Dir> dirs;
        for (int i = 0; i < possible_dirs.size(); ++i) {
            Dir cur_dir = possible_dirs[i];
            if (is_connected_from(x, y, cur_dir)) {
                dirs.push_back(cur_dir);
            }
        }
        vector<pair<int, int>> coords(dirs.size());
        for (int i = 0; i < dirs.size(); ++i) {
            coords[i] = go(x, y, dirs[i]);
        }
        return coords;
    }

    void debug(int x, int y) const {
        cout << " " << input[y - 1][x] << endl;
        cout << input[y][x - 1] << input[y][x] << input[y][x + 1] << endl;
        cout << " " << input[y + 1][x] << endl;
    }

private:
    const vector<string> input;


    bool is_connected_from(int x, int y, Dir dir) const {
        auto [target_x, target_y] = go(x, y, dir);
        if (!within_bounds(target_x, target_y)) return {};
        char target = at(target_x, target_y);
        switch (dir) {
            case NORTH: return target == '|' || target == '7' || target == 'F' || target == 'S';
            case SOUTH: return target == '|' || target == 'L' || target == 'J' || target == 'S';
            case EAST:  return target == '-' || target == 'J' || target == '7' || target == 'S';
            case WEST:  return target == '-' || target == 'L' || target == 'F' || target == 'S';
        }
    }

    inline bool within_bounds(int x, int y) const {
        return x >= 0 && x < input.size() && y >= 0 && y < input[0].size();
    }
};

struct StackState {
    int cur_x, cur_y;
    int prev_x, prev_y;
    int steps;
};

vector<pair<int, int>> find_loop(const Grid& grid) {
    // dfs until we reach out starting point
    vector<pair<int, int>> result;
    stack<StackState> s;
    s.push({grid.start_x, grid.start_y, grid.start_x, grid.start_y, 0});
    while (true) {
        StackState state = s.top();
        s.pop();
        result.push_back({state.cur_x, state.cur_y});
        vector<pair<int, int>> possible_next = grid.valid_directions(state.cur_x, state.cur_y);
        pair<int, int> previous = {state.prev_x, state.prev_y};
        auto prev_index = std::find(possible_next.begin(), possible_next.end(), previous);
        if (prev_index != possible_next.end()) {
            possible_next.erase(prev_index);
        }
        if (possible_next.empty()) {
            grid.debug(state.cur_x, state.cur_y);
        } else if (possible_next.size() > 1 && grid.at(state.cur_x, state.cur_y) != 'S') {
            // if not at the starting point and we have multiple options, error
            cerr << "found multiple options in pipe" << endl;
            return {};
        }

        for (const auto& coord : possible_next) {
            if (grid.at(coord.first, coord.second) == 'S') {
                return result;
            }
            s.push({coord.first, coord.second, state.cur_x, state.cur_y, state.steps + 1});
        }
    }
}

std::ostream& operator<<(std::ostream& os, pair<int, int> const& r) {
    return os << "[" << r.first << ", " << r.second << "]" << endl;
}

inline bool within(int x, int y, int height, int width) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

vector<pair<int, int>> surrounds(int x, int y, int height, int width) {
    vector<pair<int, int>> results;
    pair<int, int> up = {x, y - 1};
    if (within(up.first, up.second, height, width))
        results.push_back(up);
    pair<int, int> down = {x, y + 1};
    if (within(down.first, down.second, height, width))
        results.push_back(down);
    pair<int, int> left = {x - 1, y};
    if (within(left.first, left.second, height, width))
        results.push_back(left);
    pair<int, int> right = {x + 1, y};
    if (within(right.first, right.second, height, width))
        results.push_back(right);
    return results;
}

set<pair<int, int>> outside_area(const int height, const int width, const vector<pair<int, int>>& loop_vec) {
    set<pair<int, int>> loop(loop_vec.begin(), loop_vec.end());

    set<pair<int, int>> outside;
    set<pair<int, int>> inside;
    // go around outside of grid and add any element not in the loop to the set of elements
    // outside of the loop
    for (int i = 0; i < width; ++i) {
        pair<int, int> cur = {i, 0};
        if (loop.find(cur) == loop.end()) {
            outside.insert(cur);
        }
        cur = {i, height - 1};
        if (loop.find(cur) == loop.end()) {
            outside.insert(cur);
        }
    }
    for (int i = 1; i < height - 1; ++i) {
        pair<int, int> cur = {0, i};
        if (loop.find(cur) == loop.end()) {
            outside.insert(cur);
        }
        cur = {width - 1, i};
        if (loop.find(cur) == loop.end()) {
            outside.insert(cur);
        }
    }

    // BFS
    set<pair<int, int>> all_outside(outside.begin(), outside.end());
    for (const auto& coord : outside) {
        queue<pair<int, int>> q;
        q.push(coord);
        set<pair<int, int>> encountered;
        set<pair<int, int>> skip;
        encountered.insert(coord);
        int i = 0;
        while (!q.empty()) {
            pair<int, int> next = q.front();
            q.pop();

            vector<pair<int, int>> surrounded = surrounds(next.first, next.second, height, width);
            for (const auto& c : surrounded) {
                if (all_outside.find(c) == all_outside.end() && encountered.find(c) == encountered.end() && loop.find(c) == loop.end()) {
                    encountered.insert(c);
                    q.push(c);
                }
            }
        }
        all_outside.insert(encountered.begin(), encountered.end());
    }
    return all_outside;
}

void print_grid(const vector<pair<int, int>>& loop, int height, int width) {
    set<pair<int, int>> loop_set(loop.begin(), loop.end());
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            bool looped = loop_set.find({col, row}) != loop_set.end();
            cout << (looped ? "O" : ".");
        }
        cout << endl;
    }
}

int part1(const Grid& grid) {
    Timer timer;
    int loop_size = find_loop(grid).size();
    return std::ceil(loop_size / static_cast<float>(2));
}

int part2(const Grid& grid) {
    Timer timer;
    vector<pair<int, int>> loop = find_loop(grid);

    // double the size of the grid to account for pipes squeezing
    vector<pair<int, int>> loop_double;
    for (int i = 0; i < grid.get_height() * 2; ++i) {
        vector<bool> row(grid.get_width() * 2, false);
    }

    loop_double.push_back({loop[0].first * 2, loop[0].second * 2});
    for (int i = 1; i < loop.size() + 1; ++i) {
        const auto& cur = i == loop.size() ? loop[0] : loop[i];
        const auto& prev = i == loop.size() ? loop[loop.size() - 1] : loop[i - 1];
        const pair<int, int> cur_double = {cur.first * 2, cur.second * 2};
        // fill in gap from previous section of loop
        if (prev.first < cur.first) {
            // a move from left to right
            loop_double.push_back({cur_double.first - 1, cur_double.second});
        } else if (prev.first > cur.first) {
            // right -> left
            loop_double.push_back({cur_double.first + 1, cur_double.second});
        } else if (prev.second < cur.second) {
            // up -> down
            loop_double.push_back({cur_double.first, cur_double.second - 1});
        } else {
            // down -> up
            loop_double.push_back({cur_double.first, cur_double.second + 1});
        }
        loop_double.push_back({cur_double});
    }
    // print_grid(loop, grid.get_height(), grid.get_width());
    print_grid(loop_double, grid.get_height() * 2, grid.get_width() * 2);

    // search from the outside in to find the area outside of the grid
    set<pair<int, int>> outside = outside_area(grid.get_height() * 2, grid.get_width() * 2, loop_double);
    // keep only those that would exist at the original grid coordinates that are not
    // part of the loop and not on the outside
    set<pair<int, int>> loop_double_set(loop_double.begin(), loop_double.end());
    set<pair<int, int>> inside;
    for (int row = 0; row < grid.get_height() * 2; row += 2) {
        for (int col = 0; col < grid.get_width() * 2; col += 2) {
            const pair<int, int> coord = {col, row};
            if (outside.find(coord) == outside.end() && loop_double_set.find(coord) == loop_double_set.end()) {
                inside.insert(coord);
            }
        }
    }
    // hope for the best
    return inside.size();
}

int main() {
    vector<string> lines = read_lines("../input/day10.txt");
    Grid grid{lines};

    cout << part1(grid) << endl;
    cout << part2(grid) << endl;
}