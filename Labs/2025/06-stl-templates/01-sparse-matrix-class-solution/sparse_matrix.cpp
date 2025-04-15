#include "sparse_matrix.hpp"

void
sparse_matrix::init (){
    m = 0;
    nnz = 0;
};

void 
sparse_matrix::update_properties ()
{
  //sparse_matrix::col_iter j;
  init ();
  
  for (size_t i=0; i<size (); ++i)
  {
    if ((*this)[i].size () >0)
    {
      nnz += (*this)[i].size ();
      
      size_t tmp = (*this)[i].rbegin () -> first + 1; //loop over j
      
      m = std::max (m,tmp); 
    }
  }
}


std::ostream &
operator << (std::ostream &stream, sparse_matrix &M)
{
  sparse_matrix::col_iter j;
  
  M.update_properties ();
  stream << "nrows = " << M.rows () << "; ncols = " << M.cols ();
  stream << "; nnz = " << M.nnz << ";" << std::endl;
  
  stream << "matrix = [ ";
  for (size_t i = 0; i < M.size (); ++i)
    if (M[i].size ())
      for (j = M[i].begin (); j!=M[i].end (); ++j)
      {
         stream << i+1 << ", " << M.col_idx (j) + 1 << ", ";
         stream << M.col_value (j) << "; " << std::endl;
      }
  stream << "]";//<<std::endl;
  
  return stream;
}

void 
sparse_matrix::operator += (sparse_matrix &rhs) // A += B;
{

  //pre-conditions 
  assert (rows () == rhs.rows ()); //turn it off CPPFLAGS... -DNDEBUG
  assert (cols () == rhs.cols ());
  
  //+=
  for (size_t i = 0; i < rhs.size (); ++i)
    if (rhs[i].size ())
      for (col_iter j = rhs[i].begin (); j != rhs[i].end (); ++j)
        (*this)[i][j->first] += rhs.col_value (j); //std::map<...>::operator[] which is not .at()
  
  //post-conditions 
  assert (rows () == rhs.rows ()); //turn it off CPPFLAGS... -DNDEBUG
  assert (cols () == rhs.cols ());
}
  
std::vector <double> 
operator*(sparse_matrix & M, const std::vector<double> &x) //yi = Aij xj
{
  M.update_properties ();
  //assert
  std::vector <double> y (M.rows (), 0.0);
  sparse_matrix::col_iter j;
  for (size_t i = 0; i < M.size (); ++i)
    if (M[i].size())
      for (j = M[i].begin (); j!= M[i].end (); ++j)
        y[i] += M.col_value (j) * x [M.col_idx (j)];
  return y;
}

void 
sparse_matrix::csr (std::vector <double> &a,
                    std::vector <int> &col_ind,
                    std::vector <int> &row_ptr)
{
  int idx =0, idr =1;
  sparse_matrix::col_iter j;
  
  for (size_t i=0; i < size (); ++i)
  {
    if ((*this)[i].size ())
      {
        for (j = (*this)[i].begin (); j != (*this)[i].end (); ++j)
          {
            col_ind[idx] = col_idx(j);
            a[idx] = col_value (j);
            ++idx;
          }
      }
    row_ptr [idr++] = idx; //rmk last element
   // ++idr;
  }
}



















