#include <algorithm>
#include <exception>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <random> 
#include <vector>

//#define PSTLD_HEADER_ONLY   // no prebuilt library, only the header
//#define PSTLD_HACK_INTO_STD // export into namespace std
//#include <pstld.h>

void fluid_velocity (double x, double y, double G, double mu, double h, double& vx, double& vy){ 
  vx = G * y * (h - y);
  vy = 0;
}


void particle_force (double x, double y, double g, double m, double& fx, double& fy){
  fx = 0;
  fy = -m*g;
}

class
stepper {
private :
  std::vector<double> &x;
  std::vector<double> &y;
  double G, mu, h, KT, r, mob, D, dt, g, m; 
  std::function<double (void)> normal;
  
public :
  
  stepper (std::vector<double> &x_, std::vector<double> &y_, double G_, double mu_,
	   double h_, double KT_, double r_, double mob_, double D_, double dt_, double g_,
	   double m_, std::function<double (void)> &noise_)
    : x(x_), y(y_), G{G_}, mu{mu_}, h{h_}, KT{KT_}, r{r_}, mob{mob_}, D{D_}, dt{dt_}, g{g_},
      m{m_}, normal(noise_) { };
  
  void operator() (int n) {
    double vx, vy;
    double fx, fy;
    double dxb, dyb;
    
    // Compute fluid velocity and forces  
    fluid_velocity (x[n], y[n], G, mu, h, vx, vy);
    particle_force (x[n], y[n], g, m, fx, fy);
  
    //Brownian motion displacements
    dxb=normal() * std::sqrt (2*D*dt);
    dyb=normal() * std::sqrt (2*D*dt);
 
    //update particles positions
    x[n] += (vx + mob*fx) * dt + dxb;
    y[n] += (vy + mob*fy) * dt + dyb;
      
    // Apply boundary conditions (unelastic walls)
    y[n] = std::min (h, std::max (0.0, y[n]));
  };

  
  void save(const std::string &filename) const {
    std::ofstream outFile (filename);
    int Np = x.size ();
    
    // Check if the file is open
    if (!outFile.is_open()) {
      std::cerr << "Error opening output file." << std::endl;
    }
  
    // Write the header for the CSV file
    outFile << "Ball_X, Ball_Y \n";

       
    // Write particle position and time to the output file
    for (int n = 0; n < Np; ++n){
      outFile << x[n] << "," << y[n] << "\n"; 
    }
      
    //Close the time-step-specific file 
    outFile.close();
  }
};


int main() {

  double        G   = 50000;                    // [kg s^-2 m^-2] head
  double        mu  = 1;                        // [kg s^-1 m_1] viscosity
  double        h   = .2;                       // [m] thickness
  double        KT  = .1;                       // [J] thermal energy
  double        r   = 1e-2;                     // [m] particle radius
  double        mob = 1. / 6. / M_PI / mu / r;  // [s kg^-1] mobility
  double        D   = 1;                 // [m^2 s^-1] diffusivity
  int           Np = 100000;                     // [-] number of particles
  double        dt = 1e-6;                      // [s] time step
  double        T  = 0.02;                     // [s] simulation time
  double        g  = 0;                         // [m s^-2] gravity
  double        m  = 2;                         // [kg] particle mass

  std::vector<double> x (Np);
  std::vector<double> y (Np);
    
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> uniform(0.0, 1.);
    
  // initialize particle positions
  for (int n = 0; n < Np; ++n) {  
    x[n]=uniform(gen) * 1.;
    y[n]=uniform(gen) * h;
  }

  std::random_device rd2;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen2;
  std::normal_distribution<> normal;
  std::function<double ()> noise = [&gen2, &rd2, &normal] () { return normal(gen2); };
    
  stepper state (x, y, G, mu, h, KT, r, mob, D, dt, g, m, noise);

  //Calculate the number of time steps
  int Nt = std::ceil(T/dt);

  double t = 0; //initial time
  int iff = 0;  //File index

  //Main simulation loop
  bool printframe = false;
  std::ofstream outFile;
  for (int it = 0; it<Nt; ++it){    
    
    // Loop over all particles
    for (n=0; n<Np; ++Np)
      state(n);
      
    printframe = false;
    if ((it % 1000) == 0) printframe = true;

    if (printframe) {
      //Create a new CSV file for each time step    
      auto particle_move_n = std::to_string(iff++);
      std::string particle_move = "particle_position_00000.csv";
      particle_move.replace (23-particle_move_n.length (), particle_move_n.length (), particle_move_n);  
      state.save (particle_move);      
    }

    t += dt;
  }  
    
  std::cout << "Simulation complete. Data saved to particle_positions.csv.\n"; 
  return 0;
}
