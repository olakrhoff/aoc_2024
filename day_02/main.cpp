#include <iostream>
#include <fstream>

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

vector<int> parse_line(const string &line)
{
    vector<int> report {};

    for (auto &val : split(line, ' '))
        report.emplace_back(stoi(val));

    return report;
}

bool is_report_safe(vector<int> &report)
{
    bool asc = report.at(0) < report.at(1);

    for (int i = 1; i < report.size(); ++i)
    {
        int diff = report.at(i) - report.at(i - 1);

        if (abs(diff) < 1 || abs(diff) > 3)
            return false;

        if (asc)
        {
            if (diff < 0)
                return false;
        }
        else
        {
            if (diff > 0)
                return false;
        }
    }

    return true;
}

bool is_report_safe_with_tolerance(vector<int> &report)
{
    int dir_count {};
    for (int i = 0; i < 3; ++i)
    {
        if (report.at(i) == report.at(i + 1))
        {
            auto remove_first = report;
            remove_first.erase(next(remove_first.begin(), i - 1));
            auto remove_second = report;
            remove_second.erase(next(remove_second.begin(), i));

            return is_report_safe(remove_first) || is_report_safe(remove_second);
        }
        else if (report.at(i) < report.at(i + 1))
            ++dir_count; //asc
        else
            --dir_count; //dsc
    }

    bool asc = dir_count > 0;

    int previous_error_idx = -1;

    for (int i = 1; i < report.size(); ++i)
    {
        int diff = report.at(i) - report.at(i - 1);

        if (abs(diff) < 1 || abs(diff) > 3)
        {
            auto remove_first = report;
            remove_first.erase(next(remove_first.begin(), i - 1));
            auto remove_second = report;
            remove_second.erase(next(remove_second.begin(), i));

            return is_report_safe(remove_first) || is_report_safe(remove_second);
        }

        if (asc)
        {
            if (diff < 0)
            {
                auto remove_first = report;
                remove_first.erase(next(remove_first.begin(), i - 1));
                auto remove_second = report;
                remove_second.erase(next(remove_second.begin(), i));

                return is_report_safe(remove_first) || is_report_safe(remove_second);
            }
        }
        else
        {
            if (diff > 0)
            {
                auto remove_first = report;
                remove_first.erase(next(remove_first.begin(), i - 1));
                auto remove_second = report;
                remove_second.erase(next(remove_second.begin(), i));

                return is_report_safe(remove_first) || is_report_safe(remove_second);
            }
        }
    }

    return true;
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
    vector<vector<int>> reports {};
    
    while (!file.eof())
    {
        getline(file, temp);

        reports.emplace_back(parse_line(temp));
    }
    
    file.close();
    
    // Part 1
    if (PART_1)
    {
        int safe_reports {};
        for (auto &report : reports)
            if (is_report_safe(report))
                ++safe_reports;

        cout << "Safe reports: " << safe_reports << endl;
    }

    // Part 2
    if (!PART_1)
    {
        int safe_reports {};
        for (auto &report : reports)
            if (is_report_safe_with_tolerance(report))
                ++safe_reports;


        cout << "Safe reports: " << safe_reports << endl;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Time taken: " << duration.count() / 1000 << " milliseconds" << endl;
    
    return EXIT_SUCCESS;
}
