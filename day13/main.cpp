#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <numeric>

using ull = unsigned long long;

struct Machine {
    std::pair<ull, ull> A;
    std::pair<ull, ull> B;
    std::pair<ull, ull> prize;

    Machine(std::pair<ull, ull> A, std::pair<ull, ull> B, std::pair<ull, ull> C) : A(A), B(B), prize(C){}
};

std::pair<ull, ull> extractNumbers(const std::string& input) {
    std::pair<ull, ull> result;
    std::regex numberPattern("(\\d+)");
    std::smatch matches;

    auto begin = std::sregex_iterator(input.begin(), input.end(), numberPattern);
    auto end = std::sregex_iterator();

    int count = 0;
    for (auto it = begin; it != end; ++it) {
        if (count == 0) {
            result.first = std::stoi(it->str());
        } else if (count == 1) {
            result.second = std::stoi(it->str());
        }
        count++;
        if (count == 2) break;
    }

    return result;
}

std::vector<Machine> readFileLines(const std::string& filePath) {
    std::vector<Machine> machines;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line1, line2, line3, line4;
    while(std::getline(file, line1)) {
        if (std::getline(file, line2) && std::getline(file, line3) && std::getline(file, line4)) {}
        machines.push_back(Machine(extractNumbers(line1), extractNumbers(line2), extractNumbers(line3)));
    }

    file.close();
    return machines;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

unsigned long long part1(std::vector<Machine>& machines) {
    unsigned long long tokens = 0;
    for (auto &machine: machines) {
        unsigned long long min_tokens = ULLONG_MAX;
        for (int i = 0; i <= 100; i++) {
            for (int j = 0; j <= 100; j++) {
                bool satisfies_first = (machine.A.first * i + machine.B.first * j == machine.prize.first);
                bool satisfies_second = (machine.A.second * i + machine.B.second * j == machine.prize.second);
                if (satisfies_first && satisfies_second) {
                    unsigned long long cost = 3 * i + j;
                    min_tokens = std::min(min_tokens, cost);
                }
            }
        }
        if (min_tokens == ULLONG_MAX) continue;
        tokens += min_tokens;
    }
    return tokens;
}

std::vector<Machine> reconstructMachines(std::vector<Machine>& machines) {
    std::vector<Machine> new_machines;
    for(auto& machine : machines) {
        Machine new_machine(machine.A, machine.B, std::make_pair(machine.prize.first + 10000000000000, machine.prize.second + 10000000000000));
        new_machines.push_back(new_machine);
    }
    return new_machines;
}

unsigned long long part2(std::vector<Machine>& machines) {
    unsigned long long tokens = 0;
    for (auto& machine : machines) {
        long long denominator = machine.A.first * machine.B.second - machine.A.second * machine.B.first;

        if (denominator == 0) continue;

        long long ca_num = (machine.prize.first * machine.B.second - machine.prize.second * machine.B.first);
        if (ca_num % denominator != 0) continue;
        long long ca = ca_num / denominator;

        long long cb_num = (machine.prize.first - machine.A.first * ca);
        if (cb_num % machine.B.first != 0) continue;
        long long cb = cb_num / machine.B.first;

        if (ca >= 0 && cb >= 0) {
            tokens += static_cast<unsigned long long>(ca) * 3 + static_cast<unsigned long long>(cb);
        }
    }
    return tokens;
}


int main() {
    std::vector<Machine> machines;
    try {
        std::string filePath = "input.txt";
        machines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    unsigned long long result_1 = part1(machines);
    writeResultToFile(result_1, 1);

    machines = reconstructMachines(machines);
    unsigned long long result_2 = part2(machines);
    writeResultToFile(result_2, 2);
}

// 71493195288102
// 71493195288102
