
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

    int local_Np = Np / size;
    int remainder = Np % size;
    if (rank < remainder) local_Np++;
    int offset = (Np / size) * rank + std::min(rank, remainder);

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

    for (int it = 0; it < Nt; ++it) {
        for (int n = 0; n < local_Np; ++n) {
            state(n);
        }
	{
	  std::ofstream out("trajectory_rank_" + std::to_string(rank) + "_t" + std::to_string(it) + ".csv");
	  for (int i = 0; i < local_Np; ++i)
	    out << x_local[i] << "," << y_local[i] << "\n";
	  out.close();
	}
        t += dt;
    }

    MPI_Finalize();
    return 0;

}

