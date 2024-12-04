#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

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

template<char X>
std::queue<std::pair<int, int> > getPlacements(std::vector<std::vector<char> >& a) {
    std::queue<std::pair<int, int> > q;
    for(size_t i = 0; i < a.size(); i++) {
        for(size_t j = 0; j < a[i].size(); j++) {
            if(a[i][j] == X) {
                q.push(std::make_pair(i, j));
            }
        }
    }
    return q;
}

int countPart1(std::vector<std::vector<char> >& a) {
    int count = 0;
    std::queue<std::pair<int, int> > q = getPlacements<'X'>(a);
    for (; !q.empty(); q.pop()) {
        std::pair<int, int> current = q.front();
        int r = std::get<0>(current);
        int c = std::get<1>(current);

        for(int dr = -1; dr <= 1; dr++) {
            for(int dc = -1; dc <= 1; dc++) {
                if(dr == 0 && dc == 0) continue;
                if(!(r + 3 * dr >= 0 && r + 3 * dr < a.size() && c + 3 * dc >= 0 && c + 3 * dc < a[0].size())) continue;

                if(a[r + dr][c + dc] == 'M' && a[r + 2*dr][c + 2*dc] == 'A' && a[r + 3*dr][c + 3*dc] == 'S') count++;
            }
        }
    }
    return count;
}

int countPart2(std::vector<std::vector<char> >& a) {
    int count = 0;
    std::queue<std::pair<int, int> > q = getPlacements<'A'>(a);
    for (; !q.empty(); q.pop()) {
        std::pair<int, int> current = q.front();
        int r = std::get<0>(current);
        int c = std::get<1>(current);
        std::set<std::string> possible_corners{"MMSS", "MSMS", "SSMM", "SMSM"};

        if(!(r - 1 >= 0 && r + 1 < a.size() && c - 1 >= 0 && c + 1 < a[0].size())) continue;

        if(possible_corners.find(std::string(1, a[r-1][c-1]) + a[r-1][c+1] + a[r+1][c-1] + a[r+1][c+1]) != possible_corners.end()) count++;
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
    catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int result_1 = countPart1(lines);
    writeResultToFile(result_1, 1);
    // END PART 1

    // PART 2
    int result_2 = countPart2(lines);
    writeResultToFile(result_2, 2);
    // END PART 2
}