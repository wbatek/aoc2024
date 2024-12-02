#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::vector<std::vector<int>> readFileLines(const std::string& filePath) {
    std::vector<std::vector<int>> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while(std::getline(file, line)) {
        std::vector<int> currentLine;
        std::istringstream iss(line);
        int num;
        while(iss >> num) {
            currentLine.push_back(num);
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

bool isMonotonic(std::vector<int>& vec) {
    if (vec.size() <= 1) return true;

    bool increasing = false, decreasing = false;

    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] > vec[i - 1]) {
            increasing = true;
        } else if (vec[i] < vec[i - 1]) {
            decreasing = true;
        }
        if (increasing && decreasing) return false;
    }

    return increasing || decreasing;
}

bool hasCorrectDifferences(std::vector<int>& vec) {
    for(size_t i = 1; i < vec.size(); i++) {
        int diff = std::abs(vec[i] - vec[i - 1]);
        if(diff < 1 || diff > 3) {
            return false;
        } 
    }
    return true;
}

bool isCorrect(std::vector<int>& vec) {
    return isMonotonic(vec) && hasCorrectDifferences(vec);
}

std::vector<std::vector<int>> generateSubvectors(const std::vector<int>& vec) {
    std::vector<std::vector<int>> subvectors;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::vector<int> subvector;
        for (size_t j = 0; j < vec.size(); ++j) {
            if (j != i) {
                subvector.push_back(vec[j]);
            }
        }
        subvectors.push_back(subvector);
    }
    return subvectors;
}

bool isCorrect_p2(std::vector<int>& vec) {
    std::vector<std::vector<int>> subvectors = generateSubvectors(vec);
    for(auto& subvec : subvectors) {
        if(isCorrect(subvec)) {
            return true;
        }
    }
    return false;
}

template <typename CFunc>
int countSafeLines(std::vector<std::vector<int>>& lines, CFunc isCorrect) {
    int count = 0;
    for(auto& line : lines) {
        if(isCorrect(line)) {
            count++;
        }
    }
    return count;
}

int main() {
    // PART 1
    std::vector<std::vector<int>> lines;
    try {
        std::string filePath = "input.txt";
        lines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int result_1 = countSafeLines(lines, isCorrect);
    writeResultToFile(result_1, 1);
    // END PART 1
    
    int result_2 = countSafeLines(lines, isCorrect_p2);
    writeResultToFile(result_2, 2);
}