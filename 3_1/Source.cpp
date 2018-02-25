#include <fstream>
#include <mpi.h>

using namespace std;

void GetCounts(const double data[], const int dataSize, int counts[], const int countSize)
{
    for (auto i = 0; i < countSize; i++) counts[i] = 0;
    for (auto i = 0; i < dataSize; i++) if (data[i] >= 0 && data[i] < 100) counts[static_cast<int>(data[i]) / 5]++;
}

int main(int argc, char** argv)
{
    MPI_Init(nullptr, nullptr);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // Read data.
        // Number of data should be evenly divided by number of processors for simplicity.
        ifstream fin;
        fin.open("./data.txt");
        int numData;
        fin >> numData;
        const auto data = new double[numData];
        for (auto i = 0; i < numData; i++) fin >> data[i];

        // Broadcast size of data.
        auto sizeEach = numData / size;
        MPI_Bcast(&sizeEach, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Scatter data.
        const auto localData = new double[sizeEach];
        MPI_Scatter(data, sizeEach, MPI_DOUBLE, localData, sizeEach, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Count.
        const auto counts = new int[20];
        GetCounts(localData, sizeEach, counts, 20);

        // Receive results from slaves.
        const auto countTotal = new int[20];
        MPI_Reduce(counts, countTotal, 20, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Report result.
        for (auto i = 0; i < 20; i++)
            printf("Number of data in interval [%d, %d): %d\n", i * 5, (i + 1) * 5, countTotal[i]);

        // Clean up.
        delete[] countTotal;
        delete[] counts;
        delete[] localData;
        delete[] data;
    }
    else
    {
        // Receive size of data.
        int sizeEach;
        MPI_Bcast(&sizeEach, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Receive data.
        const auto localData = new double[sizeEach];
        MPI_Scatter(nullptr, 0, MPI_DOUBLE, localData, sizeEach, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Count.
        const auto counts = new int[20];
        GetCounts(localData, sizeEach, counts, 20);

        // Send result to master.
        MPI_Reduce(counts, counts, 20, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Clean up.
        delete[] counts;
        delete[] localData;
    }

    MPI_Finalize();
    return 0;
}
