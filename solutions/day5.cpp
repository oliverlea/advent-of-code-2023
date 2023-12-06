#include "../include/timer.h"
#include "../include/utils.h"

#include <iostream>
#include <istream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct Range {
    long start;
    long end;

    Range(long start, long end) : start(start), end(end) {}

    bool contains(const long x) const {
        return x >= start && x <= end;
    }
};

ostream& operator<<(ostream& os, const Range& range) {
    return os << "[" << range.start << ", " << range.end << "]";
}

struct Mapping {
    Range source;
    Range dest; 

    Mapping(long dest_start, long source_start, long length) : 
        source{source_start, source_start + (length - 1)}, 
        dest{dest_start, dest_start + (length - 1)} {}

    bool contains(const long x) const {
        return x >= source.start && x <= source.end;
    }

    long map_to_target(const long x) const {
        return dest.start + (x - source.start);
    }

    Range map_to_target(const Range& range) const {
        long start = map_to_target(range.start);
        long end = map_to_target(range.end);
        return {start, end};
    }

    vector<Range> map(const Range& range) const {
        vector<Range> results;
        if (range.end < source.start) return results;
        if (range.start > source.end) return results;

        Range inter{max(range.start, source.start), min(range.end, source.end)};
        results.push_back(inter);

        if (range.start < source.start) {
            results.push_back({range.start, inter.start - 1});
        }
        if (range.end > source.end) {
            results.push_back({source.end + 1, range.end});
        }
        return results;
    }
};

typedef vector<Mapping> Conversion; 

struct Input {
    vector<long> seeds;
    vector<Range> seed_ranges;
    vector<Conversion> conversions;

    Input(vector<long> seeds, vector<Conversion> conversions) : seeds(seeds), conversions(conversions) {
        for (int i = 0; i < seeds.size(); i += 2) {
            Range range{seeds[i], seeds[i] + (seeds[i + 1] - 1)};
            seed_ranges.push_back(range);
        }
    }
};


Input parse_input(const vector<string>& lines) {
    // read seeds
    vector<long> seeds;
    string tmp;
    istringstream ss(lines[0]);
    ss >> tmp;
    long cur_seed;
    while (ss >> cur_seed) {
        seeds.push_back(cur_seed);
    }
    
    vector<Conversion> conversions;
    Conversion cur;
    for (int i = 1; i < lines.size(); ++i) {
        istringstream ss_line(lines[i]);
        string s;
        getline(ss_line, s);
        if (s.empty()) {
            if (!cur.empty()) {
                conversions.push_back(cur);
                cur = Conversion{};
            }
        } else if (s.find(':') != std::string::npos) {
            // skip mapping line
            continue;
        } else {
            istringstream input_ss(s);
            long dest_start;
            long source_start;
            long length;
            input_ss >> dest_start >> source_start >> length;
            Mapping m{dest_start, source_start, length};
            cur.push_back(m);
        }
    }
    if (!cur.empty()) {
        conversions.push_back(cur);
    }
    return {
        seeds,
        conversions
    };
}

long part1(Input& input) {
    Timer timer;
    long result = numeric_limits<long>().max();
    for (const long seed : input.seeds) {
        long mapped_index = seed;
        for (const Conversion conversions : input.conversions) {
            for (const Mapping& mapping : conversions) {
                if (mapping.contains(mapped_index)) {
                    mapped_index = mapping.map_to_target(mapped_index);
                    break;
                }
            }
            // cur = cur
        }
        result = min(result, mapped_index);
    }
    return result;
}

long part2(Input& input) {
    Timer timer;
    vector<Range> considerable_ranges(input.seed_ranges.begin(), input.seed_ranges.end());
    vector<Range> next_step_ranges;
    for (const Conversion& conversion : input.conversions) {
        while (!considerable_ranges.empty()) {
            const Range cur_range = considerable_ranges.back();
            considerable_ranges.pop_back();
            bool mapped = false;
            for (const Mapping& mapping : conversion) {
                vector<Range> splits = mapping.map(cur_range);
                if (splits.empty()) continue;
                mapped = true;
                next_step_ranges.push_back(mapping.map_to_target(splits[0]));
                for (int i = 1; i < splits.size(); ++i) {
                    considerable_ranges.push_back(splits[i]);
                }
                break;
            }
            if (!mapped) next_step_ranges.push_back(cur_range);
        }
        swap(considerable_ranges, next_step_ranges);
    }
    long result = LONG_MAX;
    for (const Range& range : considerable_ranges) {
        result = min(result, range.start);
    }
    return result;
}

int main() {
    vector<string> lines = read_lines("../input/day5.txt");
    Input input = parse_input(lines);

    cout << part1(input) << endl;

    cout << part2(input) << endl;
}

