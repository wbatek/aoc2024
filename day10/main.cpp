#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

std::vector<std::vector<int>> readFileLines(const std::string& filePath) {
    std::vector<std::vector<int>> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while(std::getline(file, line)) {
        std::vector<int> currentLine;
        for(char c : line) {
            if (isdigit(c)) { // Ensure the character is a digit
                currentLine.push_back(c - '0');
            }
        }
        lines.push_back(currentLine);
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

template<int X>
std::vector<std::pair<int, int>> getLocation(std::vector<std::vector<int>>& grid) {
    std::vector<std::pair<int, int>> result;
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == X) {
                result.push_back(std::make_pair(i, j));
            }
        }
    }
    return result;
}

template <typename ConditionFunc>
int getReachableCount(std::vector<std::vector<int>>& grid, std::pair<int, int>& startLocation, ConditionFunc shouldCount) {
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    int count = 0;
    std::vector<std::pair<std::pair<int, int>, int>> stack;
    std::set<std::pair<int, int>> seen_9s;

    stack.push_back(std::make_pair(startLocation, 0));

    while (!stack.empty()) {
        auto [location, prev_value] = stack.back();
        stack.pop_back();

        int r = location.first;
        int c = location.second;

        for (int i = 0; i < 4; i++) {
            int new_r = r + dr[i];
            int new_c = c + dc[i];

            if (new_r >= 0 && new_r < grid.size() && new_c >= 0 && new_c < grid[new_r].size()) {
                std::pair<int, int> p = std::make_pair(new_r, new_c);

                if (grid[new_r][new_c] == prev_value + 1) {
                    if (shouldCount(grid[new_r][new_c], p, seen_9s)) {
                        count++;
                        seen_9s.insert(p);
                    } else {
                        stack.push_back(std::make_pair(p, grid[new_r][new_c]));
                    }
                }
            }
        }
    }

    return count;
}

bool part1Condition(int cellValue, std::pair<int, int>& p, std::set<std::pair<int, int>>& seen_9s) {
    return (cellValue == 9 && seen_9s.find(p) == seen_9s.end());
}

bool part2Condition(int cellValue, std::pair<int, int>& p, std::set<std::pair<int, int>>& seen_9s) {
    return (cellValue == 9);
}

int part1(std::vector<std::vector<int>>& grid, std::vector<std::pair<int, int>>& zeroLocations) {
    int result = 0;
    for (std::pair<int, int> currentZero : zeroLocations) {
        result += getReachableCount(grid, currentZero, part1Condition);
    }
    return result;
}

int part2(std::vector<std::vector<int>>& grid, std::vector<std::pair<int, int>>& zeroLocations) {
    int result = 0;
    for (std::pair<int, int> currentZero : zeroLocations) {
        result += getReachableCount(grid, currentZero, part2Condition);
    }
    return result;
}

int main() {
    // PART 1
    std::vector<std::vector<int>> lines;
    try {
        std::string filePath = "input.txt";
        lines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::vector<std::pair<int, int>> zeroLocations = getLocation<0>(lines);
    int result_1 = part1(lines, zeroLocations);
    writeResultToFile(result_1, 1);
    
    int result_2 = part2(lines, zeroLocations);
    writeResultToFile(result_2, 2);
}
    