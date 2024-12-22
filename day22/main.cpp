#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using ull = unsigned long long;

std::vector<ull> readFileLines(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<ull> result;

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;

    while(std::getline(file, line)) {
        result.push_back(std::stoull(line));
    }
    return result;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

ull calculateNext(ull n, ull count) {
    for(int i = 0; i < count; i++) {
        n = (n ^ (64 * n)) % 16777216;
        n = (n ^ ull(std::floor(n / 32))) % 16777216;
        n = (n ^ (2048 * n)) % 16777216;
    }
    return n;
}

ull part1(std::vector<ull>& numbers) {
    ull result = 0;
    for(ull n : numbers) {
        result += calculateNext(n, 2000);
    }
    return result;
}

int main() {
    std::vector<ull> numbers;
    try {
        std::string filePath = "input.txt";
        numbers = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    ull result_1 = part1(numbers);
    writeResultToFile(result_1, 1);
}