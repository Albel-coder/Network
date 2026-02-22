<<<<<<< HEAD
﻿#include "NetWork.h"
#include <chrono>
#include <math.h>

struct data_info
{
    double* pixels;
    int digit;
};
data_NetWork ReadDataNetWork(string path)
{
    data_NetWork data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open())
    {
        cout << "Error reading the file" << path << "\n";
        system("pause");
    }
    else
    {
        cout << path << "loading...\n";
    }
    string tmp;
    int L;
    while (!fin.eof())
    {
        fin >> tmp;
        if (tmp == "NetWork")
        {
            fin >> L;
            data.Layer = L;
            data.size = new int[L];
            for (int i = 0; i < L; i++)
            {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}

data_info* ReadData(string path, const data_NetWork& data_NW, int& examples)
{
    data_info* data;
    ifstream fin;
    fin.open(path);
    if (!fin.is_open())
    {
        cout << "Error reading the file" << path << "\n";
        system("pause");
    }
    else
    {
        cout << path << " loading...\n";
    }
    string tmp;
    fin >> tmp;
    if (tmp == "Examples")
    {
        fin >> examples;
        cout << "Examples: " << examples << "\n";
        data = new data_info[examples];
        for (int i = 0; i < examples; i++)
        {
            data[i].pixels = new double[data_NW.size[0]];
        }

        for (int i = 0; i < examples; ++i)
        {
            fin >> data[i].digit;            
            for (int j = 0; j < data_NW.size[0]; ++j)
            {
                fin >> data[i].pixels[j];
                data[i].digit /= 5;
            }
        }
        fin.close();
        cout << " lib_MNIST loaded... \n";
        return data;
    }
    else
    {
        cout << "Error loading: " << path << "\n";
        fin.close();
        return nullptr;
    }
}

int main()
{
    NetWork NW{};
    data_NetWork NW_config;
    data_info* data;
    double ra = 0, right, predict, maxra = 0;
    int epoch = 0;
    bool study, repeat = true;
    chrono::duration<double> time;

    NW_config = ReadDataNetWork("Config.txt");
    NW.Initialization(NW_config);
    NW.printConfig();

    while (repeat)
    {
        cout << "STUDY? (1/0) \n";
        cin >> study;
        if (study)
        {
            int examples;
            data = ReadData("lib_MNIST_edit.csv", NW_config, examples);
            auto start = chrono::steady_clock::now();
            while (ra / examples * 100 < 100)
            {
                ra = 0;
                auto t1 = chrono::steady_clock::now();
                for (int i = 0; i < examples; ++i)
                {
                    NW.Input(data[i].pixels);
                    right = data[i].digit;
                    predict = NW.ForwardFeed();
                    if (predict != right)
                    {
                        NW.BackPropogation(right);
                        NW.WeightsUpdater(0.15 * exp(-epoch / 20.));
                    }
                    else
                    {
                        ra++;
                    }
                }
                auto t2 = chrono::steady_clock::now();
                time = t2 - t1;
                if (ra > maxra)
                {
                    maxra = ra;
                }      
                cout << "ra: " << ra / examples * 100 << "\t" << " maxra: " << maxra / examples * 100 << " time: " << time.count() << "\n";
            }
            epoch++;
            if (epoch == 1000)
            {
                break;
            }
            auto end = chrono::steady_clock::now();
            time = end - start;
            cout << "TIME: " << time.count() / 60. << " min \n";
            NW.SaveWeight();
        }
        else
        {
            NW.ReadWeight();
        }
        cout << "Test? (1/0)\n";
        bool test_flag;
        cin >> test_flag;
        if (test_flag)
        {
            int ex_tests;
            data_info* data_test;
            data_test = ReadData("lib_10k.csv", NW_config, ex_tests);
            ra = 0;
            for (int i = 0; i < ex_tests; ++i)
            {
                NW.Input(data_test[i].pixels);
                predict = NW.ForwardFeed();
                right = data_test[i].digit;
                if (right == predict)
                {
                    ra++;
                }                
            }    
            cout << "RA: " << ra / ex_tests * 100 << "\n";
        }
        cout << "Repeat? (1/0)\n";
        cin >> repeat;
    }

    system("pause");
    return 0;
=======
﻿#include "NetWork.h"
#include <chrono>
#include <math.h>

struct data_info
{
    double* pixels;
    int digit;
};
data_NetWork ReadDataNetWork(string path)
{
    data_NetWork data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open())
    {
        cout << "Error reading the file" << path << "\n";
        system("pause");
    }
    else
    {
        cout << path << "loading...\n";
    }
    string tmp;
    int L;
    while (!fin.eof())
    {
        fin >> tmp;
        if (tmp == "NetWork")
        {
            fin >> L;
            data.Layer = L;
            data.size = new int[L];
            for (int i = 0; i < L; i++)
            {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}

data_info* ReadData(string path, const data_NetWork& data_NW, int& examples)
{
    data_info* data;
    ifstream fin;
    fin.open(path);
    if (!fin.is_open())
    {
        cout << "Error reading the file" << path << "\n";
        system("pause");
    }
    else
    {
        cout << path << " loading...\n";
    }
    string tmp;
    fin >> tmp;
    if (tmp == "Examples")
    {
        fin >> examples;
        cout << "Examples: " << examples << "\n";
        data = new data_info[examples];
        for (int i = 0; i < examples; i++)
        {
            data[i].pixels = new double[data_NW.size[0]];
        }

        for (int i = 0; i < examples; ++i)
        {
            fin >> data[i].digit;            
            for (int j = 0; j < data_NW.size[0]; ++j)
            {
                fin >> data[i].pixels[j];
                data[i].digit /= 5;
            }
        }
        fin.close();
        cout << " lib_MNIST loaded... \n";
        return data;
    }
    else
    {
        cout << "Error loading: " << path << "\n";
        fin.close();
        return nullptr;
    }
}

int main()
{
    NetWork NW{};
    data_NetWork NW_config;
    data_info* data;
    double ra = 0, right, predict, maxra = 0;
    int epoch = 0;
    bool study, repeat = true;
    chrono::duration<double> time;

    NW_config = ReadDataNetWork("Config.txt");
    NW.Initialization(NW_config);
    NW.printConfig();

    while (repeat)
    {
        cout << "STUDY? (1/0) \n";
        cin >> study;
        if (study)
        {
            int examples;
            data = ReadData("lib_MNIST_edit.csv", NW_config, examples);
            auto start = chrono::steady_clock::now();
            while (ra / examples * 100 < 100)
            {
                ra = 0;
                auto t1 = chrono::steady_clock::now();
                for (int i = 0; i < examples; ++i)
                {
                    NW.Input(data[i].pixels);
                    right = data[i].digit;
                    predict = NW.ForwardFeed();
                    if (predict != right)
                    {
                        NW.BackPropogation(right);
                        NW.WeightsUpdater(0.15 * exp(-epoch / 20.));
                    }
                    else
                    {
                        ra++;
                    }
                }
                auto t2 = chrono::steady_clock::now();
                time = t2 - t1;
                if (ra > maxra)
                {
                    maxra = ra;
                }      
                cout << "ra: " << ra / examples * 100 << "\t" << " maxra: " << maxra / examples * 100 << " time: " << time.count() << "\n";
            }
            epoch++;
            if (epoch == 1000)
            {
                break;
            }
            auto end = chrono::steady_clock::now();
            time = end - start;
            cout << "TIME: " << time.count() / 60. << " min \n";
            NW.SaveWeight();
        }
        else
        {
            NW.ReadWeight();
        }
        cout << "Test? (1/0)\n";
        bool test_flag;
        cin >> test_flag;
        if (test_flag)
        {
            int ex_tests;
            data_info* data_test;
            data_test = ReadData("lib_10k.csv", NW_config, ex_tests);
            ra = 0;
            for (int i = 0; i < ex_tests; ++i)
            {
                NW.Input(data_test[i].pixels);
                predict = NW.ForwardFeed();
                right = data_test[i].digit;
                if (right == predict)
                {
                    ra++;
                }                
            }    
            cout << "RA: " << ra / ex_tests * 100 << "\n";
        }
        cout << "Repeat? (1/0)\n";
        cin >> repeat;
    }

    system("pause");
    return 0;
>>>>>>> f401698 (edited)
}