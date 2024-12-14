#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <regex>

using namespace std;

#define DEBUG 0

#define PART_1 false

pair<int, int> parse_input_pair(const string &input)
{
    pair<int, int> result {};

    regex mulRegex(R"((\d{2})\|(\d{2}))");

    smatch match;
    if (!regex_search(input, match, mulRegex))
    {
        cout << "Malformed input: " << input << endl;
        exit(EXIT_FAILURE);
    }

    if (!match.ready())
    {
        cout << "Not ready" << endl;
        exit(EXIT_FAILURE);
    }

    result.first = stoi(match[1].str());
    result.second = stoi(match[2].str());

    return result;
}

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

vector<int> parse_line(const string &line)
{
    vector<int> report {};

    for (auto &val : split(line, ','))
        report.emplace_back(stoi(val));

    return report;
}

bool check_sequence(const vector<int> &sequence, const map<int, vector<int>> &rules)
{
    set<int> occurred {};

    for (auto val : sequence)
    {
        occurred.insert(val);
        if (!rules.contains(val))
            continue;
        for (auto checks : rules.at(val))
            if (occurred.contains(checks))
                return false;
    }

    return true;
}

int middle_number(const vector<int> &sequence)
{
    return sequence.at(sequence.size() / 2);
}

vector<int> fix_sequence_order(const vector<int> &sequence, const map<int, vector<int>> &rules)
{
    vector<int> new_sequence {sequence};
    set<int> occurred {};

    for (auto val : sequence)
    {
        occurred.insert(val);
        if (!rules.contains(val))
            continue;
        for (auto checks : rules.at(val))
            if (occurred.contains(checks))
            {
                // Swap the number causing the issue.
                // val should be before checks which has occurred
                auto itr = find(new_sequence.begin(), new_sequence.end(), val);
                int val_idx = distance(new_sequence.begin(), itr);

                itr = find(new_sequence.begin(), new_sequence.end(), checks);
                int checks_idx = distance(new_sequence.begin(), itr);

                swap(new_sequence[val_idx], new_sequence[checks_idx]);
                return fix_sequence_order(new_sequence, rules);
            }
    }

    return new_sequence;
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
    bool first_step {true};

    vector<vector<int>> sequences {};
    map<int, vector<int>> rules {};

    while (!file.eof())
    {
        getline(file, temp);

        if (temp.empty())
        {
            first_step = false;
            continue;
        }

        if (first_step)
        {
            auto val  = parse_input_pair(temp);
            if (!rules.contains(val.first))
                rules.insert({val.first, {val.second}});
            else
                rules.at(val.first).emplace_back(val.second);
        }
        else
            sequences.emplace_back(parse_line(temp));
    }
    
    file.close();
    
    // Part 1
    if (PART_1)
    {
        int sum {};
        for (auto &seq : sequences)
            if (check_sequence(seq, rules))
                sum += middle_number(seq);

        cout << "Sum of middle numbers of valid sequences: " << sum << endl;
    }

    // Part 2
    if (!PART_1)
    {
        int sum {};
        for (auto &seq : sequences)
            if (!check_sequence(seq, rules))
                sum += middle_number(fix_sequence_order(seq, rules));

        cout << "Sum of middle numbers of fixed invalid sequences: " << sum << endl;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Time taken: " << duration.count() / 1000 << " milliseconds" << endl;
    
    return EXIT_SUCCESS;
}
