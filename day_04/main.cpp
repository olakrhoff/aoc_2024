#include <iostream>
#include <fstream>

using namespace std;

#define DEBUG 0

#define PART_1 false

char XMAS[] = "XMAS";

void swap(char *c1, char *c2)
{
    char t = *c1;
    *c1 = *c2;
    *c2 = t;
}

void invert_xmas()
{
    swap(XMAS, XMAS + 3);
    swap(XMAS + 1, XMAS + 2);
}

int search_line(const string &line)
{
    int occurences {};
    for (int i = 0; i < (int)line.length() - 3; ++i)
    {
        if (line.at(i) == XMAS[0]
            && line.at(i + 1) == XMAS[1]
            && line.at(i + 2) == XMAS[2]
            && line.at(i + 3) == XMAS[3])
        {
            ++occurences;
            i += 3;
        }
    }

    return occurences;
}

int search_all_lines(const vector<string> &word_search)
{
    int sum {};

    for (auto line : word_search)
        sum += search_line(line);

    return sum;
}

void transpose_search(vector<string> &word_search)
{
    vector<string> result {};

    for (int i = 0; i < word_search.at(0).length(); ++i)
    {
        string temp = "";
        for (auto &j : word_search)
            temp += j.at(i);
        result.emplace_back(temp);
    }

    word_search = result;
}

void diagonalise_search(vector<string> &word_search)
{
    vector<string> result {};

    // Upper right diagonals (and middle diagonal)
    for (int i = 0; i < word_search.at(0).length(); ++i)
    {
        string temp = "";
        for (int j = 0; i + j < word_search.size(); ++j)
            temp += word_search.at(j).at(i + j);

        result.emplace_back(temp);
    }

    // Lower left diagonals
    for (int j = 1; j < word_search.size(); ++j)
    {
        string temp = "";
        for (int i = 0; j + i < word_search.at(0).length(); ++i)
            temp += word_search.at(j + i).at(i);

        result.emplace_back(temp);
    }

    // Upper left diagonals (and middle diagonal)
    for (int i = (int)word_search.at(0).length() - 1; i >= 0; --i)
    {
        string temp = "";
        for (int j = 0; i - j >= 0; ++j)
            temp += word_search.at(j).at( i - j);

        result.emplace_back(temp);
    }

    // Lower right diagonals
    for (int j = 1; j < word_search.size(); ++j)
    {
        string temp = "";
        for (int i = (int)word_search.at(0).length() - 1; j + ((int)word_search.at(0).length() - 1 - i) < word_search.size(); --i)
            temp += word_search.at(j + ((int)word_search.at(0).length() - 1 - i)).at(i);

        result.emplace_back(temp);
    }

    word_search = result;
}

int search_for_xmas(vector<string> &word_search)
{
    int total_sum {};
    // Search horixontaly
    total_sum += search_all_lines(word_search);
    // inverse horz
    invert_xmas();
    total_sum += search_all_lines(word_search);
    // vertical
    transpose_search(word_search);
    total_sum += search_all_lines(word_search);
    // inverse vertical
    invert_xmas();
    total_sum += search_all_lines(word_search);
    // diagonal
    diagonalise_search(word_search);
    total_sum += search_all_lines(word_search);
    // inv dig
    invert_xmas();
    total_sum += search_all_lines(word_search);

    return total_sum;
}

int search_coord(const vector<string> &word_search, int col, int row)
{
    if (word_search.at(col).at(row) != 'A')
        return 0;
    int Ss {};
    int Ms {};

    if (word_search.at(col - 1).at(row - 1) == 'S')
        ++Ss;
    else if (word_search.at(col - 1).at(row - 1) == 'M')
        ++Ms;
    else
        return 0;

    if (word_search.at(col + 1).at(row + 1) == 'S')
        ++Ss;
    else if (word_search.at(col + 1).at(row + 1) == 'M')
        ++Ms;
    else
        return 0;

    // If we find MAM or SAS, we stop
    if (Ss != Ms)
        return 0;

    if (word_search.at(col + 1).at(row - 1) == 'S')
        ++Ss;
    else if (word_search.at(col + 1).at(row - 1) == 'M')
        ++Ms;
    else
        return 0;

    if (word_search.at(col - 1).at(row + 1) == 'S')
        ++Ss;
    else if (word_search.at(col - 1).at(row + 1) == 'M')
        ++Ms;
    else
        return 0;

    if (Ss != 2 || Ms != 2)
        return 0;

    return 1;
}

int search_for_x_mas(vector<string> &word_search)
{
    int sum {};
    for (int i = 1; i < word_search.at(0).length() - 1; ++i)
        for (int j = 1; j < word_search.size() - 1; ++j)
            sum += search_coord(word_search, i, j);

    return sum;
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

    vector<string> word_search {};
    
    while (!file.eof())
    {
        getline(file, temp);
        word_search.emplace_back(temp);
    }
    
    file.close();
    
    // Part 1
    if (PART_1)
    {
        cout << "Number of 'XMAS' in word search: " << search_for_xmas(word_search) << endl;
    }

    // Part 2
    if (!PART_1)
    {
        // 1988 is too low, 2051 is too high
        cout << "Number of 'X-MAS' in word search: " << search_for_x_mas(word_search) << endl;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    
    cout << "Time taken: " << duration.count() / 1000 << " milliseconds" << endl;
    
    return EXIT_SUCCESS;
}
