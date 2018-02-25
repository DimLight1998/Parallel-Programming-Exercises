// Global sum computation using butterfly structure.

#include <mpi.h>
#include <random>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

int GetPartner(const int round, const int selfNumber)
{
    const auto base = 1 << round;
    if (selfNumber % (base * 2) < base) return selfNumber + base;
    return selfNumber - base;
}

// Number of processes need to be a power of 2 for simplicity.
int main()
{
    MPI_Init(nullptr, nullptr);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(static_cast<unsigned>(time(nullptr)) + rank);

    auto round = 0;
    auto value = rand() % 100;

    cout << "Initial value is " << value << " on processor " << rank << endl;

    const auto maxRound = log2(size) - 1;
    
    while(round <= maxRound)
    {
        int buffer;
        const auto partner = GetPartner(round, rank);
        MPI_Sendrecv(&value, 1, MPI_INT, partner, 0, &buffer, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += buffer;
        round++;
    }

    cout << "Final value is " << value << " on processor " << rank << endl;
    MPI_Finalize();

    return 0;
}