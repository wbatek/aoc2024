#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

enum Move {
    UP = '^',
    DOWN = 'v',
    LEFT = '<',
    RIGHT = '>'
};

struct MoveHash {
    std::size_t operator()(const Move& move) const {
        return std::hash<int>()(static_cast<int>(move));
    }
};

const std::unordered_map<Move, std::pair<int, int>, MoveHash> createDirectionMap() {
    std::unordered_map<Move, std::pair<int, int>, MoveHash> map;
    map[UP] = std::make_pair(-1, 0);
    map[DOWN] = std::make_pair(1, 0);
    map[LEFT] = std::make_pair(0, -1);
    map[RIGHT] = std::make_pair(0, 1);
    return map;
}

std::pair<std::vector<std::vector<char> >, std::vector<Move> > readFileLines(const std::string& filePath) {
    std::vector<std::vector<char> > grid;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while(std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }

    std::vector<Move> moves;

    while (std::getline(file, line)) {
        for (char c : line) {
            switch (c) {
                case '^':
                    moves.push_back(UP);
                    break;
                case 'v':
                    moves.push_back(DOWN);
                    break;
                case '<':
                    moves.push_back(LEFT);
                    break;
                case '>':
                    moves.push_back(RIGHT);
                    break;
                default:
                    std::cerr << "Invalid character: " << c << std::endl;
                    break;
            }
        }
    }


    file.close();
    return std::make_pair(grid, moves);
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

std::pair<int, int> localizeRobot(std::vector<std::vector<char> >& grid) {
    for(int r = 0; r < grid.size(); r++) {
        for(int c = 0; c < grid[r].size(); c++) {
            if(grid[r][c] == '@') {
                return std::make_pair(r, c);
            }
        }
    }
    return std::make_pair(-1, -1);
}

void moveRobot(std::vector<std::vector<char> >& grid, std::vector<Move>& moves) {
    const auto& moveMap = createDirectionMap();
    for(auto& move : moves) {
        auto [r, c] = localizeRobot(grid);
        int new_r = r + moveMap.at(move).first;
        int new_c = c + moveMap.at(move).second;
        if(grid[new_r][new_c] == '#') {}
        else if(grid[new_r][new_c] == '.') {
            grid[r][c] = '.';
            r = new_r;
            c = new_c;
            grid[r][c] = '@';
        }
        else if(grid[new_r][new_c] == 'O') {
            int move_r = new_r;
            int move_c = new_c;
            while(grid[move_r][move_c] == 'O') {
                move_r += moveMap.at(move).first;
                move_c += moveMap.at(move).second;
            }
            if(grid[move_r][move_c] == '#') {}
            else {
                grid[move_r][move_c] = 'O';
                grid[new_r][new_c] = '@';
                grid[r][c] = '.';
            }
        }
    }
}

template<char X>
int calculateDistances(const std::vector<std::vector<char> >& grid) {
    int result = 0;
    for(int r = 0; r < grid.size(); r++) {
        for(int c = 0; c < grid[r].size(); c++) {
            if(grid[r][c] == X) {
                result += 100 * r + c;
            }
        }
    }
    return result;
}

int part1(std::vector<std::vector<char> >& grid, std::vector<Move>& moves) {
    moveRobot(grid, moves);
    int result = calculateDistances<'O'>(grid);
    return result;
}

std::vector<std::vector<char> > widenGrid(std::vector<std::vector<char> >& grid) {
    std::vector<std::vector<char> > newGrid;
    newGrid.resize(grid.size());
    for(int r = 0; r < grid.size(); r++) {
        newGrid[r].reserve(grid[r].size() * 2);
        for(int c = 0; c < grid[r].size(); c++) {
            if(grid[r][c] == '#') {
                newGrid[r].push_back('#');
                newGrid[r].push_back('#');
            }
            else if(grid[r][c] == 'O') {
                newGrid[r].push_back('[');
                newGrid[r].push_back(']');
            }
            else if(grid[r][c] == '.') {
                newGrid[r].push_back('.');
                newGrid[r].push_back('.');
            }
            else {
                newGrid[r].push_back('@');
                newGrid[r].push_back('.');
            }
        }
    }
    return newGrid;
}

void moveRobotPart2(std::vector<std::vector<char> >& grid, std::vector<Move>& moves) {
    const auto& moveMap = createDirectionMap();
    for(auto& move : moves) {
        auto [r, c] = localizeRobot(grid);
        int new_r = r + moveMap.at(move).first;
        int new_c = c + moveMap.at(move).second;
        if(grid[new_r][new_c] == '#') {}
        else if(grid[new_r][new_c] == '.') {
            grid[r][c] = '.';
            r = new_r;
            c = new_c;
            grid[r][c] = '@';
        }
        else if(grid[new_r][new_c] == '[') {
            if(move == RIGHT) {
                int move_r = new_r;
                int move_c = new_c;
                while(grid[move_r][move_c] == '[') {
                    move_r += 2 * moveMap.at(move).first;
                    move_c += 2 * moveMap.at(move).second;
                }
                if(grid[move_r][move_c] == '.') {
                    while(true) {
                        move_c -= moveMap.at(move).second;
                        std::swap(grid[move_r][move_c + moveMap.at(move).second], grid[move_r][move_c]);
                        if(move_c == c) {
                            break;
                        }
                    }
                }
                else if(grid[move_r][move_c] == '#') {}
            }
            else {
                std::vector<std::pair<int, int> > targets;
                targets.push_back(std::make_pair(r,c));
                bool go = true;
                for (size_t i = 0; i < targets.size(); ++i) {
                    auto& [cr, cc] = targets[i];
                    int nr = cr + moveMap.at(move).first;
                    int nc = cc + moveMap.at(move).second;
                    if (std::find(targets.begin(), targets.end(), std::make_pair(nr, nc)) != targets.end()) {
                        continue;
                    }
                    if(grid[nr][nc] == '#') {
                        go = false;
                        break;
                    }
                    else if(grid[nr][nc] == '[') {
                        targets.push_back(std::make_pair(nr, nc));
                        targets.push_back(std::make_pair(nr, nc + 1));
                    }
                    else if(grid[nr][nc] == ']') {
                        targets.push_back(std::make_pair(nr, nc));
                        targets.push_back(std::make_pair(nr, nc - 1));
                    }
                }
                if(!go) continue;
                std::vector<std::vector<char> > copy = grid;
                grid[r][c] = '.';
                grid[r + moveMap.at(move).first][c + moveMap.at(move).second] = '@';
                for(int i = 2; i < targets.size(); i++) {
                    auto p = targets[i];
                    grid[p.first][p.second] = '.';
                }
                for(int i = 1; i < targets.size(); i++) {
                    auto p = targets[i];
                    grid[p.first + moveMap.at(move).first][p.second + moveMap.at(move).second] = copy[p.first][p.second];
                }
            }
        }
        else if(grid[new_r][new_c] == ']') {
            if(move == LEFT) {
                int move_r = new_r;
                int move_c = new_c;
                while(grid[move_r][move_c] == ']') {
                    move_r += 2 * moveMap.at(move).first;
                    move_c += 2 * moveMap.at(move).second;
                }
                if(grid[move_r][move_c] == '.') {
                    while(true) {
                        move_c -= moveMap.at(move).second;
                        std::swap(grid[move_r][move_c + moveMap.at(move).second], grid[move_r][move_c]);
                        if(move_c == c) {
                            break;
                        }
                    }
                }
                else if(grid[move_r][move_c] == '#') {}
            }
            else {
                std::vector<std::pair<int, int> > targets;
                targets.push_back(std::make_pair(r,c));
                bool go = true;
                for (size_t i = 0; i < targets.size(); ++i) {
                    auto& [cr, cc] = targets[i];
                    int nr = cr + moveMap.at(move).first;
                    int nc = cc + moveMap.at(move).second;
                    if (std::find(targets.begin(), targets.end(), std::make_pair(nr, nc)) != targets.end()) {
                        continue;
                    }
                    if(grid[nr][nc] == '#') {
                        go = false;
                        break;
                    }
                    else if(grid[nr][nc] == '[') {
                        targets.push_back(std::make_pair(nr, nc));
                        targets.push_back(std::make_pair(nr, nc + 1));
                    }
                    else if(grid[nr][nc] == ']') {
                        targets.push_back(std::make_pair(nr, nc));
                        targets.push_back(std::make_pair(nr, nc - 1));
                    }
                }
                if(!go) continue;
                std::vector<std::vector<char> > copy = grid;
                grid[r][c] = '.';
                grid[r + moveMap.at(move).first][c + moveMap.at(move).second] = '@';
                for(int i = 2; i < targets.size(); i++) {
                    auto p = targets[i];
                    grid[p.first][p.second] = '.';
                }
                for(int i = 1; i < targets.size(); i++) {
                    auto p = targets[i];
                    grid[p.first + moveMap.at(move).first][p.second + moveMap.at(move).second] = copy[p.first][p.second];
                }
            }
        }
    }
}

int part2(std::vector<std::vector<char> >& grid, std::vector<Move>& moves) {
    moveRobotPart2(grid, moves);
    int result = calculateDistances<'['>(grid);
    return result;
}

int main() {
    // PART 1
    std::vector<std::vector<char> > grid;
    std::vector<Move> moves;
    try {
        std::string filePath = "input.txt";
        auto result = readFileLines(filePath);
        grid = result.first;
        moves = result.second;
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::vector<std::vector<char> > copyGrid = grid;

    int result_1 = part1(grid, moves);
    writeResultToFile(result_1, 1);

    std::vector<std::vector<char> > widenedGrid = widenGrid(copyGrid);
    int result_2 = part2(widenedGrid, moves);
    writeResultToFile(result_2, 2);
}
