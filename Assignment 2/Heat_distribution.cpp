#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

const int N = 10;
const int max_iterations = 100;  
const double tolerance = 1e-4;

void printGrid(const vector<vector<double>>& grid) {
    for (const auto& row : grid) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

double updateHeat(vector<vector<double>>& local_grid, int start, int end, int N) {
    vector<vector<double>> new_grid = local_grid;
    double max_diff = 0.0;

    for (int i = start; i < end; i++) {
        for (int j = 1; j < N - 1; j++) {
            new_grid[i][j] = 0.25 * (local_grid[i - 1][j] + local_grid[i + 1][j] +
                                     local_grid[i][j - 1] + local_grid[i][j + 1]);
            max_diff = max(max_diff, fabs(new_grid[i][j] - local_grid[i][j]));
        }
    }
    
    local_grid = new_grid;
    return max_diff;
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;
    vector<vector<double>> local_grid(N, vector<double>(N, 20.0));  // Initial temp 20C

    if (rank == 0) {
        for (int j = 0; j < N; j++) {
            local_grid[0][j] = 100.0;
        }
    }

    double global_max_diff;
    int iter = 0;

    while (global_max_diff > tolerance && iter < max_iterations) {
        double local_max_diff = updateHeat(local_grid, 1, rows_per_proc + 1, N);

        if (rank > 0) {
            MPI_Send(local_grid[1].data(), N, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(local_grid[0].data(), N, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank < size - 1) {
            MPI_Send(local_grid[rows_per_proc].data(), N, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(local_grid[rows_per_proc + 1].data(), N, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        MPI_Allreduce(&local_max_diff, &global_max_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        iter++;

    } 


    vector<vector<double>> final_grid(N, vector<double>(N));
    MPI_Gather(local_grid.data(), rows_per_proc * N, MPI_DOUBLE, final_grid.data(), rows_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Final Heat Distribution (Iteration " << iter << "):"<<endl;
        printGrid(final_grid);
    }

    MPI_Finalize();
    return 0;
}
