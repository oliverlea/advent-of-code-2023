#include "../include/utils.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

int part1(std::vector<std::string>& lines) {
    int result = 0;
    for (std::string& line : lines) {
        int first = -1;
        int last = -1;
        for (char c : line) {
            if (isdigit(c)) {
                int digit = c - '0';
                if (first == -1) first = digit;
                last = digit;
            }
        }
        result += (first * 10) + last;
    }
    return result;
}

int part2(std::vector<std::string>& lines) {
    std::map<std::string, std::string> replacements = {
        {"one", "o1e"},
        {"two", "t2o"},
        {"three", "t3e"},
        {"four", "f4r"},
        {"five", "f5e"},
        {"six", "s6x"},
        {"seven", "s7n"},
        {"eight", "e8t"},
        {"nine", "n9e"},
    };
    for (std::string& line : lines) {
        while (true) {
            std::map<std::string, int> candidates;
            for (auto entry : replacements) {
                std::string::size_type location = line.find(entry.first, 0);
                if (location != std::string::npos) {
                    candidates[entry.first] = location;
                }
            }
            if (candidates.empty()) break;
            std::pair<int, std::string> lowest = {std::numeric_limits<int>().max(), ""};
            for (auto entry : candidates) {
                if (entry.second < lowest.first) {
                    lowest = {entry.second, entry.first};
                }
            }
            replace_strings(line, lowest.second, replacements[lowest.second]);
        }
    }
    return part1(lines);
}

int main() {
    std::vector<std::string> example = {
        "two1nine",
        "eightwothree",
        "abcone2threexyz",
        "xtwone3four",
        "4nineeightseven2",
        "zoneight234",
        "7pqrstsixteen"
    };
    std::cout << part2(example) << std::endl;

    std::vector<std::string> lines = read_lines("../input/day1.txt");
    std::cout << part1(lines) << std::endl;
    std::cout << part2(lines) << std::endl;

}