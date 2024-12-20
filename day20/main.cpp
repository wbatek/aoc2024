#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include <set>
#include <sstream>
#include <utility>

std::vector<std::vector<char> > readFileLines(const std::string& filePath) {
    std::vector <std::vector<char> > grid;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }
    return grid;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

template<char X>
std::pair<int, int> find(std::vector<std::vector<char>>& grid) {
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == X) return std::make_pair(i, j);
        }
    }
    return std::make_pair(-1, -1);
}

void bfs(std::vector<std::vector<char>>& grid, std::vector<std::vector<int>>& distances, std::pair<int, int> start) {
    int rows = grid.size();
    int cols = grid[0].size();

    distances[start.first][start.second] = 1;
    std::vector<std::pair<int, int>> directions = {
            {1, 0},  // Down
            {-1, 0}, // Up
            {0, 1},  // Right
            {0, -1}  // Left
    };

    std::set<std::pair<int, int>> seen;
    std::queue<std::pair<int, int>> q;
    q.push(start);
    seen.insert(start);

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc;
            if (grid[nr][nc] == '#' || seen.count({nr, nc}) > 0) continue;
            distances[nr][nc] = distances[r][c] + 1;
            seen.insert({nr, nc});
            q.push({nr, nc});
        }
    }
}

std::vector<std::pair<int, int>> generateCheats(int max) {
    std::vector<std::pair<int, int>> cheats;
    for(int first = -max; first <= max; first++) {
        for(int second = -max; second <= max; second++) {
            if(std::abs(first) + std::abs(second) > max) continue;
            cheats.push_back({first, second});
        }
    }
    return cheats;
}

int part1(std::vector<std::vector<char>>& grid, std::vector<std::vector<int>>& distFromBeginning,
          std::vector<std::vector<int>>& distToEnd, std::pair<int, int> begin, std::pair<int, int> end, int max) {
    std::vector<std::pair<int, int>> cheats = generateCheats(max);
    int count = 0;

    for(int r = 0; r < grid.size(); r++) {
        for(int c = 0; c < grid[r].size(); c++) {
            if(grid[r][c] == '#') continue;
            for(auto& [dr, dc] : cheats) {
                int cost = std::abs(dr) + std::abs(dc);
                int nr = r + dr;
                int nc = c + dc;
                if(nr < 1 || nr > grid.size() - 2 || nc < 1 || nc > grid[nr].size() - 2) continue;
                if(grid[nr][nc] == '#') continue;
                int x = distFromBeginning[r][c] - 1;
                int y = distToEnd[nr][nc] - 1;
                if(distFromBeginning[end.first][end.second] - (x + y + cost) >= 100) count++;
            }
        }
    }
    return count;
}

int main() {
    std::vector<std::vector<char> > grid;
    try {
        std::string filePath = "input.txt";
        grid = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::vector<std::vector<int>> distFromBeginning(grid.size(), std::vector<int>(grid[0].size(), 0));
    std::vector<std::vector<int>> distToEnd(grid.size(), std::vector<int>(grid[0].size(), 0));

    std::pair<int, int> begin = find<'S'>(grid);
    std::pair<int, int> end = find<'E'>(grid);

    bfs(grid, distFromBeginning, begin);
    bfs(grid, distToEnd, end);

    int result_1 = part1(grid, distFromBeginning, distToEnd, begin, end, 2);
    writeResultToFile(result_1, 1);

    int result_2 = part1(grid, distFromBeginning, distToEnd, begin, end, 20);
    writeResultToFile(result_2, 2);
}