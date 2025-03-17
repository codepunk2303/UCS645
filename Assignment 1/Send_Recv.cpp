#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int data = 3517;
        for (int i = 1; i < size; i++) {
            MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        int received_data;
        MPI_Recv(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " received data: " << received_data << endl;
    }

    MPI_Finalize();
    return 0;
}