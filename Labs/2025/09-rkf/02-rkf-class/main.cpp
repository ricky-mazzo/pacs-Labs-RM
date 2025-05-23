#include "RKF.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

int
main(int argc, char **argv)
{
  using namespace std;

  {
    auto fun = [](double const &t, double const &y) {
      return -10 * y;
    };
    auto exact = [](double const &t) { return std::exp(-10. * t); };

    double t0           = 0;
    double y0           = 1;
    double T            = 100;
    double h_init       = 0.2;
    double errorDesired = 1.e-4;

    RKF <RKFScheme::RK45_t, RKFKind::SCALAR> solver {RKFScheme::RK45, fun};
    
    auto solution = solver (t0, T, y0, h_init, errorDesired);
    

    ofstream file2("results_exp.dat");
    file2 << solution;
  }
  /*
  {
    // Van Der Pol with mu=1
    auto fun = [](double const &  t,
                  Eigen::VectorXd y) -> Eigen::VectorXd {
      constexpr double mu = 1.;
      Eigen::VectorXd  out(2);
      out(0) = y(1);
      out(1) = -y(0) + mu * (1 - y(0) * y(0)) * y(1);
      return out;
    };

    
    //TBD
    
    ofstream file3("results_VDP.dat");
    file3 << solution;
  }*/

  return 0;
}
