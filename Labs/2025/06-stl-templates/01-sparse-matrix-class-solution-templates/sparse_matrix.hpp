#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>

#include <map>
#include <vector>
#include <cassert>

/*
A[i][j]
A[i] row
A[i][j] j-th elem

matrix as std::vector < std::map < int, double > > 
A[i] vector
A[i][j] map
A[i][j].first
*/

template < /*class*/ typename T /* = double*/> 
class sparse_matrix : public std::vector <std::map <int, T>>
{
  using col_type = std::map <int, T>;
  using row_type = std::vector <col_type>;
  
  size_t m;
  size_t nnz;
  
  void init ();
  
  void update_properties ();
  
public :
  using row_iter = typename row_type::iterator;
  using col_iter = typename col_type::iterator;
  
  sparse_matrix () { init();}
  
  inline int
  col_idx (const col_iter &j) const {return j->first;}
  
  inline T
  col_value (const col_iter &j) const {return j->second;}
  
  inline const size_t rows () const {return this->size ();} // Rmk: compare with templates solutions
  
  inline const size_t cols () const {return m;}
  
  //aij
  
  void 
  csr (std::vector <T> &a,
       std::vector <int> &col_ind,
       std::vector <int> &row_ptr);
  
  void 
  operator += (sparse_matrix<T> &rhs);
  
  template <typename U> friend std::vector <U> 
  operator*(sparse_matrix<U> & M, const std::vector<U> &x);
  
  template <typename U> friend std::ostream & 
  operator << (std::ostream & os, sparse_matrix<U> &M); //return os;
  
};

#include <sparse_matrix_impl.hpp>

#endif
