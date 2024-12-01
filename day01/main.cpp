#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> readFileLines(const std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

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

void splitColumns(const std::vector<std::string>& lines, std::vector<int>& column1, std::vector<int>& column2) {
    for(const auto& line : lines) {
        size_t spacePosition = line.find(' ');
        int first = std::stoi(line.substr(0, spacePosition));
        int second = std::stoi(line.substr(spacePosition + 1));
        column1.push_back(first);
        column2.push_back(second);
    }
}

int countDifferences(std::vector<int>& col1, std::vector<int>& col2) {
    std::sort(col1.begin(), col1.end());
    std::sort(col2.begin(), col2.end());
    int running_difference = 0;
    for(size_t i = 0; i < col1.size(); i++) {
        running_difference += std::abs(col1[i] - col2[i]);
    }
    return running_difference;
}

std::unordered_map<int, int> getCounts(std::vector<int>& vec) {
    std::unordered_map<int, int> result;
    for (auto& element : vec) {
        auto it = result.find(element);
        if (it != result.end()) {
            it->second += 1;
        } else {
            result[element] = 1;
        }
    }
    return result;
}

int countProducts(std::vector<int>& vec, std::unordered_map<int, int> counts) {
    int product = 0;
    for(auto& x : vec) {
        product += x * counts[x];
    }
    return product;
}

int main() {
    // PART 1
    std::vector<std::string> lines;
    try {
        std::string filePath = "input.txt";
        lines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::vector<int> col1, col2;

    splitColumns(lines, col1, col2);
    int result_1 = countDifferences(col1, col2);
    writeResultToFile(result_1, 1);
    // END PART 1

    // PART 2
    std::unordered_map<int, int> counts = getCounts(col2);
    int result_2 = countProducts(col1, counts);
    writeResultToFile(result_2, 2);
    // END PART 2
}
