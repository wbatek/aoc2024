#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::vector<int> readFileLines(const std::string& filePath) {
    std::vector<int> numbers;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    std::getline(file, line);
    file.close();

    for(char c : line) {
        numbers.push_back(c - '0');
    }
    return numbers;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

std::vector<std::string> constructDisk(std::vector<int>& numbers) {
    std::vector<std::string> disk;
    unsigned long long current_id = 0;
    for(size_t i = 0; i < numbers.size(); i++) {
        if(i % 2 == 0) {
            for(int j = 0; j < numbers[i]; j++) {
                disk.push_back(std::to_string(current_id));
            }
            current_id++;
        }
        else {
            for(int j = 0; j < numbers[i]; j++) {
                disk.push_back(".");
            }
        }
    }
    return disk;
}

std::vector<std::string> changeOrder(std::vector<std::string>& disk) {
    size_t left = 0, right = disk.size() - 1;
    while(left < right) {
        while(disk[left] != ".") left++;
        while(disk[right] == ".") right--;
        if(left >= right) break;
        
        std::swap(disk[left], disk[right]);
        left++;
        right--;
    }
    return disk;
}

void swapBlocks(std::vector<std::string>& vec, size_t start1, size_t end1, size_t start2, size_t end2) {
    std::vector<std::string> temp1(vec.begin() + start1, vec.begin() + end1 + 1);
    std::vector<std::string> temp2(vec.begin() + start2, vec.begin() + end2 + 1);
    
    for (size_t i = 0; i < temp1.size(); i++) {
        vec[start1 + i] = temp2[i];
        vec[start2 + i] = temp1[i];
    }
}

std::pair<size_t, size_t> findFirstSpace(std::vector<std::string>& disk, size_t length) {
    size_t left = 0;
    while(left < disk.size()) {
        while(left < disk.size() && disk[left] != ".") left++;
        if(left >= disk.size()) {
            return std::make_pair(-1, -1);
        }
        
        size_t l = left;
        while(left < disk.size() && disk[left] == ".") left++;
        size_t r = left - 1;
        
        if(r - l + 1 >= length) {
            return std::make_pair(l, l + length - 1);
        }
    }
    return std::make_pair(-1, -1);
}

std::vector<std::string> changeOrderFullBlocks(std::vector<std::string>& disk) {
    size_t right = disk.size() - 1;
    while(right >= 0) {
        while(right > 0 && disk[right] == ".") right--;
        if(right == 0) {
            break;
        }
        std::string currentBlockId = disk[right];
        size_t end_idx = right;
        
        while(right > 0 && disk[right] == currentBlockId) right--;
        size_t start_idx = right + 1;
        size_t block_length = end_idx - start_idx + 1;
        
        auto [l, r] = findFirstSpace(disk, block_length);
        if(l == -1) {
            continue;
        }
        if(l >= start_idx || r >= end_idx) continue;
        swapBlocks(disk, l, r, start_idx, end_idx);
    }
    return disk;
}

unsigned long long calculateChecksum(std::vector<std::string>& disk) {
    unsigned long long sum = 0;
    for(unsigned long long i = 0; i < disk.size(); i++) {
        if(disk[i] != ".") {
            unsigned long long value = std::stoull(disk[i]);
            sum += value * i;
        }
    }
    return sum;
}

int main() {
    std::vector<int> numbers;
    try {
        std::string filePath = "input.txt";
        numbers = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::vector<std::string> disk = constructDisk(numbers);
    std::vector<std::string> disk_p1 = changeOrder(disk);
    unsigned long long result_1 = calculateChecksum(disk_p1);
    writeResultToFile(result_1, 1);
    
    std::vector<std::string> disk_p2 = changeOrderFullBlocks(disk);
    unsigned long long result_2 = calculateChecksum(disk_p2);
    writeResultToFile(result_2, 2);
    
}
