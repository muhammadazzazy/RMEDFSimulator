#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

bool flag = false;

static bool parse(string filepath, vector<string>& tasks, vector<int>& periods, vector<int>& capacities, vector<int>& deadlines)
{
    ifstream file;
    file.open(filepath);
    if (!file.is_open())
    {
        cout << "Error opening file.\n";
        return false;
    }
    string line;
    while (getline(file, line))
    {
        istringstream ss(line);
        string value;
        getline(ss, value, ',');
        tasks.push_back((value));

        getline(ss, value, ',');
        int period = stoi(value);
        periods.push_back(period);

        getline(ss, value, ',');
        int capacity = stoi(value);
        capacities.push_back(capacity);
        
        if (flag)
        {
            getline(ss, value, ',');
            int deadline = stoi(value);
            deadlines.push_back(deadline);
        }

    }
    file.close();
    return true;
}

static int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

static int lcm(int a, int b)
{
    if ((a == 0) || (b == 0))
    {
        return 0;
    }
    return (a * b) / gcd(a, b);
}

static int calculate_hyperperiod(vector<int>& periods)
{
    int hyperperiod = periods[0];
    for (int i = 1; i < periods.size(); i++)
    {
        hyperperiod = lcm(hyperperiod, periods[i]);
    }
    return hyperperiod;
}

static void sort(vector<string>& tasks, vector<int>& periods, vector<int>& capacities, vector<int>& deadlines)
{
    for (int i = 0; i < tasks.size(); i++)
    {
        for (int j = i + 1; j < tasks.size(); j++)
        {
            if (!flag)
            {
                if (periods[i] > periods[j])
                {
                    swap(tasks[i], tasks[j]);
                    swap(periods[i], periods[j]);
                    swap(capacities[i], capacities[j]);
                }
            }
            else
            {
                if (deadlines[i] > deadlines[j])
                {
                    swap(tasks[i], tasks[j]);
                    swap(periods[i], periods[j]);
                    swap(capacities[i], capacities[j]);
                    swap(deadlines[i], deadlines[j]);
                }
            }

        }
    }
}

static int get_idx(vector<string>& tasks, vector<int>& periods, int time)
{
    for (int i = 0; i < periods.size(); i++)
    {
        if ((time != 0) && ((time % periods[i]) == 0))
        {
            return i;
        }
    }
    return -1;
}


static void rma(vector<string> tasks, vector<int> periods, vector<int> capacities, int hyperperiod)
{
    vector<int> v;
    sort(tasks, periods, capacities, v);
    vector<string> schedule(hyperperiod);
    vector<int> arr(tasks.size());
    arr = capacities;
    ofstream file;
    string filepath = "example_out.txt";
    file.open(filepath);
    cout << "RM Scheduling Algorithm\n";
    bool lock;
    int idx = -1;
    for (int i = 0; i < hyperperiod; i++)
    {
        lock = false;
        for (int j = 0; j < tasks.size(); j++)
        {
            if ((i != 0) && ((i % periods[j]) == 0))
            {
                arr[j] = capacities[j];
                if (!lock)
                {
                    lock = true;
                    schedule[i] = tasks[j];
                    arr[j]--;
                    cout << "Time " << i << ": " << schedule[i] << endl;
                    file << "Time: " << i << " " << schedule[i] << endl;
                }
                continue;
            }
            if (arr[j] != 0)
            {
                idx = get_idx(tasks, periods, i);
                if (idx == -1)
                {
                    schedule[i] = tasks[j];
                    arr[j]--;
                    cout << "Time " << i << ": " << schedule[i] << endl;
                    file << "Time: " << i << " " << schedule[i] << endl;
                    break;
                }
            }
        }
    }
    file.close();
    tasks.clear();
    periods.clear();
    capacities.clear();
}

static void edf(vector<string> tasks, vector<int> periods, vector<int> capacities, vector<int> deadlines, int hyperperiod)
{
    //filepath = "example_in.txt";

    //cout << "EDF Scheduling Algorithm\n";
    //flag = true;
    //parse(filepath, tasks, periods, capacities, deadlines);
    //sort(tasks, periods, capacities, deadlines);
    //vector<vector<int>> mat;
    //for (int i = 0; i < tasks.size(); i++)
    //{

    //}

    //arr = capacities;
    //cout << hyperperiod << endl;
    //for (int i = 0; i < hyperperiod; i++)
    //{
    //    lock = false;
    //    for (int j = 0; j < tasks.size(); j++)
    //    {
    //        if ((i % periods[j] == 0))
    //        {
    //            
    //        }
    //        else if (arr[j] != 0)
    //        {
    //            schedule[i] = tasks[j];
    //            arr[j]--;
    //            cout << tasks[j] << " " << arr[j] << endl;
    //            cout << "Time " << i << ": " << schedule[i] << endl;
    //            break;
    //        }
    //    }
    //}
}

int main()
{
    //string filepath = "problem8_in.txt";
    string filepath = "example_in.txt";
    
    vector<string> tasks;
    vector<int> periods;
    vector<int> capacities;
    vector<int> deadlines;
    if (!parse(filepath, tasks, periods, capacities, deadlines))
    {
        return -1;
    }

    int hyperperiod = calculate_hyperperiod(periods);

    rma(tasks, periods, capacities, hyperperiod);
    
    //string filepath = "problem8_in.txt";

    flag = true;
    //edf(tasks, periods, capacities, deadlines, hyperperiod);
    return 0;
}
