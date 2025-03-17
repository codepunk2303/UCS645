#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(0) + rank);
    int position = 0;
    int steps = 10;

    for (int i = 0; i < steps; i++) {
        int move = (rand() % 2) ? 1 : -1;
        position += move;
        cout << "Process " << rank << " step " << i + 1 << " moved to position " << position << endl;
        if (rank < size - 1) {
            MPI_Send(&position, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
        if (rank > 0) {
            MPI_Recv(&position, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    cout << "Process " << rank << " final position: " << position << endl;
    MPI_Finalize();
    return 0;
}
