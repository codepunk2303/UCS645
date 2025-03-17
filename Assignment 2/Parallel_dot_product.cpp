#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N = 100; 
    vector<int> A, B;
    int local_n = N / size;
    vector<int> local_A(local_n);
    vector<int> local_B(local_n);

    if (rank == 0) {
        A.resize(N);
        B.resize(N);
        srand(time(0));
        for (int i = 0; i < N; i++) {
            A[i] = rand() % 10;
            B[i] = rand() % 10;
        }

        cout << "Vector A: ";
        for (int num : A) cout << num << " ";
        cout << endl;

        cout << "Vector B: ";
        for (int num : B) cout << num << " ";
        cout << endl;
    }

    MPI_Scatter(A.data(), local_n, MPI_INT, local_A.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B.data(), local_n, MPI_INT, local_B.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int local_dot_product = 0;
    for (int i = 0; i < local_n; i++) {
        local_dot_product += local_A[i] * local_B[i];
    }

    int global_dot_product = 0;
    MPI_Reduce(&local_dot_product, &global_dot_product, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Dot Product (Parallel Computation): " << global_dot_product << endl;
    }

    MPI_Finalize();
    return 0;
}
