#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct Data {
    unsigned long long target;
    std::vector<unsigned long long> values;
};

enum Operation {
    ADD = 0,
    MUL = 1,
    CONCAT = 2
};

std::vector<std::string> readFileLines(const std::string& filePath) {
    std::vector<std::string> lines;
    std::ifstream file(filePath);

    if (!file.is_open()) throw std::ios_base::failure("Failed to open the file: " + filePath);
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

Data divideLine(std::string& line) {
    int colonPos = line.find(":");
    unsigned long long target = std::stoul(line.substr(0, colonPos));
    std::string rest = line.substr(colonPos + 1);
    std::vector<unsigned long long> values;
    std::istringstream stream(rest);
    std::string temp;
    while(stream >> temp) {
        unsigned long long v = std::stoull(temp);
        values.push_back(v);
    }
    return {target, values};
}

unsigned long long part1(std::vector<std::string>& lines) {
    unsigned long long sum = 0;
    for(std::string line : lines) {
        Data data = divideLine(line);
        unsigned long long allPossibilities = 1UL << (data.values.size() - 1);

        for(unsigned long long i = 0; i <= allPossibilities; i++) {
            unsigned long long result = data.values[0];

            for(unsigned long long j = 0; j < data.values.size() - 1; j++) {
                unsigned long long operation = (i >> j) & 1;
                if(operation == Operation::MUL) {
                    result *= data.values[j + 1];
                }
                else {
                    result += data.values[j + 1];
                }
            }

            if(data.target == result) {
                sum += data.target;
                break;
            }
        }
    }
    return sum;
}

int numDigits(unsigned long long x) {
    int digits = 0;
    while(x > 0) {
        x /= 10;
        digits++;
    }
    return digits;
}

unsigned long long part2(std::vector<std::string>& lines) {
    unsigned long long sum = 0;
    for(std::string line : lines) {
        Data data = divideLine(line);
        unsigned long long allPossibilities = 1UL << ((data.values.size() - 1) * 2);

        for(unsigned long long i = 0; i <= allPossibilities; i++) {
            unsigned long long result = data.values[0];

            for(unsigned long long j = 0; j < data.values.size() - 1; j++) {
                unsigned long long operation = (i >> j * 2) & 3;
                if(operation == Operation::MUL) {
                    result *= data.values[j + 1];
                }
                else if(operation == Operation::ADD) {
                    result += data.values[j + 1];
                }
                else {
                    int next_digits = numDigits(data.values[j + 1]);
                    result = result * static_cast<unsigned long long>(std::pow(10, next_digits)) + data.values[j + 1];
                }
            }

            if(data.target == result) {
                sum += data.target;
                break;
            }
        }
    }
    return sum;
}




int main() {
    // PART 1
    std::vector<std::string> lines;
    try {
        std::string filePath = "input.txt";
        lines = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    unsigned long long result_1 = part1(lines);
    writeResultToFile(result_1, 1);

    unsigned long long result_2 = part2(lines);
    writeResultToFile(result_2, 2);
}