#include <iostream>
#include <vector>
#include <string>
#include <fstream>

enum Chars {
    EMPTY = '.',
    SEEN = '#'
};

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

std::unordered_map<char, std::vector<std::pair<int, int>>> getPositions(std::vector<std::vector<char>>& lines) {
    std::unordered_map<char, std::vector<std::pair<int, int>>> antenna_positions;
    for(int r = 0; r < lines.size(); r++) {
        for(int c = 0; c < lines[r].size(); c++) {
            if(lines[r][c] != Chars::EMPTY) {
                antenna_positions[lines[r][c]].push_back(std::make_pair(r, c));
            }
        }
    }
    return antenna_positions;
}

int part1(std::vector<std::vector<char>> lines, std::unordered_map<char, std::vector<std::pair<int, int>>>& positions) {
    int count = 0;

    for(const auto& [ch, vec] : positions) {
        for (size_t i = 0; i < vec.size(); i++) {
            for (size_t j = i + 1; j < vec.size(); j++) {
                auto [r1, c1] = vec[i];
                auto [r2, c2] = vec[j];
                int r_diff = r2 - r1;
                int c_diff = c2 - c1;

                int r_delta = r1 - r_diff;
                int c_delta = c1 - c_diff;
                if(r_delta >= 0 && r_delta < lines.size() && c_delta >= 0 && c_delta < lines[r1].size()) {
                    if(lines[r_delta][c_delta] != Chars::SEEN) {
                        count++;
                        lines[r_delta][c_delta] = Chars::SEEN;
                    }
                }
                r_delta = r2 + r_diff;
                c_delta = c2 + c_diff;
                if(r_delta >= 0 && r_delta < lines.size() && c_delta >= 0 && c_delta < lines[r1].size()) {
                    if(lines[r_delta][c_delta] != Chars::SEEN) {
                        count++;
                        lines[r_delta][c_delta] = Chars::SEEN;
                    }
                }
            }
        }
    }
    return count;
}

int part2(std::vector<std::vector<char>> lines, std::unordered_map<char, std::vector<std::pair<int, int>>>& positions, int range) {
    int count = 0;

    for(const auto& [ch, vec] : positions) {
        for (size_t i = 0; i < vec.size(); i++) {
            for (size_t j = i + 1; j < vec.size(); j++) {
                auto [r1, c1] = vec[i];
                auto [r2, c2] = vec[j];
                int r_diff = r2 - r1;
                int c_diff = c2 - c1;

                for(int x = 0; x <= range; x++) {
                    int r_delta = r1 - x * r_diff;
                    int c_delta = c1 - x * c_diff;
                    if(r_delta >= 0 && r_delta < lines.size() && c_delta >= 0 && c_delta < lines[r1].size()) {
                        if(lines[r_delta][c_delta] != Chars::SEEN) {
                            count++;
                            lines[r_delta][c_delta] = Chars::SEEN;
                        }
                    }
                    r_delta = r2 + x * r_diff;
                    c_delta = c2 + x * c_diff;
                    if(r_delta >= 0 && r_delta < lines.size() && c_delta >= 0 && c_delta < lines[r1].size()) {
                        if(lines[r_delta][c_delta] != Chars::SEEN) {
                            count++;
                            lines[r_delta][c_delta] = Chars::SEEN;
                        }
                    }
                }
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

    std::unordered_map<char, std::vector<std::pair<int, int>>> antenna_positions = getPositions(lines);
    int result_1 = part1(lines, antenna_positions);
    writeResultToFile(result_1, 1);

    int result_2 = part2(lines, antenna_positions, lines.size());
    writeResultToFile(result_2, 2);
}