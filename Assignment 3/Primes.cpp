#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max_value = 102203517;  //Roll no is 102203517
    if (rank == 0) {
        int num = 2;
        int received, source;
        MPI_Status status;
        vector<int> primes;
        int active_slaves = size - 1;
        for (int i = 1; i < size; i++) {
            MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            num++;
        }
        while (active_slaves > 0) {
            MPI_Recv(&received, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            if (received > 0) primes.push_back(received);
            if (num <= max_value) {
                MPI_Send(&num, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
                num++;
            } else {
                int stop_signal = -1;
                MPI_Send(&stop_signal, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
                active_slaves--;
            }
        }
        cout << "Total primes found: " << primes.size() << endl;
    } else {
        int num;
        MPI_Status status;
        while (true) {
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            if (num < 0) break;
            int result = is_prime(num) ? num : -num;
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
