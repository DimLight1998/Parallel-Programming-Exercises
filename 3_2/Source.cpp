#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
using namespace std;

void Simulate(const long long times, long long& hitCount)
{
    hitCount = 0;
    for (auto i = 0; i < times; i++)
    {
        const auto x = (static_cast<double>(rand()) / RAND_MAX) * 2 - 1;
        const auto y = (static_cast<double>(rand()) / RAND_MAX) * 2 - 1;
        if (x * x + y * y <= 1) hitCount++;
    }
}

int main()
{
    srand(time(nullptr));
    MPI_Init(nullptr, nullptr);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        long long numTosses;
        cin >> numTosses;

        auto numTossesEach = numTosses / size;
        MPI_Bcast(&numTossesEach, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

        long long hitCount;
        Simulate(numTossesEach, hitCount);

        long long hitCountSum;
        MPI_Reduce(&hitCount, &hitCountSum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        cout << "Estimated value of Pi is "
            << static_cast<long double>(hitCountSum * 4) / static_cast<long double>(numTossesEach * size)
            << " with " << numTossesEach * size << " tosses.\n";
    }
    else
    {
        long long numTosses;
        MPI_Bcast(&numTosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

        long long hitCount;
        Simulate(numTosses, hitCount);

        MPI_Reduce(&hitCount, nullptr, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
