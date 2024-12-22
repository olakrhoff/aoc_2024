#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

#define DEBUG 0

#define PART_1 false

typedef struct coord
{
    int64_t row {}, col {};

    coord(int64_t row, int64_t col) : row(row), col(col)
    {}

    coord() = default;

    bool operator<(const coord &rhs) const
    {
        if (row < rhs.row)
            return true;
        if (rhs.row < row)
            return false;
        return col < rhs.col;
    }

    bool operator>(const coord &rhs) const
    {
        return rhs < *this;
    }

    bool operator<=(const coord &rhs) const
    {
        return !(rhs < *this);
    }

    bool operator>=(const coord &rhs) const
    {
        return !(*this < rhs);
    }

    bool operator==(const coord &rhs) const
    {
        return row == rhs.row &&
               col == rhs.col;
    }

    bool operator!=(const coord &rhs) const
    {
        return !(rhs == *this);
    }

    coord operator-(const coord &rhs) const
    {
        return {row - rhs.row, col - rhs.col};
    }

} coord_t;

vector<pair<coord_t, bool>> parse_line(const string &line, int y_pos)
{
    vector<pair<coord_t, bool>> results {};

    for (int i = 0; i < line.length(); ++i)
    {
        switch (line.at(i))
        {
            case '.':
                break;
            case '#':
                results.emplace_back(make_pair<coord_t, bool>(coord_t(y_pos, i), false));
                break;
            case '^':
                results.emplace_back(make_pair<coord_t, bool>(coord_t(y_pos, i), true));
                break;
            default:
                cout << "Should not reach this, unrecognised char" << endl;
                exit(EXIT_FAILURE);
        }
    }

    return results;
}

typedef enum dir
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} dir_t;

typedef struct guard
{
    coord_t pos;
    dir_t direction = NORTH;

    void step()
    {
        switch (direction)
        {
            case NORTH:
                pos.row--;
                break;
            case EAST:
                pos.col++;
                break;
            case SOUTH:
                pos.row++;
                break;
            case WEST:
                pos.col--;
                break;
        }
    }

    void step_back()
    {
        switch (direction)
        {
            case NORTH:
                pos.row++;
                break;
            case EAST:
                pos.col--;
                break;
            case SOUTH:
                pos.row--;
                break;
            case WEST:
                pos.col++;
                break;
        }
    }

    void rotate()
    {
        direction = (dir_t)((direction + 1) % 4);
    }

    void big_step(const set<coord_t> &obstacles, coord_t corner)
    {
        set<coord_t> subset {};
        coord_t first_hit {};
        switch (direction)
        {
            case NORTH:
                copy_if(obstacles.begin(), obstacles.end(), inserter(subset, subset.begin()), [&](const coord_t &coord)
                {
                    return coord.row < pos.row && coord.col == pos.col;
                });
                if (subset.empty())
                {
                    pos.row = -1;
                    return;
                }
                first_hit = *min_element(subset.begin(), subset.end(), [](const coord_t &a, const coord_t &b)
                {
                   return a.row > b.row;
                });
                pos = {first_hit.row + 1, first_hit.col};
                break;
            case EAST:
                copy_if(obstacles.begin(), obstacles.end(), inserter(subset, subset.begin()), [&](const coord_t &coord)
                {
                    return coord.col > pos.col && coord.row == pos.row;
                });
                if (subset.empty())
                {
                    pos.col = corner.col + 1;
                    return;
                }
                first_hit = *min_element(subset.begin(), subset.end(), [](const coord_t &a, const coord_t &b)
                {
                    return a.col < b.col;
                });
                pos = {first_hit.row, first_hit.col - 1};
                break;
            case SOUTH:
                copy_if(obstacles.begin(), obstacles.end(), inserter(subset, subset.begin()), [&](const coord_t &coord)
                {
                    return coord.row > pos.row && coord.col == pos.col;
                });
                if (subset.empty())
                {
                    pos.row = corner.row + 1;
                    return;
                }
                first_hit = *min_element(subset.begin(), subset.end(), [](const coord_t &a, const coord_t &b)
                {
                    return a.row < b.row;
                });
                pos = {first_hit.row - 1, first_hit.col};
                break;
            case WEST:
                copy_if(obstacles.begin(), obstacles.end(), inserter(subset, subset.begin()), [&](const coord_t &coord)
                {
                    return coord.col < pos.col && coord.row == pos.row;
                });
                if (subset.empty())
                {
                    pos.col = -1;
                    return;
                }
                first_hit = *min_element(subset.begin(), subset.end(), [](const coord_t &a, const coord_t &b)
                {
                    return a.col > b.col;
                });
                pos = {first_hit.row, first_hit.col + 1};
                break;
        }
        this->rotate();
    }

    bool operator==(const guard &rhs) const
    {
        return this->pos == rhs.pos && this->direction == rhs.direction;
    }

} guard_t;

void writeCoordsToFile(const set<coord_t> &coords, const string &filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return;
    }

    for (const auto& c : coords) {
        outFile << c.row << ' ' << c.col << '\n';
    }

    // Always good to close and check for write errors
    outFile.close();
    if (!outFile.good()) {
        cerr << "Error occurred while writing to " << filename << endl;
    }
}

bool is_in_area(coord_t pos, coord_t corner)
{
    //return !(pos.row < 0 || pos.col < 0 || pos.row > corner.row || pos.col > corner.col);
    bool val = !(pos.row < 0 || pos.col < 0 || pos.row > corner.row || pos.col > corner.col);
    //if (val == false)
        //cout << "here" << endl;
    return val;
}

int find_all_coords(guard_t guard, const set<coord_t> &obstacles, coord_t corner)
{
    set<coord_t> visited {};
    while (is_in_area(guard.pos, corner))
    {
        visited.insert(guard.pos);

        guard.step();

        if (obstacles.contains(guard.pos))
        {
            guard.step_back();
            guard.rotate();
        }
    }

    return visited.size();
}

vector<pair<coord_t, dir_t>> get_all_coords(guard_t guard, const set<coord_t> &obstacles, coord_t corner)
{
    vector<pair<coord_t, dir_t>> visited {};
    while (is_in_area(guard.pos, corner))
    {
        visited.emplace_back(guard.pos, guard.direction);

        guard.step();

        if (obstacles.contains(guard.pos))
        {
            guard.step_back();
            guard.rotate();
            visited.pop_back();
        }
    }

    return visited;
}

bool contains_cycle(guard_t guard, const set<coord_t> &obstacles, coord_t corner, set<pair<coord_t, dir_t>> visited)
{
    while (is_in_area(guard.pos, corner))
    {
        if (visited.contains({guard.pos, guard.direction}))
            return true;
        visited.insert({guard.pos, guard.direction});

        guard.big_step(obstacles, corner);
    }

    return false;
}

int find_all_positions_for_obstacle(guard_t guard, const set<coord_t> &obstacles, coord_t corner)
{
    vector<pair<coord_t, dir_t>> route = get_all_coords(guard, obstacles, corner);

    set<coord_t> new_obstacles {obstacles};
    int cycles {};
    set<coord_t> tested {};
    for (int i = 1; i < route.size(); ++i)
    {
        auto coord = route.at(i).first;
        if (tested.contains(coord))
            continue;
        tested.insert(coord);

        new_obstacles.insert(coord);
        guard_t new_guard;
        new_guard.pos = route.at(i - 1).first;
        new_guard.direction = route.at(i - 1).second;
        if (contains_cycle(new_guard, new_obstacles, corner, {}))
            ++cycles;

        new_obstacles.erase(coord);
    }

    return cycles;
}

/*
optional<pair<coord_t, dir_t>> find_coord_to_create_cycle(pair<coord_t, dir_t> first_pos, pair<coord_t, dir_t> last_pos)
{
    optional<pair<coord_t, dir_t>> opt_pos;
    pair<coord_t, dir_t> pos;
    switch (last_pos.second)
    {
        case NORTH:
            if (first_pos.first.row > last_pos.first.row)
                return opt_pos;
            pos.first.row = first_pos.first.row - 1;
            pos.first.col = last_pos.first.col;
            break;
        case EAST:
            if (first_pos.first.col < last_pos.first.col)
                return opt_pos;
            pos.first.col = first_pos.first.col + 1;
            pos.first.row = last_pos.first.row;
            break;
        case SOUTH:
            if (first_pos.first.row < last_pos.first.row)
                return opt_pos;
            pos.first.row = first_pos.first.row + 1;
            pos.first.col = last_pos.first.col;
            break;
        case WEST:
            if (first_pos.first.col > last_pos.first.col)
                return opt_pos;
            pos.first.col = first_pos.first.col - 1;
            pos.first.row = last_pos.first.row;
            break;
    }
    pos.second = (dir_t)((last_pos.second + 1) % 4);

    return pos;
}

int find_all_positions_for_obstacle(guard_t guard, const set<coord_t> &obstacles, coord_t corner)
{
    vector<pair<coord_t, dir_t>> history {};

    set<coord_t> cycle_obstacles {};

    while (is_in_area(guard.pos, corner))
    {
        history.emplace_back(guard.pos, guard.direction);
        if (history.size() == 161)
            cout << "now" << endl;
        if (history.size() >= 4)
        {
            for (int i = 3; history.size() >= i + 1; i += 4)
            {
                optional<pair<coord_t, dir_t>> next_pos = find_coord_to_create_cycle(history.at(history.size() - 1 - i), history.back());
                if (!next_pos.has_value() || !is_in_area(next_pos->first, corner))
                    continue;

                if (next_pos->first == coord_t(12, 8))
                    cout << "kake" << endl;
                cout << "coord: " << next_pos->first.row << ", " << next_pos->first.col << ", " << next_pos->second << endl;
                set<coord_t> new_obstacles = obstacles;
                new_obstacles.insert(next_pos->first);
                guard.big_step(new_obstacles, corner);
                guard.big_step(new_obstacles, corner);

                if (guard.pos == history.at(history.size() - 1 - i + 1).first)
                    cycle_obstacles.insert(next_pos->first);

                guard.pos = history.back().first;
                guard.direction = history.back().second;
            }
        }

        guard.big_step(obstacles, corner);
    }

    writeCoordsToFile(cycle_obstacles, "incorrect.txt");

    return (int)cycle_obstacles.size();
}
*/

int main()
{
    auto start_time = chrono::high_resolution_clock::now();
    //Reading data from file
    string filename = "../data.txt";
    if (DEBUG)
        filename = "../test_data.txt";
    ifstream file;
    
    file.open(filename);
    if (!file.is_open())
        return EXIT_FAILURE;
    
    string temp {};
    set<coord_t> obstacles {};
    guard_t guard {};
    int y_pos {};
    while (!file.eof())
    {
        getline(file, temp);

        for (auto [coord, pos] : parse_line(temp, y_pos))
        {
            if (!pos)
                obstacles.insert(coord);
            else
                guard.pos = coord;
        }
        ++y_pos;
    }

    coord_t corner {(int)temp.length() - 1, y_pos - 1};

    file.close();
    
    // Part 1
    if (PART_1)
    {
        cout << "Number of visited coords: " << find_all_coords(guard, obstacles, corner) << endl;
    }

    // Part 2
    if (!PART_1)
    {
        cout << "Number of positions for extra obstacle: " << find_all_positions_for_obstacle(guard, obstacles, corner) << endl;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Time taken: " << duration.count() / 1000 << " milliseconds" << endl;
    
    return EXIT_SUCCESS;
}
