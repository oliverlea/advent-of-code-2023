#include "../include/utils.h"
#include "../include/timer.h"

#include <set>
#include <string>
#include <vector>

using namespace std;

typedef pair<long, long> loc;

vector<loc> galaxy_locations(const vector<string>& input, int expansion, int height, int width) {
    vector<loc> galaxies;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (input[row][col] == '#') {
                galaxies.push_back({row, col});
            }
        }
    }
    // inserted expanded space row-wise and then column-wise
    vector<long> row_offset(height, 0);
    for (int row = 0; row < height; ++row) {
        const string& line = input[row];
        const long prev_offset = row == 0 ? 0 : row_offset[row - 1];
        if (std::find(line.begin(), line.end(), '#') == line.end()) {
            row_offset[row] = prev_offset + (expansion - 1);
        } else {
            row_offset[row] = prev_offset;
        }
    }
    vector<long> col_offset(height, 0);
    for (int col = 0; col < width; ++col) {
        bool empty = true;
        for (int row = 0; row < height; ++row) {
            if (input[row][col] == '#') {
                empty = false;
                break;
            }
        }
        const long prev_offset = col == 0 ? 0 : col_offset[col - 1];
        if (empty) {
            col_offset[col] = prev_offset + (expansion - 1);
        } else {
            col_offset[col] = prev_offset;
        }
    }

    vector<loc> expanded_galaxies(galaxies.size());
    for (int i = 0; i < galaxies.size(); ++i) {
        loc cur = galaxies[i];
        expanded_galaxies[i] = {cur.first + row_offset[cur.first], + cur.second + col_offset[cur.second]};
    }
    return expanded_galaxies;
}

long long galaxy_distances(const vector<string>& input, int expansion, int height, int width) {
    Timer timer;
    vector<loc> galaxies = galaxy_locations(input, expansion, height, width);
    long long distances = 0;
    for (int i = 0; i < galaxies.size(); ++i) {
        const loc& galaxy = galaxies[i];
        for (int j = i + 1; j < galaxies.size(); ++j) {
            const loc& galaxy2 = galaxies[j];
            if (&galaxy == &galaxy2) continue;
            long long gap = abs(galaxy.first - galaxy2.first) + abs(galaxy.second - galaxy2.second);
            distances += gap;
        }
    }
    return distances;
}

int main() {
    vector<string> lines = read_lines("../input/day11.txt");

    int grid_height = lines.size();
    int grid_width = lines[0].size();

    cout << galaxy_distances(lines, 2, grid_height, grid_width) << endl;
    cout << galaxy_distances(lines, 1000000, grid_height, grid_width) << endl;
}