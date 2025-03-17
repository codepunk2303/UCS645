#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 1 << 16;
    double a = 2.5;
    vector<double> X(N, 1.0), Y(N, 2.0);
    int chunk = N / size;
    vector<double> local_X(chunk), local_Y(chunk);

    MPI_Scatter(X.data(), chunk, MPI_DOUBLE, local_X.data(), chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Y.data(), chunk, MPI_DOUBLE, local_Y.data(), chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double start = MPI_Wtime();
    for (int i = 0; i < chunk; i++) local_X[i] = a * local_X[i] + local_Y[i];
    double end = MPI_Wtime();
    double parallel_time = end - start;

    MPI_Gather(local_X.data(), chunk, MPI_DOUBLE, X.data(), chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double seq_start = MPI_Wtime();
        for (int i = 0; i < N; i++) X[i] = a * X[i] + Y[i];
        double seq_end = MPI_Wtime();
        double sequential_time = seq_end - seq_start;
        cout << "Parallel Time: " << parallel_time << endl;
        cout << "Sequential Time: " << sequential_time << endl;
        cout << "Speedup: " << sequential_time / parallel_time << endl;
    }

    MPI_Finalize();
    return 0;
}
