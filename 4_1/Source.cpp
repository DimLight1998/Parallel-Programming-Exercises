// Count numbers in some intervals.

#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>

using namespace std;

void Count(const double data[], const int indexFrom, const int indexTo, int globalCount[], mutex& mtx)
{
    const auto localCount = new int[20];
    for (auto i = 0; i < 20; i++) localCount[i] = 0;
    for (auto i = indexFrom; i < indexTo; i++)
        if (data[i] >= 0 && data[i] < 100)
            localCount[static_cast<int>(data[i]) / 5]++;

    mtx.lock();
    for (auto i = 0; i < 20; i++)
        globalCount[i] += localCount[i];
    mtx.unlock();

    delete[] localCount;
}

int main()
{
    int numThread;
    scanf_s("%d", &numThread);

    ifstream fin;
    fin.open("./data.txt");
    int dataSize;
    fin >> dataSize;
    const auto data = new double[dataSize];
    for (auto i = 0; i < dataSize; i++) fin >> data[i];
    fin.close();

    const auto globalCount = new int[20];
    for (auto i = 0; i < 20; i++) globalCount[i] = 0;

    mutex mtx;
    const auto threads = new thread[numThread];
    for (auto i = 0; i < numThread; i++)
        threads[i] = thread(Count, data,
                            i * (dataSize / numThread), (i + 1) * (dataSize / numThread),
                            globalCount, ref(mtx));

    for (auto i = 0; i < numThread; i++)
        threads[i].join();

    for (auto i = 0; i < 20; i++)
        printf("Number of data in interval [%d, %d): %d\n", i * 5, (i + 1) * 5, globalCount[i]);

    delete[] threads;
    delete[] globalCount;
    delete[] data;

    return 0;
}
