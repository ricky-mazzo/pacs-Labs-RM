
#ifndef STEPPER_H
#define STEPPER_H

#include <vector>
#include <random>
#include <cmath>

class stepper {

private:
  
  static void
  fluid_velocity (double x, double y, double G, double h, double& vx, double& vy){ 
    vx = G * y * (h - y);
    vy = 0;
  }
  

  static void
  particle_force (double x, double y, double g, double m, double& fx, double& fy){
    fx = 0;
    fy = -m*g;
  }

public:
  stepper(std::vector<double>& x, std::vector<double>& y,
	  double dt, double D,  double G,  double g, double m, double h, double mob,
	  std::mt19937& gen) : x_(x), y_(y), dt_(dt), D_(D), G_(G), g_(g), m_(m), h_(h),
			       mob_(mob), noise_(0.0, std::sqrt(2 * D * dt)), gen_(gen) {}

  void operator()(int i) {
    
    // Compute fluid velocity and forces  
    fluid_velocity (x_[i], y_[i], G_, h_, vx, vy);
    particle_force (x_[i], y_[i], g_, m_, fx, fy);
    
    x_[i] += (vx + mob_*fx) * dt_ + noise_(gen_);
    y_[i] += (vx + mob_*fx) * dt_ + noise_(gen_);
    
  }

private:
  std::vector<double>& x_;
  std::vector<double>& y_;
  double vx, vy, fx, fy;
  double dt_;
  double D_;
  double G_;
  double g_;
  double m_;
  double h_;
  double mob_;
  std::normal_distribution<> noise_;
  std::mt19937& gen_;
};

#endif // STEPPER_H
