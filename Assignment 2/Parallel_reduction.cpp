#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 100;
    vector<int> data;
    int local_n = N / size;
    vector<int> local_data(local_n);

    if (rank == 0) {
        data.resize(N);
        srand(time(0));
        for (int i = 0; i < N; i++) {
            data[i] = rand() % 10;
        }
        cout << "Original Data: ";
        for (int num : data) cout << num << " ";
        cout << endl;
    }

    MPI_Scatter(data.data(), local_n, MPI_INT, local_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int num : local_data) {
        local_sum += num;
    }

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Total Sum (Parallel Reduction): " << global_sum << endl;
    }

    MPI_Finalize();
    return 0;
}
