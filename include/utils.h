#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> read_lines(const std::string& filename) {
    std::ifstream infile(filename);
    std::vector<std::string> output;
    std::string line;
    while (std::getline(infile, line)) {
        output.push_back(line);
    }
    return output;
}

void replace_strings(std::string& s, const std::string& from, const std::string& to) {
    int start_pos = 0;
    while ((start_pos = s.find(from, start_pos)) != std::string::npos) {
        s.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (p.first * 1000000) + p.second;
        // auto h1 = std::hash<int>{}(p.first);
        // auto h2 = std::hash<int>{}(p.second);

        // // Mainly for demonstration purposes, i.e. works but is overly simple
        // // In the real world, use sth. like boost.hash_combine
        // return h1 ^ h2;
    }
};
