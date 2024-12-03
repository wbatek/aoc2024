#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>

std::string readFileLines(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    return content.str();
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

int findSum(std::string line) {
    std::regex regex_part1("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    std::sregex_iterator it(line.begin(), line.end(), regex_part1);
    std::sregex_iterator end;

    int sum = 0;
    while (it != end) {
        std::smatch match = *it;
        int first = std::stoi(match.str(1));
        int second = std::stoi(match.str(2));
        sum += first * second;
        ++it;
    }
    return sum;
}

std::vector<std::string> divideLine(std::string line) {
    std::vector<std::string> result;
    std::regex pattern("(do\\(\\)(.*?)don't\\(\\))");
    std::sregex_iterator it(line.begin(), line.end(), pattern);
    std::sregex_iterator end;

    while(it != end) {
        std::smatch match = *it;
        result.push_back(match.str(1));
        ++it;
    }
    return result;
}

int findSumVector(std::vector<std::string> lines) {
    int sum = 0;
    for(auto& vec : lines) {
        sum += findSum(vec);
    }
    return sum;
}

int main() {
    // PART 1
    std::string line;
    try {
        std::string filePath = "input.txt";
        line = readFileLines(filePath);
    }
    catch (const std::ios_base::failure& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int result_1 = findSum(line);
    writeResultToFile(result_1, 1);
    // END PART 1

    std::vector<std::string> validParts = divideLine(line);
    int result_2 = findSumVector(validParts);
    writeResultToFile(result_2, 2);
}
