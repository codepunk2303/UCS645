#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;


int monteCarloPi(int num_samples) {
    int count = 0;
    for (int i = 0; i < num_samples; i++) {
        double x = (double)rand() / RAND_MAX; 
        double y = (double)rand() / RAND_MAX;  
        if (x * x + y * y <= 1.0) {
            count++;  
        }
    }
    return count;
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(0) + rank);  // Seed random number generator differently for each process

    int total_samples = 1e6; 
    int local_samples = total_samples / size;

    int local_count = monteCarloPi(local_samples);
    int global_count = 0;

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = (4.0 * global_count) / total_samples;
        cout << "Estimated Pi = " << pi_estimate << endl;
    }

    MPI_Finalize();
    return 0;
}
