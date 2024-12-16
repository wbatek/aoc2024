#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include <set>

#include <string>
#include <set>
#include <sstream>
#include <utility>

std::string serializeSet(const std::set<std::pair<int, int>>& s) {
    std::ostringstream oss;
    for (const auto& p : s) {
        oss << "(" << p.first << "," << p.second << ")";
    }
    return oss.str();
}


std::vector<std::vector<char> > readFileLines(const std::string& filePath) {
    std::vector <std::vector<char> > grid;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open the file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }
    return grid;
}

template <typename T>
void writeResultToFile(T result, int taskNumber) {
    std::string filename = "output" + std::to_string(taskNumber) + ".txt";
    std::ofstream outFile(filename);
    outFile << result;
    outFile.close();
}

template<char X>
std::pair<int, int> findOnGrid(std::vector<std::vector<char> >& grid) {
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == X) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

int dijkstra(const std::vector<std::vector<char> >& grid, std::pair<int, int> start) {
    int rows = grid.size();
    int cols = grid[0].size();

    std::priority_queue<
            std::tuple<int, int, int, int, int>,
            std::vector<std::tuple<int, int, int, int, int>>,
            std::function<bool(const std::tuple<int, int, int, int, int>&, const std::tuple<int, int, int, int, int>&)>
    > pq([](const std::tuple<int, int, int, int, int>& a, const std::tuple<int, int, int, int, int>& b) {
        return std::get<0>(a) > std::get<0>(b);
    });
    pq.push(std::make_tuple(0, start.first, start.second, 0, 1));

    std::set<std::tuple<int, int, int, int> > seen;

    while(!pq.empty()) {
        auto element = pq.top();
        pq.pop();
        int cost = std::get<0>(element);
        int nr = std::get<1>(element);
        int nc = std::get<2>(element);
        int dr = std::get<3>(element);
        int dc = std::get<4>(element);
        if(grid[nr][nc] == '#') continue;
        std::tuple<int, int, int, int> target = std::make_tuple(nr, nc, dr, dc);
        if(seen.count(target) > 0) continue;

        seen.insert(std::make_tuple(nr, nc, dr, dc));

        if(grid[nr][nc] == 'E') return cost;

        pq.push(std::make_tuple(cost + 1, nr + dr, nc + dc, dr, dc));
        pq.push(std::make_tuple(cost + 1000, nr, nc, dc, -dr));
        pq.push(std::make_tuple(cost + 1000, nr, nc, -dc, dr));
    }
    return -1;
}

int part1(std::vector<std::vector<char> >& grid) {
    std::pair<int, int> initialPosition = findOnGrid<'S'>(grid);
    return dijkstra(grid, initialPosition);
}

int dijkstra_p2(const std::vector<std::vector<char> >& grid, std::pair<int, int> start) {
    int rows = grid.size();
    int cols = grid[0].size();
    int best_val = dijkstra(grid, start);

    auto cmp = [](const std::tuple<int, int, int, int, int, std::set<std::pair<int, int>>>& a,
                  const std::tuple<int, int, int, int, int, std::set<std::pair<int, int>>>& b) {
        return std::get<0>(a) > std::get<0>(b);
    };

    std::set<std::pair<int, int>> goodPoints;

    std::priority_queue<
            std::tuple<int, int, int, int, int, std::set<std::pair<int, int>>>,
            std::vector<std::tuple<int, int, int, int, int, std::set<std::pair<int, int>>>>,
            decltype(cmp)>
            pq(cmp);

    std::set<std::pair<int, int>> first_set;
    pq.push(std::make_tuple(0, start.first, start.second, 0, 1, first_set));

    // std::set<std::tuple<int, int, int, int> > seen;
    std::set<std::tuple<int, int, int, int, std::size_t>> seen;

    while(!pq.empty()) {
        auto element = pq.top();
        pq.pop();
        int cost = std::get<0>(element);
        int nr = std::get<1>(element);
        int nc = std::get<2>(element);
        int dr = std::get<3>(element);
        int dc = std::get<4>(element);
        std::set<std::pair<int, int> > current_set = std::get<5>(element);

        if(grid[nr][nc] == '#') continue;
        if(cost > best_val) continue;

        std::size_t set_hash = std::hash<std::string>()(serializeSet(current_set));
        std::tuple<int, int, int, int, std::size_t> target = std::make_tuple(nr, nc, dr, dc, set_hash);
        if (seen.count(target) > 0) continue;

        seen.insert(target);

        if (grid[nr][nc] == 'E' && cost == best_val) {
            goodPoints.insert(current_set.begin(), current_set.end());
            continue;
        }
        current_set.insert(std::make_pair(nr, nc));
        std::set<std::pair<int, int>> copy1 = current_set;
        std::set<std::pair<int, int>> copy2 = current_set;
        std::set<std::pair<int, int>> copy3 = current_set;

        pq.push(std::make_tuple(cost + 1, nr + dr, nc + dc, dr, dc, std::move(copy1)));
        pq.push(std::make_tuple(cost + 1000, nr, nc, dc, -dr, std::move(copy2)));
        pq.push(std::make_tuple(cost + 1000, nr, nc, -dc, dr, std::move(copy3)));
    }
    return goodPoints.size() + 1;
}

int part2(std::vector<std::vector<char> >& grid) {
    std::pair<int, int> initialPosition = findOnGrid<'S'>(grid);
    return dijkstra_p2(grid, initialPosition);
}

int main() {
    std::vector<std::vector<char> > grid;
    try {
        std::string filePath = "input.txt";
        grid = readFileLines(filePath);
    }
    catch (const std::ios_base::failure &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    int result_1 = part1(grid);
    writeResultToFile(result_1, 1);

    int result_2 = part2(grid);
    writeResultToFile(result_2, 2);
}