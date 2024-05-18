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
        
        getline(ss, value, ',');
        int deadline = stoi(value);
        deadlines.push_back(deadline);
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
    vector<int> vec;
    sort(tasks, periods, capacities, vec);
    vector<string> schedule(hyperperiod);
    vector<int> arr = capacities;
    
    ofstream file;
    //string filepath = "problem8_out_rma.txt";
    string filepath = "example_out_rma.txt";
    file.open(filepath);
    
    cout << "RM Scheduling Algorithm\n";
    bool lock;
    int idx = -1;
    int timer = 0;
    while(timer < hyperperiod)
    {
        lock = false;
        for (int i = 0; i < tasks.size(); i++)
        {
            if ((timer != 0) && ((timer % periods[i]) == 0))
            {
                arr[i] = capacities[i];
                if (!lock)
                {
                    lock = true;
                    schedule[timer] = tasks[i];
                    arr[i]--;
                    cout << "Time " << timer << ": " << schedule[timer] << endl;
                    file << "Time: " << timer << " " << schedule[timer] << endl;
                }
                continue;
            }
            if (arr[i] != 0)
            {
                idx = get_idx(tasks, periods, timer);
                if (idx == -1)
                {
                    schedule[timer] = tasks[i];
                    arr[i]--;
                    cout << "Time " << timer << ": " << schedule[timer] << endl;
                    file << "Time: " << timer << " " << schedule[timer] << endl;
                    break;
                }
            }
        }
        timer++;
    }
    file.close();
}

static void edf(vector<string> tasks, vector<int> periods, vector<int> capacities, vector<int> deadlines, int hyperperiod)
{
    cout << "EDF Scheduling Algorithm\n";
    ofstream file;
    //string filepath = "problem8_out_edf.txt";
    string filepath = "example_out_edf.txt";
    file.open(filepath);

    sort(tasks, periods, capacities, deadlines);
    vector<vector<int>> matrix(tasks.size());
    vector<string> schedule(hyperperiod);
    bool lock;
    for (int i = 0; i < tasks.size(); i++)
    {
        matrix[i].push_back(deadlines[i]);
        for (int j = 1; j <= hyperperiod / periods[i]; j++)
        {
            matrix[i].push_back(deadlines[i] + j * periods[i]);
            cout << matrix[i][j-1] << endl;
        }
    }
    vector<int> arr = capacities;
    int timer = 0;
    while(timer < hyperperiod)
    {
        lock = false;
        for (int i = 0; i < tasks.size(); i++)
        {
            if ((timer != 0) && ((timer % periods[i]) == 0))
            {
                arr[i] = capacities[i];
            }
            //for (int j = 0; j < matrix.size(); j++)
            //{
            //    for (int k = 0; k < matrix[j].size(); k++)
            //    {
            //        if (timer == matrix[j][k])
            //        {
            //            matrix[j].erase(matrix[j].begin() + k);
            //        }
            //    }
            //}

            if (arr[i] != 0)
            {
                schedule[timer] = tasks[i];
                arr[i]--;
                cout << tasks[i] << " " << arr[i] << endl;
                cout << "Time " << timer << ": " << schedule[timer] << endl;
                break;
            }
        }
        timer++;
    }
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
        cout << "Error parsing input files\n";
        return -1;
    }

    int hyperperiod = calculate_hyperperiod(periods);

    rma(tasks, periods, capacities, hyperperiod);

    flag = true;
    //edf(tasks, periods, capacities, deadlines, hyperperiod);
    
    return 0;
}
