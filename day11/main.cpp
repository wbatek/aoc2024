#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using CacheKey = std::pair<unsigned long long, int>;
struct CacheKeyHash {
    std::size_t operator()(const CacheKey& key) const {
        return std::hash<unsigned long long>()(key.first) ^ (std::hash<int>()(key.second) << 1);
    }
};

std::vector<unsigned long long> readFileLines(const std::string& filePath) {
    std::vector<unsigned long long> numbers;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    if (std::getline(file, line)) {
        std::istringstream stream(line);
        unsigned long long number;
        while (stream >> number) {
            numbers.push_back(number);
        }
    }

    file.close();
    return numbers;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

int numDigits(unsigned long long number) {
    return std::to_string(number).size();
}

std::pair<unsigned long long, unsigned long long> splitNumber(unsigned long long number) {
    std::string numStr = std::to_string(number);
    size_t mid = numStr.size() / 2;

    std::string firstHalfStr = numStr.substr(0, mid);
    std::string secondHalfStr = numStr.substr(mid);

    unsigned long long firstHalf = firstHalfStr.empty() ? 0 : std::stoull(firstHalfStr);
    unsigned long long secondHalf = secondHalfStr.empty() ? 0 : std::stoull(secondHalfStr);

    return {firstHalf, secondHalf};
}

unsigned long long getValue(unsigned long long n, int blinks, std::unordered_map<CacheKey, unsigned long long, CacheKeyHash>& cache) {
    if(blinks == 0) {
        return 1;
    }

    CacheKey key = {n, blinks};
    if (cache.find(key) != cache.end()) {
        return cache[key];
    }

    unsigned long long result;

    if(n == 0) {
        result = getValue(1, blinks - 1, cache);
    }
    else if(numDigits(n) % 2 == 0) {
        auto [a, b] = splitNumber(n);
        result = getValue(a, blinks - 1, cache) + getValue(b, blinks - 1, cache);
    }
    else {
        result = getValue(n * 2024, blinks - 1, cache);
    }
    cache[key] = result;
    return result;
}

unsigned long long part1(std::vector<unsigned long long> numbers, int max_blinks) {
    std::unordered_map<CacheKey, unsigned long long, CacheKeyHash> cache;
    unsigned long long r = 0;
    for(auto n : numbers) {
        r += getValue(n, max_blinks, cache);
    }
    return r;
}

int main() {
    // PART 1
    std::vector<unsigned long long> numbers;
    try {
        std::string filePath = "input.txt";
        numbers = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    unsigned long long result_1 = part1(numbers, 25);
    writeResultToFile(result_1, 1);

    unsigned long long result_2 = part1(numbers, 75);
    writeResultToFile(result_2, 2);
}
