#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        vector<int> data = {10, 20, 30, 40, 50};
        for (int i = 1; i < size; i++) {
            int count = data.size();
            MPI_Send(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(data.data(), count, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        int count;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        vector<int> received_data(count);
        MPI_Recv(received_data.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " received data: ";
        for (int num : received_data) cout << num << " ";
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
