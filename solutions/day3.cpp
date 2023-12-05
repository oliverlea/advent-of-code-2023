#include "../include/utils.h"

#include <iostream>
#include <set>
#include <vector>

inline bool loc_is_symbol(std::vector<std::string>& grid, int row, int col) {
    char c = grid[row][col];
    return !isdigit(c) && c != '.';
}

inline bool loc_is_digit(std::vector<std::string>& grid, int row, int col) {
    char c = grid[row][col];
    return isdigit(c);
}

template<typename F>
std::pair<int, int> borders(std::vector<std::string>& grid, int row, int symbol_start_col, int symbol_end_col, F predicate) {
    int start_col = symbol_start_col - 1 >= 0 ? symbol_start_col - 1 : symbol_start_col;
    int end_col = symbol_end_col + 1 < grid[row].size() ? symbol_end_col + 1 : symbol_end_col;
    if (row - 1 >= 0) {
        for (int col = start_col; col <= end_col; ++col) {
            if (predicate(grid, row - 1, col)) return {row - 1, col};
        }
    }
    if (row + 1 < grid.size()) {
        for (int col = start_col; col <= end_col; ++col) {
            if (predicate(grid, row + 1, col)) return {row + 1, col};
        }
    }
    if (symbol_start_col - 1 >= 0) {
        if (predicate(grid, row, symbol_start_col - 1)) return {row, symbol_start_col - 1};
    }
    if (symbol_end_col + 1 < grid[row].size()) {
        if (predicate(grid, row, symbol_end_col + 1)) return {row, symbol_end_col + 1};
    }
    return {-1, -1};
}

int part1(std::vector<std::string>& grid) {
    int width = grid[0].size();
    int result = 0;
    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < width; ++col) {
            char cur = grid[row][col];
            if (!isdigit(cur)) continue;
            else {
                int digit_start = col;
                int digit_end = col++;
                while (col < width && isdigit(grid[row][col])) {
                    ++digit_end;
                    ++col;
                }
                // search for symbol bordering number
                if (borders(grid, row, digit_start, digit_end, loc_is_symbol).first != -1) {
                    std::string digit_str = grid[row].substr(digit_start, (digit_end - digit_start) + 1);
                    result += std::stoi(digit_str);
                }
            }
        }
    }
    return result;
}

std::pair<int, int> greedy_digit(std::vector<std::string>& grid, int row, int root) {
    // read left and then read right
    int moves = 0;
    while (root - moves - 1 >= 0 && isdigit(grid[row][root - moves - 1])) {
        ++moves;
    }
    int start_col = root - moves;
    moves = 0;
    while (root + moves + 1 < grid[row].size() && isdigit(grid[row][root + moves + 1])) {
        ++moves;
    }
    int end_col = root + moves;
    return {start_col, end_col};
}

int gear_ratio(std::vector<std::string>& grid, int row, int col) {
    std::set<std::pair<int, int>> border = { 
        {row - 1, col -1},
        {row - 1, col},
        {row -1, col + 1},
        {row, col - 1},
        {row, col + 1},
        {row + 1, col - 1},
        {row + 1, col},
        {row + 1, col + 1}
    };
    std::vector<std::pair<int, int>> removes;
    for (std::pair<int, int> coord : border) {
        int crow = coord.first;
        int ccol = coord.second;
        if (crow < 0 || crow >= grid.size() || ccol < 0 || ccol >= grid[0].size() || !loc_is_digit(grid, crow, ccol)) {
            removes.push_back(coord);
        }
    }
    for (auto coord : removes) {
        border.erase(coord);
    }

    std::vector<int> values;
    std::set<std::pair<int, int>> visited;
    for (std::pair<int, int> coord : border) {
        if (visited.find(coord) != visited.end()) {
            continue;
        }
        auto [val_start_col, val_end_col] = greedy_digit(grid, coord.first, coord.second);
        bool new_value = true;
        for (int cur_col = val_start_col; cur_col <= val_end_col; ++cur_col) {
            if (visited.find({coord.first, cur_col}) != visited.end()) {
                new_value = false;
                break;
            }
            visited.insert({coord.first, cur_col});
        }
        if (new_value) {
            std::string val_str = grid[coord.first].substr(val_start_col, (val_end_col - val_start_col) + 1);
            values.push_back(std::stoi(val_str));
        }
    }
    if (values.size() == 2) {
        return values[0] * values[1];
    }
    return 0;
}

long long part2(std::vector<std::string>& grid) {
    int width = grid[0].size();
    long long result = 0;
    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < width; ++col) {
            char c = grid[row][col];
            if (c == '*') {
                result += gear_ratio(grid, row, col);
            }
        }
    }
    return result;
}

int main() {
    std::vector<std::string> sample = read_lines("../input/sample3.txt");
    std::cout << part1(sample) << std::endl;

    std::vector<std::string> input = read_lines("../input/day3.txt");
    std::cout << part1(input) << std::endl;

    std::cout << part2(sample) << std::endl;
    std::cout << part2(input) << std::endl;
}