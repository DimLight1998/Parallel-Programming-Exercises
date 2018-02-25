// Global sum computation using tree structure.

#include <iostream>
#include <mpi.h>
#include <ctime>

using namespace std;

int GetPartner(const int round, const int numberSelf, bool& sending)
{
    const auto base = 1 << round;
    if ((numberSelf / base) % 2 == 0)
    {
        sending = false;
        return numberSelf + base;
    }

    sending = true;
    return numberSelf - base;
}

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

    while (true)
    {
        bool sending;
        const auto partner = GetPartner(round, rank, sending);
        round++;
        if (partner >= size)
        {
            if (rank == 0) break;
            continue;
        }

        if (sending)
        {
            MPI_Send(&value, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Finalize();
            return 0;
        }

        int buffer;
        MPI_Recv(&buffer, 1, MPI_INT, partner, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += buffer;
    }

    cout << "Sum is " << value << endl;
    MPI_Finalize();
    return 0;
}
