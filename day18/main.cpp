#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>

std::vector<std::pair<int, int>> readFileLines(const std::string& filePath) {
    std::vector<std::pair<int, int>> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) throw std::ios_base::failure("Failed to open the file: " + filePath);
    std::string line;
    while(std::getline(file, line)) {
        int f = std::stoi(line.substr(0, line.find(',')));
        int s = std::stoi(line.substr(line.find(',') + 1));
        lines.push_back(std::make_pair(f, s));
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

int part1(std::vector<std::pair<int, int>>& coordinates, int size, int n) {
    std::vector<std::vector<char>> grid(size + 1, std::vector<char>(size + 1, '.'));
    for(size_t i = 0; i < n; i++) {
        std::pair<int, int> current = coordinates[i];
        grid[current.first][current.second] = '#';
    }

    std::vector<std::pair<int, int>> directions = {
            {1, 0},  // Right
            {-1, 0}, // Left
            {0, 1},  // Down
            {0, -1}  // Up
    };

    std::set<std::pair<int, int>> seen;
    std::vector<std::tuple<int, int, int>> path;

    path.push_back(std::make_tuple(0, 0, 0));
    seen.insert(std::make_pair(0, 0));

    while(!path.empty()) {
        auto current = path.front();
        std::remove(path.begin(), path.end(), current);
        path.pop_back();
        int r = std::get<0>(current);
        int c = std::get<1>(current);
        int d = std::get<2>(current);

        for(auto& [dr, dc] : directions) {
            int nr = r + dr;
            int nc = c + dc;
            if(nr < 0 || nc < 0 || nr > size || nc > size) continue;
            if(grid[nr][nc] == '#') continue;
            if(seen.find(std::make_pair(nr, nc)) != seen.end()) continue;
            if(nr == size && nc == size) {
                return d + 1;
            }
            seen.insert(std::make_pair(nr, nc));
            path.push_back(std::make_tuple(nr, nc, d + 1));
        }
    }
    return -1;
}

std::string part2(std::vector<std::pair<int, int>>& coordinates, int size) {
    for(int i = 0; i < coordinates.size(); i++) {
        if(part1(coordinates, size, i) == -1) {
            return std::to_string(coordinates[i - 1].first) + "," + std::to_string(coordinates[i - 1].second);
        }
    }
}

int main() {
    std::vector<std::pair<int, int>> coordinates;
    try {
        std::string filePath = "input.txt";
        coordinates = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    int result_1 = part1(coordinates, 70, 1024);
    writeResultToFile(result_1, 1);

    std::string result_2 = part2(coordinates, 70);
    writeResultToFile(result_2, 2);
}