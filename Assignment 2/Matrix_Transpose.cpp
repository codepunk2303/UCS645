#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

void printMatrix(const vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void l_transpose(vector<vector<int>>& matrix, vector<vector<int>>& t_matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            t_matrix[j][i] = matrix[i][j];
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 6;
    vector<vector<int>> matrix(N, vector<int>(N, 0));
    vector<vector<int>> transposed(N, vector<int>(N, 0));

    int local_rows = N / size; 
    vector<vector<int>> local_matrix(local_rows, vector<int>(N));
    vector<vector<int>> local_transposed(N, vector<int>(local_rows));

    if (rank == 0) {
        int value = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = value++;
            }
        }
        cout << "Original Matrix:"<<endl;
        printMatrix(matrix, N, N);
    }

    MPI_Scatter(matrix.data(), local_rows * N, MPI_INT, local_matrix.data(), local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    l_transpose(local_matrix, local_transposed, local_rows, N);

    MPI_Gather(local_transposed.data(), local_rows * N, MPI_INT, transposed.data(), local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Transposed Matrix:"<<endl;
        printMatrix(transposed, N, N);
    }

    MPI_Finalize();
    return 0;
}
