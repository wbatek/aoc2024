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

int findSum(std::string line, std::regex _regex) {
    std::sregex_iterator it(line.begin(), line.end(), _regex);
    std::sregex_iterator end;

    int sum = 0;
    bool enable = true;
    while (it != end) {
        std::smatch match = *it;
        std::string mulMatch = match.str();
        if(mulMatch == "do()") {
            enable = true;
        }
        else if(mulMatch == "don't()") {
            enable = false;
        }
        else {
            if (enable) {
                size_t startPos = mulMatch.find('(') + 1;
                size_t endPos = mulMatch.find(')');
                std::string nums = mulMatch.substr(startPos, endPos - startPos);
                size_t commaPos = nums.find(',');
                int first = std::stoi(nums.substr(0, commaPos));
                int second = std::stoi(nums.substr(commaPos + 1));
                sum += first * second;
            }
        }
        ++it;
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
    std::regex regex_part1("mul\\(\\d{1,3},\\d{1,3}\\)");
    int result_1 = findSum(line, regex_part1);
    writeResultToFile(result_1, 1);
    // END PART 1

    // PART 2
    std::regex regex_part2("mul\\(\\d{1,3},\\d{1,3}\\)|do\\(\\)|don't\\(\\)");
    int result_2 = findSum(line, regex_part2);
    writeResultToFile(result_2, 2);
    // END PART 2
}
