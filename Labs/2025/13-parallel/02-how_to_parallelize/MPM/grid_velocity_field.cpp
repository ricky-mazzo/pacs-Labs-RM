#include <json.hpp>
#include <particles.h>

#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <vector>

#include "counter.h"

class
stepper {
private :
  std::vector<double> &x;
  std::vector<double> &y;
  std::vector<double> &vx;
  std::vector<double> &vy;

  double dt, D; 
  std::function<double (void)> normal;
  
public :
  
  stepper (std::vector<double> &x_, std::vector<double> &y_,
	   std::vector<double> &vx_, std::vector<double> &vy_,
	   double dt_, double D_, std::function<double (void)> &noise_)
    : x(x_), y(y_), vx(vx_), vy(vy_), dt{dt_}, D{D_}, normal(noise_) { };
  
  void operator() (int n) {
    double dxb, dyb;
  
    //Brownian motion displacements
    dxb=normal() * std::sqrt (2*D*dt);
    dyb=normal() * std::sqrt (2*D*dt);
 
    //update particles positions
    x[n] += vx[n] * dt + dxb;
    y[n] += vy[n] * dt + dyb;
      
    // Apply boundary conditions (unelastic walls)
    y[n] = std::min (1.999, std::max (0.001, y[n]));
    x[n] = std::min (1.999, std::max (0.001, x[n]));
  }; 

};


int
main () {

  // read data from file
  const std::string filename = "velocity.json";

  nlohmann::json j;
  std::ifstream inbuf (filename);

  inbuf >> j;
  quadgrid_t<std::vector<double>> qg (j["grid_properties"]);
  particles_t p (j, qg);
  p.build_mass ();
  std::map<std::string, std::vector<double>> vars= 
    j["grid_vars"].get<std::map<std::string, std::vector<double>>> ();
    
  inbuf.close ();


  std::random_device rd2;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen2;
  std::normal_distribution<> normal;
  std::function<double ()> noise = [&gen2, &rd2, &normal] () { return normal(gen2); };

  stepper state (p.x, p.y, p.dprops["VX"], p.dprops["VY"], 1., 1.e-5, noise);
  p.init_particle_mesh ();
  
  for (int it = 0; it < 1000; ++it) {

    

    std::fill(p.dprops["VX"].begin (), p.dprops["VX"].end (), 0.0);
    std::fill(p.dprops["VY"].begin (), p.dprops["VY"].end (), 0.0);
    p.g2p (vars, {"vx", "vy"}, {"VX", "VY"});

#pragma omp parallel for
    for (int ip = 0; ip < p.num_particles; ++ip) {
      state(ip);
    }

    p.init_particle_mesh ();
    std::fill(vars["rho"].begin (), vars["rho"].end (), 0.0);
    p.p2g (vars, {"M"}, {"rho"}, true);

#ifdef SAVE_OUTPUT
    if (it % 50 == 0) 
    {
      // write particle data to file
      const std::string ofilename = "particle";
      const std::string ofileext = ".csv";
      const std::string numfile = std::string(".") + std::to_string(it);
      
      std::ofstream outbuf (ofilename + numfile + ofileext);  
      p.print<particles_t::output_format::csv> (outbuf);

      outbuf.close ();

      // write grid data to file
      const std::string gfilename = std::string("grid.") + std::to_string(it) + std::string(".vts");
      qg.vtk_export (gfilename.c_str(), vars);
      
    }
#endif
    
  }

  return 0;
}
