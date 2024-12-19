#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

void readFileLines(const std::string& filePath, std::vector<std::string>& possible, std::vector<std::string>& expected) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;

    if(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, ',')) {
            possible.push_back(trim(item));
        }
    }

    std::getline(file, line);

    while(std::getline(file, line)) {
        if(line == "") break;
        expected.push_back(line);
    }
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

int findMatch_p1(const std::string& current, const std::vector<std::string>& possible) {
    std::unordered_set<std::string> wordSet(possible.begin(), possible.end());
    std::unordered_set<int> visited;
    std::queue<int> q;

    q.push(0);

    while (!q.empty()) {
        int start = q.front();
        q.pop();

        if (visited.count(start)) continue;
        visited.insert(start);

        for (int end = start + 1; end <= current.size(); ++end) {
            std::string substring = current.substr(start, end - start);
            if (wordSet.count(substring)) {
                if (end == current.size()) {
                    return 1;
                }
                q.push(end);
            }
        }
    }
    return 0;
}



int part1(std::vector<std::string>& possible, std::vector<std::string>& expected) {
    int correct = 0;
    for(std::string& current : expected) {
        int r = findMatch_p1(current, possible) != 0 ? 1 : 0;
        correct += r;
    }
    return correct;
}

unsigned long long findMatch_p2(std::string current, std::vector<std::string>& possible, std::unordered_map<std::string, unsigned long long>& cache) {
    if(cache.count(current)) {
        return cache[current];
    }

    if(current.empty()) return 1;
    unsigned long long count = 0;

    for (int i = 1; i <= current.size(); ++i) {
        std::string prefix = current.substr(0, i);
        if (std::find(possible.begin(), possible.end(), prefix) != possible.end()) {
            count += findMatch_p2(current.substr(i), possible, cache);
        }
    }
    cache[current] = count;
    return count;
}

unsigned long long part2(std::vector<std::string>& possible, std::vector<std::string>& expected) {
    std::unordered_map<std::string, unsigned long long> cache;
    unsigned long long correct = 0;
    for(std::string& current : expected) {
        unsigned long long r = findMatch_p2(current, possible, cache);
        correct += r;
    }
    return correct;
}

int main() {
    std::vector<std::string> possible;
    std::vector<std::string> expected;
    try {
        std::string filePath = "input.txt";
        readFileLines(filePath, possible, expected);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int result_1 = part1(possible, expected);
    writeResultToFile(result_1, 1);

    unsigned long long result_2 = part2(possible, expected);
    writeResultToFile(result_2, 2);
}