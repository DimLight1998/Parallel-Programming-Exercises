// Estimate Pi using Monte Carlo method.

#include <thread>
#include <mutex>
#include <iostream>
#include <random>

using namespace std;

void Simulate(const int numTosses, const int seed, int* hit)
{
    srand(seed);
    auto hitCount = 0;
    for (auto i = 0; i < numTosses; i++)
    {
        const auto x = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
        const auto y = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
        if (x * x + y * y <= 1) hitCount++;
    }
    *hit = hitCount;
}

int main()
{
    int numTosses;
    cin >> numTosses;

    auto localNumTosses = numTosses / 4;

    thread threads[4];
    int* hits[4];
    auto hitTotal = 0;

    for (auto i = 0; i < 4; i++)
    {
        hits[i] = new int;
        threads[i] = thread(Simulate, localNumTosses, (i << 7) + 71, hits[i]);
    }
    for (auto i = 0; i < 4; i++)
    {
        threads[i].join();
        hitTotal += *hits[i];
        delete hits[i];
    }

    cout << "Estimated value of Pi is " << 4 * static_cast<double>(hitTotal) / (static_cast<int>(numTosses / 4) * 4);

    return 0;
}
