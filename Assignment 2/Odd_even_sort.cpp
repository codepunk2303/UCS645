#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

void l_sort(vector<int>& l_data) {
    sort(l_data.begin(), l_data.end());
}

void mergeAndSwap(vector<int>& l_data, vector<int>& neighbor_data, bool keepSmaller) {
    vector<int> merged(l_data.size() + neighbor_data.size());
    merge(l_data.begin(), l_data.end(), neighbor_data.begin(), neighbor_data.end(), merged.begin());

    if (keepSmaller) {
        copy(merged.begin(), merged.begin() + l_data.size(), l_data.begin());
    } else {
        copy(merged.end() - l_data.size(), merged.end(), l_data.begin());
    }
}

void parallelOddEvenSort(vector<int>& data, int rank, int size) {
    int n = data.size();
    int local_n = n / size;
    vector<int> l_data(local_n);

    MPI_Scatter(data.data(), local_n, MPI_INT, l_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    l_sort(l_data);

    for (int phase = 0; phase < size; ++phase) {
        int partner = (phase % 2 == 0) ? (rank ^ 1) : (rank);
        
        if (partner >= 0 && partner < size) {
            vector<int> neighbor_data(local_n);
            MPI_Sendrecv(l_data.data(), local_n, MPI_INT, partner, 0,
                         neighbor_data.data(), local_n, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (rank < partner) {
                mergeAndSwap(l_data, neighbor_data, true);
            } else {
                mergeAndSwap(l_data, neighbor_data, false);
            }
        }
    }

    MPI_Gather(l_data.data(), local_n, MPI_INT, data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100; 
    vector<int> data;

    if (rank == 0) {
        srand(time(0));
        data.resize(N);
        for (int i = 0; i < N; i++) {
            data[i] = rand() % 1000;
        }
        cout << "Unsorted Data: ";
        for (int num : data) cout << num << " ";
        cout << endl;
    }

    double start = MPI_Wtime();
    parallelOddEvenSort(data, rank, size);
    double end = MPI_Wtime();

    if (rank == 0) {
        cout << "Sorted Data: ";
        for (int num : data) cout << num << " ";
        cout << endl;
        cout << "Parallel Sorting Time: " << (end - start) << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
