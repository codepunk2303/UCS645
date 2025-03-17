#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long num_steps = 100000;
    double step, local_sum = 0.0, pi = 0.0;
    
    if (rank == 0) step = 1.0 / (double)num_steps;
    MPI_Bcast(&step, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int chunk = num_steps / size;
    int start = rank * chunk;
    int end = start + chunk;

    for (int i = start; i < end; i++) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    MPI_Reduce(&local_sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi *= step;
        cout << "Estimated Pi: " << pi << endl;
    }

    MPI_Finalize();
    return 0;
}
