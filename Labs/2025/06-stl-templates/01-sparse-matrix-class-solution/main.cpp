#include <sparse_matrix.hpp>

#include <iostream>
#include <vector>

int main (){

  const int n = 10;
  
  //A*x A_ij x_j
  std::vector <double> x (n, 1.0);
 
  sparse_matrix A;
  A.resize (n);
  
  for (size_t i = 0; i<A.rows (); ++i)
    {
      if (i > 0)
        A[i][i-1] = -1;
      
      if (i<n-1)
        A[i][i+1] = -1;
        
      A[i][i] = 4;
    }
    
  std::cout << A << std::endl;
  
  std::cout << "\nx = " << std::endl;
  for (const auto & r : x)
    std::cout << r << "\n";
  
  std::vector<double> y = A*x;
  std::cout << "\nA*x = " << std::endl;
  for (const auto & r : y)
    std::cout << r << "\n";
  std::cout <<std::endl;

  return 0;
}