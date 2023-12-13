#include "../include/timer.h"
#include "../include/utils.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef vector<vector<bool>> grid;

vector<grid> parse(const vector<string>& lines) {
    vector<grid> grids;
    grid cur_grid;
    for (const string& line : lines) {
        if (line.length() <= 1) {
            grids.push_back(cur_grid);
            grid n;
            cur_grid = n;
        } else {
            istringstream l(line);
            string spacefree;
            l >> spacefree;
            vector<bool> row(spacefree.size());
            for (int i = 0; i < spacefree.size(); ++i) {
                row[i] = spacefree[i] == '#';
            }
            cur_grid.push_back(row);
        }
    }
    if (!cur_grid.empty()) grids.push_back(cur_grid);
    return grids;
}

inline int row_distance(const grid& g, int x, int y) {
    bool smudged = false;
    for (int col = 0; col < g[0].size(); ++col) {
        if (g[x][col] != g[y][col]) {
            if (smudged) return 2;
            else {
                smudged = true;
            }
        }
    }
    return smudged ? 1 : 0;
}

int reflects_hor(const grid& g, bool require_smudge) {
    int pi = 0;
    for (int i = 1; i < g.size(); ++i) {
        int ci = i;
        bool smudged = !require_smudge;
        int dist = 0;
        while ((dist = row_distance(g, pi, ci)) <= ((require_smudge && !smudged) ? 1 : 0)) {
            if (dist > 0) smudged = true;
            --pi;
            ++ci;
            if (pi < 0 || ci >= g.size()) {
                if (require_smudge && smudged) return i;
                if (!require_smudge) return i;
                break;
            }
        } 
        pi = i;
    }
    return -1;
}

inline int col_distance(const grid& g, int x, int y) {
    bool smudged = false;
    for (int row = 0; row < g.size(); ++row) {
        if (g[row][x] != g[row][y]) {
            if (smudged) return 2;
            else {
                smudged = true;
            }
        }
    }
    return smudged ? 1 : 0;
}

int reflects_ver(const grid& g, bool require_smudge) {
    int pi = 0;
    for (int i = 1; i < g[0].size(); ++i) {
        int ci = i;
        bool smudged = !require_smudge;
        int dist = 0;
        while ((dist = col_distance(g, pi, ci)) <= ((require_smudge && !smudged) ? 1 : 0)) {
            if (dist > 0) smudged = true;
            --pi;
            ++ci;
            if (pi < 0 || ci >= g[0].size()) {
                if (require_smudge && smudged) return i;
                if (!require_smudge) return i;
                break;
            }
        } 
        pi = i;
    }
    return -1;
}

long solve(const vector<grid>& grids, bool require_smudge) {
    Timer timer;
    long result = 0;
    for (const grid& g : grids) {
        int hor_index = reflects_hor(g, require_smudge);
        if (hor_index != -1) {
            result += hor_index * 100;
        } else {
            int col_index = reflects_ver(g, require_smudge);
            if (col_index != -1) {
                result += col_index;
            }
        }
    }
    return result;
}

int main() {
    vector<string> lines = read_lines("../input/day13.txt");
    vector<grid> grids = parse(lines);
    cout << solve(grids, false) << endl;
    cout << solve(grids, true) << endl;
}