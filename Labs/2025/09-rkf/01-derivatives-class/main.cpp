#include "Derivatives.hpp"

#include <cmath>
#include <iostream>

#include <functional>

int main (){

  double h = 0.01;
  
  std::function<double (const double &)> f = [] (const double & x) 
    {return std::exp(x);};
  
  std::cout << "Approx. 3rd order der. of exp(x) at x = 1 is: " <<
               NthDerivative <3, decltype (f)> {f, h} (1.)
            << std::endl;
            
  return 0;

}