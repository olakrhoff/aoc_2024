#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

#define DEBUG 0

#define PART_1 false

pair<int, int> parse_input(const string &input)
{
    pair<int, int> result {};

    regex mulRegex(R"(mul\((\d{1,3}),(\d{1,3})\))");

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

    regex mulRegex(R"((mul\(\d{1,3},\d{1,3}\))|(do\(\))|(don't\(\)))");

    vector<pair<int, int>> multiplications {};
    bool mul_enabled {true};

    while (!file.eof())
    {
        getline(file, temp);

        auto begin = sregex_iterator(temp.begin(), temp.end(), mulRegex);
        auto end = sregex_iterator();

        for (sregex_iterator itr = begin; itr != end; ++itr)
        {
            smatch match = *itr;

            if (!match[0].matched)
                continue;

            switch (match[0].str().length())
            {
                case 4: //do()
                    mul_enabled = true;
                    break;
                case 7: //don't()
                    mul_enabled = false;
                    break;
                default: //mul(X,Y)
                    if (mul_enabled)
                        multiplications.emplace_back(parse_input(match[0].str()));
                    break;
            }

        }
    }

    file.close();
    
    // Part 1
    if (PART_1)
    {
        int sum {};
        for (auto p : multiplications)
            sum += p.first * p.second;

        cout << "Sum of multiplications: " << sum << endl;
    }

    // Part 2
    if (!PART_1)
    {
        int sum {};
        for (auto p : multiplications)
            sum += p.first * p.second;

        cout << "Sum of multiplications with conditionals: " << sum << endl;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Time taken: " << duration.count() / 1000 << " milliseconds" << endl;
    
    return EXIT_SUCCESS;
}
