#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>

std::vector<std::vector<char> > readFileLines(const std::string& filePath) {
    std::vector<std::vector<char> > lines;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while(std::getline(file, line)) {
        lines.push_back(std::vector<char>(line.begin(), line.end()));
    }

    file.close();
    return lines;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

enum Chars {
    UP = '^',
    DOWN = 'v',
    LEFT = '<',
    RIGHT = '>',
    EMPTY = '.',
    SEEN = 'X',
    OBSTACLE = '#'
};

struct DirectionData {
    int rowDelta;
    int colDelta;
    Chars nextDirection;
};

struct DirectionPoint {
    int r;
    int c;
    Chars direction;

    bool operator==(const DirectionPoint& other) const {
        return r == other.r && c == other.c && direction == other.direction;
    }
};

struct DirectionPointHash {
    size_t operator()(const DirectionPoint& dp) const {
        size_t hash_r = std::hash<int>{}(dp.r);
        size_t hash_c = std::hash<int>{}(dp.c);
        size_t hash_direction = std::hash<Chars>{}(dp.direction);
        return hash_r ^ (hash_c << 1) ^ (hash_direction << 2);
    }
};

std::pair<int, int> getStartPosition(std::vector<std::vector<char>>& lines) {
    for(int i = 0; i < lines.size(); i++) {
        for(int j = 0; j < lines[i].size(); j++) {
            if(lines[i][j] == Chars::UP) {
                lines[i][j] = Chars::EMPTY;
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

int part1(std::vector<std::vector<char>> lines) {
    int count = 0;
    Chars direction = UP;
    auto [r, c] = getStartPosition(lines);

    std::unordered_map<Chars, DirectionData> directionDeltas = {
            {UP, {-1, 0, RIGHT}},
            {DOWN, {1, 0, LEFT}},
            {RIGHT, {0, 1, DOWN}},
            {LEFT, {0, -1, UP}}
    };

    while(r >= 0 && c >= 0 && r < lines.size() && c < lines[r].size()) {
       if(lines[r][c] == Chars::OBSTACLE) {
           r -= directionDeltas[direction].rowDelta;
           c -= directionDeltas[direction].colDelta;
           direction = directionDeltas[direction].nextDirection;
       }
       else {
           if(lines[r][c] == Chars::EMPTY) {
               lines[r][c] = Chars::SEEN;
               count++;
           }
           r += directionDeltas[direction].rowDelta;
           c += directionDeltas[direction].colDelta;
       }
    }

    return count;
}

bool checkLoops(std::vector<std::vector<char>>& lines, int r, int c) {
    Chars direction = UP;
    std::unordered_map<Chars, DirectionData> directionDeltas = {
            {UP, {-1, 0, RIGHT}},
            {DOWN, {1, 0, LEFT}},
            {RIGHT, {0, 1, DOWN}},
            {LEFT, {0, -1, UP}}
    };

    std::unordered_set<DirectionPoint, DirectionPointHash> seen;

    while(r >= 0 && c >= 0 && r < lines.size() && c < lines[r].size()) {
        if(lines[r][c] == Chars::OBSTACLE) {
            r -= directionDeltas[direction].rowDelta;
            c -= directionDeltas[direction].colDelta;
            direction = directionDeltas[direction].nextDirection;
        }
        else {
            if(seen.find({r, c, direction}) != seen.end()) {
                return true;
            }
            seen.insert({r, c, direction});
            if(lines[r][c] == Chars::EMPTY) {
                lines[r][c] = Chars::SEEN;
            }
            r += directionDeltas[direction].rowDelta;
            c += directionDeltas[direction].colDelta;
        }
    }
    return false;
}

int part2(std::vector<std::vector<char>>& lines) {
    int count = 0;
    auto [r, c] = getStartPosition(lines);

    for(int i = 0; i < lines.size(); i++) {
        for(int j = 0; j < lines[i].size(); j++) {
            std::vector<std::vector<char>> deepCopy = lines;
            deepCopy[i][j] = Chars::OBSTACLE;
            if(checkLoops(deepCopy, r, c)) {
                count++;
            }
        }
    }
    return count;
}

int main() {
    // PART 1
    std::vector<std::vector<char> > lines;
    try {
        std::string filePath = "input.txt";
        lines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int result_1 = part1(lines);
    writeResultToFile(result_1, 1);
    // END PART 1

    int result_2 = part2(lines);
    writeResultToFile(result_2, 2);

}
