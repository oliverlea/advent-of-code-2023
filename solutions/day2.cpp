#include "../include/utils.h"

#include <iostream>
#include <istream>
#include <string>
#include <vector>

struct Draw {
    int red;
    int blue;
    int green;
};

struct GameResult {
    int id;
    std::vector<Draw> draws;
};


GameResult parse_line(const std::string& line) {
    std::istringstream stream(line);
    std::string game_str;
    std::getline(stream, game_str, ':');
    int game_id = std::stoi(game_str.substr(5));

    GameResult result;
    result.id = game_id;

    std::string draw_str;
    while (std::getline(stream, draw_str, ';')) {
        std::istringstream pick_stream(draw_str);
        std::string colour_pick_str;
        int red = 0;
        int blue = 0;
        int green = 0;
        while (std::getline(pick_stream, colour_pick_str, ',')) {
            std::istringstream colour_pick_stream(colour_pick_str);
            std::string count_str;
            colour_pick_stream >> count_str;
            int count = std::stoi(count_str);

            std::string colour;
            colour_pick_stream >> colour;

            if (colour == "red") red += count;
            else if (colour == "blue") blue += count;
            else if (colour == "green") green += count;
        }
        result.draws.push_back({red, blue, green});
    }
    return result;
}

int part1(const std::vector<GameResult>& input) {
    int result = 0;
    for (int i = 0; i < input.size(); ++i) {
        GameResult cur = input[i];
        bool possible = true;
        for (Draw draw : cur.draws) {
            if (draw.red > 12 || draw.green > 13 || draw.blue > 14) {
                possible = false;
                break;
            }
        }
        if (possible) result += cur.id;
    }
    return result;
}

long part2(const std::vector<GameResult>& input) {
    long result = 0;
    for (int i = 0; i < input.size(); ++i) {
        GameResult cur = input[i];
        int min_red = 0;
        int min_blue = 0;
        int min_green = 0;
        for (Draw draw : cur.draws) {
            min_red = std::max(min_red, draw.red);
            min_blue = std::max(min_blue, draw.blue);
            min_green = std::max(min_green, draw.green);
        }
        long power = static_cast<long>(min_red) * min_blue * min_green;
        result += power;
    }
    return result;
}

int main() {
    std::vector<std::string> lines = read_lines("../input/day2.txt");
    std::vector<GameResult> input(lines.size());
    for (int i = 0; i < lines.size(); ++i) {
        input[i] = parse_line(lines[i]);
    }
    std::cout << part1(input) << std::endl;
    std::cout << part2(input) << std::endl;
}