#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;


int main(int argc, char* argv[]) {
    int rank, size, local_value, prefix_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_value = rank + 1;

    cout << "Process " << rank << " has local value: " << local_value << endl;

    MPI_Scan(&local_value, &prefix_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout << "Process " << rank << " computed prefix sum: " << prefix_sum << endl;

    MPI_Finalize();
    return 0;
}
