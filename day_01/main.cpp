#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

#define DEBUG 0

#define PART_1 false

string trim(string line)
{
    while (line.at(0) == ' ')
        line = line.substr(1);
    while (line.at(line.size() - 1) == ' ')
        line = line.substr(0, line.size() - 1);

    return line;
}

vector<string> split(const string &line, const char delimiter)
{
    vector<string> res {};

    string temp = line;
    while (temp.find(delimiter) != -1)
    {
        res.emplace_back(temp.substr(0, temp.find(delimiter)));
        temp = temp.substr(temp.find(delimiter) + 1);
        temp = trim(temp);
    }
    res.emplace_back(temp);

    return res;
}

pair<int, int> parse_line(const string &line)
{
    vector<string> strings = split(line, ' ');

    return {stoi(strings[0]), stoi(strings[1])};
}

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


    vector<int> list1 {}, list2 {};
    map<int, int> map1 {}, map2 {};

    while (!file.eof())
    {
        getline(file, temp);
        auto vals = parse_line(temp);
        if (PART_1)
        {
            list1.emplace_back(vals.first);
            list2.emplace_back(vals.second);
        }
        else
        {
            if (map1.contains(vals.first))
                map1.at(vals.first)++;
            else
                map1.insert({vals.first, 1});

            if (map2.contains(vals.second))
                map2.at(vals.second)++;
            else
                map2.insert({vals.second, 1});
        }
    }
    
    file.close();

    // Part 1
    if (PART_1)
    {
        std::sort(list1.begin(), list1.end());
        std::sort(list2.begin(), list2.end());
        int sum {};
        for (int i = 0; i < list1.size(); ++i)
            sum += abs(list1.at(i) - list2.at(i));

        cout << "Total distance: " << sum << endl;
    }

    // Part 2
    if (!PART_1)
    {
        int sum {};
        for (auto &itr : map1)
        {
            int key = itr.first;
            int count = itr.second;

            int occurs {};

            if (map2.contains(key))
                occurs = map2.at(key);

            sum += (key * occurs) * count;
        }

        cout << "Similarity score: " << sum << endl;
    }


    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Time taken: " << duration.count() / 1000 << " milliseconds" << endl;
    
    return EXIT_SUCCESS;
}
