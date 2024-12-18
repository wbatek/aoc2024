#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>
#include <cmath>

using ull = unsigned long long;

void readFileLines(const std::string& filePath, std::vector<ull>& registers, std::vector<ull>& program) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::regex numberRegex("\\d+");
    std::string line;

    for (int i = 0; i < 3; ++i) {
        if (std::getline(file, line)) {
            std::smatch match;
            while (std::regex_search(line, match, numberRegex)) {
                registers.push_back(std::stoull(match.str()));
                line = match.suffix().str();
            }
        }
    }
    std::getline(file, line);

    if (std::getline(file, line)) {
        std::smatch match;
        while (std::regex_search(line, match, numberRegex)) {
            program.push_back(std::stoull(match.str()));
            line = match.suffix().str();
        }
    }
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

ull combo(ull operand, std::vector<ull>& registers) {
    if(operand <= 3) {
        return operand;
    }
    else {
        return registers[operand - 4];
    }
}

std::vector<ull> part1(std::vector<ull> registers, std::vector<ull> program) {
    std::vector<ull> result_string;
    ull instruction_pointer = 0;
    while(instruction_pointer < program.size()) {
        ull operation = program[instruction_pointer];
        ull operand = program[instruction_pointer + 1];

        if(operation == 0) {
            ull numerator = registers[0];
            ull denumerator = std::pow(2, combo(operand, registers));
            ull result = numerator / denumerator;
            registers[0] = result;
            instruction_pointer += 2;
        }
        else if(operation == 1) {
            ull first = registers[1];
            ull second = operand;
            ull result = first ^ second;
            registers[1] = result;
            instruction_pointer += 2;
        }
        else if(operation == 2) {
            ull first = combo(operand, registers) % 8;
            registers[1] = first;
            instruction_pointer += 2;
        }
        else if(operation == 3) {
            if(registers[0] != 0) {
                instruction_pointer = operand;
            }
            else {
                instruction_pointer += 2;
            }
        }
        else if(operation == 4) {
            ull first = registers[1];
            ull second = registers[2];
            ull result = first ^ second;
            registers[1] = result;
            instruction_pointer += 2;
        }
        else if(operation == 5) {
            ull result = combo(operand, registers) % 8;
            result_string.push_back(result);
            instruction_pointer += 2;
        }
        else if(operation == 6) {
            ull numerator = registers[0];
            ull denumerator = std::pow(2, combo(operand, registers));
            ull result = numerator / denumerator;
            registers[1] = result;
            instruction_pointer += 2;
        }
        else if(operation == 7) {
            ull numerator = registers[0];
            ull denumerator = std::pow(2, combo(operand, registers));
            ull result = numerator / denumerator;
            registers[2] = result;
            instruction_pointer += 2;
        }
    }

    std::ostringstream oss;
    for (size_t i = 0; i < result_string.size(); ++i) {
        oss << result_string[i];
        if (i != result_string.size() - 1) {
            oss << ",";
        }
    }

    return result_string;
}

ull part2(std::vector<ull>& registers, std::vector<ull>& program) {
    ull ans = 0;
    std::vector<ull> out;
    int i = program.size();
    while(program.size() != out.size()) {
        for(size_t t = 0; t < 8; t++) {
            registers[0] = (ans << 3) + t;
            registers[1] = registers[0] % 8;
            registers[1] ^= 1;
            registers[2] = registers[0] >> registers[1];
            registers[1] ^= registers[2];
            registers[1] ^= 4;
            if(registers[1] % 8 == program[i - 1]) {
                out.insert(out.begin(), registers[1] % 8);
                i--;
                ans = registers[0];
                break;
            }
        }
    }
    return ans;
}

int main() {
    std::vector<ull> registers;
    std::vector<ull> program;
    try {
        std::string filePath = "input.txt";
        readFileLines(filePath, registers, program);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::vector<ull> result_1 = part1(registers, program);
    std::ostringstream oss;
    for (size_t i = 0; i < result_1.size(); ++i) {
        oss << result_1[i];
        if (i != result_1.size() - 1) {
            oss << ",";
        }
    }
    writeResultToFile(oss.str(), 1);
    for(auto& x : registers) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    for(auto& x : program) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    ull result_2 = part2(registers, program);
    writeResultToFile(result_2, 2);
}




