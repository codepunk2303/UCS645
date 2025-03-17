#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

#define SIZE 70

void initializeMatrix(vector<vector<int>>& matrix) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            matrix[i][j] = rand() % 10;
}

void sequence_mul(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                C[i][j] += A[i][k] * B[k][j];
}

void parallel_mul(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = SIZE / size;
    vector<vector<int>> A(SIZE, vector<int>(SIZE, 0));
    vector<vector<int>> B(SIZE, vector<int>(SIZE, 0));
    vector<vector<int>> C(SIZE, vector<int>(SIZE, 0));
    vector<int> local_A(rows_per_process * SIZE, 0);
    vector<int> local_C(rows_per_process * SIZE, 0);
    vector<int> flattened_B(SIZE * SIZE, 0);

    if (rank == 0) {
        srand(time(0));
        initializeMatrix(A);
        initializeMatrix(B);
        
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                flattened_B[i * SIZE + j] = B[i][j];
    }

    
    MPI_Bcast(flattened_B.data(), SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(A[0].data(), rows_per_process * SIZE, MPI_INT, local_A.data(), rows_per_process * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    
    vector<vector<int>> local_B(SIZE, vector<int>(SIZE, 0));
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            local_B[i][j] = flattened_B[i * SIZE + j];

   
    for (int i = 0; i < rows_per_process; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                local_C[i * SIZE + j] += local_A[i * SIZE + k] * local_B[k][j];


    MPI_Gather(local_C.data(), rows_per_process * SIZE, MPI_INT, C[0].data(), rows_per_process * SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "MPI Matrix Multiplication completed!" << endl;
    }

    MPI_Finalize();
}

int main(int argc, char* argv[]) {
    vector<vector<int>> A(SIZE, vector<int>(SIZE, 0));
    vector<vector<int>> B(SIZE, vector<int>(SIZE, 0));
    vector<vector<int>> C(SIZE, vector<int>(SIZE, 0));

    srand(time(0));
    initializeMatrix(A);
    initializeMatrix(B);

    double start_seq = MPI_Wtime();
    sequence_mul(A, B, C);
    double end_seq = MPI_Wtime();
    cout << "Sequential Time: " << end_seq - start_seq << " seconds" << endl;


    double start_mpi = MPI_Wtime();
    parallel_mul(argc, argv);
    double end_mpi = MPI_Wtime();
    cout << "MPI Time: " << end_mpi - start_mpi << " seconds" << endl;

    return 0;
}
