#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>

std::vector<std::string> readFileLines(const std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) throw std::ios_base::failure("Failed to open the file: " + filePath);
    std::string line;
    while(std::getline(file, line)) {
        lines.push_back(line);
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

std::pair<std::vector<std::string>, std::vector<std::string>> splitByBlankLine(const std::vector<std::string>& lines) {
    std::vector<std::string> firstPart, secondPart;

    auto blankLineIt = std::find(lines.begin(), lines.end(), "");
    if (blankLineIt != lines.end()) {
        firstPart.assign(lines.begin(), blankLineIt);
        secondPart.assign(blankLineIt + 1, lines.end());
    } else {
        firstPart = lines;
    }

    return {firstPart, secondPart};
}

std::vector<int> parseLine(std::string& line) {
    std::vector<int> values;
    std::stringstream ss(line);
    std::string token;

    while(std::getline(ss, token, ',')) {
        values.push_back(std::stoi(token));
    }
    return values;
}

std::unordered_map<int, std::vector<int>> constructMap(std::vector<std::string>& lines) {
    std::unordered_map<int, std::vector<int>> map;

    for(std::string& line : lines) {
        size_t pipePos = line.find('|');
        int first = std::stoi(line.substr(0, pipePos));
        int second = std::stoi(line.substr(pipePos + 1));

        map[first].push_back(second);
    }
    return map;
}

bool isCorrectLine(std::vector<int> values, std::unordered_map<int, std::vector<int>>& map) {
    for(int i = 0; i < values.size(); i++) {
        for(int j = i + 1; j < values.size(); j++) {
            int prev = values[i];
            int next = values[j];
            if(map.find(next) != map.end()) {
                const std::vector<int>& vec = map.at(next);
                if(std::find(vec.begin(), vec.end(), prev) != vec.end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

int getSumOfCorrect(std::vector<std::string>& lines, std::unordered_map<int, std::vector<int>>& map) {
    int sum = 0;
    for(auto& line : lines) {
        std::vector<int> values = parseLine(line);
        if(isCorrectLine(values, map)) {
            sum += values[values.size() / 2];
        }
    }
    return sum;
}

void fixLine(std::vector<int>& values, std::unordered_map<int, std::vector<int>>& map) {
    for(int i = 0; i < values.size(); i++) {
        for(int j = i + 1; j < values.size(); j++) {
            int prev = values[i];
            int next = values[j];
            if(map.find(next) != map.end()) {
                const std::vector<int>& vec = map.at(next);
                if(std::find(vec.begin(), vec.end(), prev) != vec.end()) {
                    std::swap(values[i], values[j]);
                }
            }
        }
    }
}

int getSumOfIncorrect(std::vector<std::string>& lines, std::unordered_map<int, std::vector<int>>& map) {
    int sum = 0;
    for(auto& line : lines) {
        std::vector<int> values = parseLine(line);
        if(!isCorrectLine(values, map)) {
            fixLine(values, map);
            sum += values[values.size() / 2];
        }
    }
    return sum;
}

int main() {
    // PART 1
    std::vector<std::string> lines;
    try {
        std::string filePath = "input.txt";
        lines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    auto [mappings, pages_to_produce] = splitByBlankLine(lines);

    std::unordered_map<int, std::vector<int>> map = constructMap(mappings);
    int result_1 = getSumOfCorrect(pages_to_produce, map);
    writeResultToFile(result_1, 1);
    // END PART 1

    // PART 2
    int result_2 = getSumOfIncorrect(pages_to_produce, map);
    writeResultToFile(result_2, 2);
}