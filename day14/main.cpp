#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>

int WIDTH = 101;
int HEIGHT = 103;

class Robot {
    int x;
    int y;
    int v_x;
    int v_y;

public:
    Robot(int x_, int y_, int v_x_, int v_y_) : x(x_), y(y_), v_x(v_x_), v_y(v_y_) {}

    int get_x() {
        return x;
    }

    int get_y() {
        return y;
    }

    int get_v_x() {
        return v_x;
    }

    int get_v_y() {
        return v_y;
    }

    void move() {
        x = (x + v_x) % WIDTH;
        y = (y + v_y) % HEIGHT;

        if (x < 0) {
            x += WIDTH;
        }
        if (y < 0) {
            y += HEIGHT;
        }
    }
};

std::vector<std::vector<char> > generateGrid(std::vector<Robot>& robots) {
    std::vector<std::vector<char> > grid(HEIGHT, std::vector<char>(WIDTH, '.'));
    for(auto& r : robots) {
        grid[r.get_x()][r.get_y()] = '#';
    }
    return grid;
}

bool checkHorizontalLineOf20(std::vector<std::vector<char> >& grid) {
    for (int row = 0; row < HEIGHT; row++) {
        int count = 0;
        for (int col = 0; col < WIDTH; col++) {
            if (grid[row][col] == '#') {
                count++;
                if (count == 31) {
                    return true;
                }
            } else {
                count = 0;
            }
        }
    }
    return false;
}

std::vector<Robot> readFileLines(const std::string& filePath) {
    std::vector<Robot> robots;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while(std::getline(file, line)) {
        std::vector<int> numbers;
        std::regex number_regex("-?\\d+");
        std::sregex_iterator it(line.begin(), line.end(), number_regex);
        std::sregex_iterator end;

        while (it != end) {
            numbers.push_back(std::stoi(it->str()));
            ++it;
        }
        Robot x(numbers[0], numbers[1], numbers[2], numbers[3]);
        robots.push_back(x);
    }

    file.close();
    return robots;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

int calculateSafetyFactor(std::vector<Robot>& robots) {
    int f=0, s=0, t=0, fth=0;
    for(Robot& robot : robots) {
        if(robot.get_x() < WIDTH / 2 && robot.get_y() < HEIGHT / 2) f++;
        else if(robot.get_x() < WIDTH / 2 && robot.get_y() > HEIGHT / 2) t++;
        else if(robot.get_x() > WIDTH / 2 && robot.get_y() < HEIGHT / 2) s++;
        else if(robot.get_x() > WIDTH / 2 && robot.get_y() > HEIGHT / 2) fth++;
    }
    return f * s * t * fth;
}

int part1(std::vector<Robot> robots) {
    for(int i = 0; i < 100; i++) {
        for(Robot& robot : robots) {
            robot.move();
        }
    }

    int result = calculateSafetyFactor(robots);
    return result;
}

int part2(std::vector<Robot>& robots) {
    int i = 1;
    while(true) {
        for(Robot& robot : robots) {
            robot.move();
        }
        std::vector<std::vector<char> > grid = generateGrid(robots);
        if(checkHorizontalLineOf20(grid)) {
            return i;
        }
        i++;
    }
}

int main() {
    // PART 1
    std::vector<Robot> robots;
    try {
        std::string filePath = "input.txt";
        robots = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    int result_1 = part1(robots);
    writeResultToFile(result_1, 1);

    int result_2 = part2(robots);
    writeResultToFile(result_2, 2);
}