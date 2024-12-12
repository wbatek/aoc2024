#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

std::vector<std::vector<char>> readFileLines(const std::string& filePath) {
    std::vector<std::vector<char>> lines;
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

void dfs(int x, int y, char target, const std::vector<std::vector<char>>& grid, std::vector<std::vector<bool>>& visited, std::vector<std::pair<int, int>>& block) {
    int rows = grid.size();
    int cols = grid[0].size();
    visited[x][y] = true;
    block.push_back({x, y});

    for (auto [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && !visited[nx][ny] && grid[nx][ny] == target) {
            dfs(nx, ny, target, grid, visited, block);
        }
    }
}

int getValueWithPerimeter(std::vector<std::pair<int, int>>& block, std::vector<std::vector<char>>& grid) {
    int area = block.size();
    int perimeter = 0;
    for(auto& [r, c] : block) {
        for(auto [dr, dc] : directions) {
            int nr = r + dr;
            int nc = c + dc;

            if (find(block.begin(), block.end(), std::make_pair(nr, nc)) == block.end()) {
                perimeter++;
            }
        }
    }
    return area * perimeter;
}

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

int countCorners(const std::vector<std::pair<int, int>>& block, std::vector<std::vector<char>>& grid) {
    std::set<std::pair<int, int>> blockSet(block.begin(), block.end());
    int corners = 0;

    for (auto [r, c] : block) {
        int top = (r > 0) ? grid[r - 1][c] : -1;
        int bottom = (r < grid.size() - 1) ? grid[r + 1][c] : -1;
        int left = (c > 0) ? grid[r][c - 1] : -1;
        int right = (c < grid[0].size() - 1) ? grid[r][c + 1] : -1;

        int top_left = (r > 0 && c > 0) ? grid[r - 1][c - 1] : -1;
        int top_right = (r > 0 && c < grid[0].size() - 1) ? grid[r - 1][c + 1] : -1;
        int bottom_left = (r < grid.size() - 1 && c > 0) ? grid[r + 1][c - 1] : -1;
        int bottom_right = (r < grid.size() - 1 && c < grid[0].size() - 1) ? grid[r + 1][c + 1] : -1;

        int curr = grid[r][c];

        if ((top == -1 && right == -1) || (curr != top && curr != right)) {
            corners++;
        }
        if ((top == -1 && left == -1) || (curr != top && curr != left)) {
            corners++;
        }
        if ((bottom == -1 && right == -1) || (curr != bottom && curr != right)) {
            corners++;
        }
        if ((bottom == -1 && left == -1) || (curr != bottom && curr != left)) {
            corners++;
        }
        if (top != -1 && right != -1 && curr == top && curr == right && top_right != curr) {
            corners++;
        }
        if (top != -1 && left != -1 && curr == top && curr == left && top_left != curr) {
            corners++;
        }
        if (bottom != -1 && right != -1 && curr == bottom && curr == right && bottom_right != curr) {
            corners++;
        }
        if (bottom != -1 && left != -1 && curr == bottom && curr == left && bottom_left != curr) {
            corners++;
        }
    }
    return corners;
}

int getValueWithFencing(std::vector<std::pair<int, int>>& block, std::vector<std::vector<char>>& grid) {
    int area = block.size();
    int n_corners = countCorners(block, grid);
    return area * n_corners;
}

template<typename Func>
int solution(std::vector<std::vector<std::pair<int, int>>>& blocks, std::vector<std::vector<char>>& grid, Func countFunc) {
    int result = 0;
    for(auto& block : blocks) {
        result += countFunc(block, grid);
    }
    return result;
}

int main() {
    std::vector<std::vector<char>> grid;
    try {
        std::string filePath = "input.txt";
        grid = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> blocks;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!visited[i][j]) {
                std::vector<std::pair<int, int>> block;
                dfs(i, j, grid[i][j], grid, visited, block);
                blocks.push_back(block);
            }
        }
    }

    int result_1 = solution(blocks, grid, getValueWithPerimeter);
    writeResultToFile(result_1, 1);

    int result_2 = solution(blocks, grid, getValueWithFencing);
    writeResultToFile(result_2, 2);
}