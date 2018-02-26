// Time cost test of communication.

#include <mpi.h>
#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    MPI_Init(nullptr, nullptr);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        const auto start = clock();
        const auto ping = "ping";
        const auto recvBuf = new char[10];
        auto count = 0;
        while (clock() - start < CLOCKS_PER_SEC)
        {
            MPI_Ssend(ping, strlen(ping) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(recvBuf, 10, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            count++;
        }
        cout << count << " loops in one second.\n";
        MPI_Send("end", strlen("end") + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        delete[] recvBuf;
    }
    else if (rank == 1)
    {
        const auto pong = "pong";
        const auto recvBuf = new char[10];
        while (true)
        {
            MPI_Recv(recvBuf, 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (strcmp(recvBuf, "end") == 0) break;
            MPI_Ssend(pong, strlen(pong) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
