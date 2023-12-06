#include "../include/utils.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Race {
    long time;
    long distance;
};

vector<Race> parse(vector<string> lines) {
    vector<int> times;
    vector<int> distances;
    vector<int>* cv = &times;
    for (auto& line : lines) {
        istringstream ss{line};
        string tmp;
        getline(ss, tmp, ':');
        int cur;
        while (ss >> cur) {
            cv->push_back(cur);
        }
        cv = &distances;
    }
    vector<Race> races(times.size());
    for (int i = 0; i < times.size(); ++i) {
        races[i] = {times[i], distances[i]};
    }
    return races;
}

pair<double, double> quadratic(long a, long b, long c) {
    double root = sqrt(pow(b, 2) - (4 * a * c));
    double denom = 2 * static_cast<double>(a);

    double b1 = (-b + root) / denom;
    double b2 = (-b - root) / denom;
    return {min(b1, b2), max(b1, b2)};
}

long ways_to_win(long time, long distance) {
    auto [lower, upper] = quadratic(-1, time, -distance);

    long lower_int, upper_int;
    if (lower == trunc(lower)) lower_int = static_cast<long>(lower) + 1;
    else lower_int = static_cast<long>(ceil(lower));
    if (upper == trunc(upper)) upper_int = static_cast<long>(upper) - 1;
    else upper_int = static_cast<long>(floor(upper));

    return (upper_int - lower_int) + 1;
}

long part12(const vector<Race>& races) {
    long result = 1;
    for (const Race& race : races) {
        result *= ways_to_win(race.time, race.distance);
    }
    return result;
}

long part1(const vector<Race>& races) {
    long result = 1;
    for (const Race& race : races) {
        long possible = 0;
        for (int i = 0; i < race.time; ++i) {
            long speed = i;
            long remaining_time = race.time - i;
            long distance = speed * remaining_time;
            if (distance > race.distance) ++possible;
        };
        result *= possible;
    }
    return result;
}

long part2(const vector<Race>& races) {
    string full_race_time;
    string full_race_distance;
    for (auto& race : races) {
        full_race_time += to_string(race.time);
        full_race_distance += to_string(race.distance);
    }
    long time = stol(full_race_time);
    long distance = stol(full_race_distance);
    return ways_to_win(time, distance);
}

int main() {
    vector<string> lines = read_lines("../input/day6.txt");

    vector<Race> races = parse(lines);
    cout << part1(races) << endl;
    cout << part12(races) << endl;

    // part 2
    cout << part2(races) << endl;
}