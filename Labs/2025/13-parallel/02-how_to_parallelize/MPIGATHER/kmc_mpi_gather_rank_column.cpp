
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>
#include <string>
#include <mpi.h>
#include "stepper.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int Np = 1000;
    double T = 100.0, dt = 0.1;
    double G = 1.0, mu = 1.0, h = 1.0, KT = 0.1;
    double r = 0.5, mob = 1.0, D = 0.01;
    double g = 1.0, m = 1.0;

    int base_Np = Np / size;
    int remainder = Np % size;
    int local_Np = base_Np + (rank < remainder ? 1 : 0);
    int offset = base_Np * rank + std::min(rank, remainder);

    std::vector<double> x_local(local_Np), y_local(local_Np);
    std::mt19937 gen(rank + 1);
    std::normal_distribution<> noise(0.0, 1.0);
    for (int i = 0; i < local_Np; ++i) {
        x_local[i] = noise(gen);
        y_local[i] = noise(gen);
    }

    stepper state(x_local, y_local, dt, D, G, g, m, h, mob, gen);
    int Nt = std::ceil(T / dt);
    double t = 0;

    std::vector<int> counts(size), displs(size);
    for (int i = 0; i < size; ++i) {
        counts[i] = base_Np + (i < remainder ? 1 : 0);
        displs[i] = base_Np * i + std::min(i, remainder);
    }

    std::vector<double> x_all, y_all;
    if (rank == 0) {
        x_all.resize(Np);
        y_all.resize(Np);
    }

    for (int it = 0; it < Nt; ++it) {
        for (int n = 0; n < local_Np; ++n) {
            state(n);
        }

        MPI_Gatherv(x_local.data(), local_Np, MPI_DOUBLE,
                    x_all.data(), counts.data(), displs.data(), MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
        MPI_Gatherv(y_local.data(), local_Np, MPI_DOUBLE,
                    y_all.data(), counts.data(), displs.data(), MPI_DOUBLE,
                    0, MPI_COMM_WORLD);

        if (rank == 0) {
            std::ofstream out("trajectory_t" + std::to_string(it) + ".csv");
            for (int i = 0; i < Np; ++i)
                out << x_all[i] << "," << y_all[i] << "\n";
            out.close();
        }

        t += dt;
    }

    MPI_Finalize();
    return 0;
}
