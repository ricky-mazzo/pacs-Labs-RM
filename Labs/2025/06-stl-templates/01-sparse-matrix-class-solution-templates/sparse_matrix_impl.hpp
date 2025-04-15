
template <class T> void
sparse_matrix<T>::init (){
    m = 0;
    nnz = 0;
};

template <class T> void 
sparse_matrix<T>::update_properties ()
{
  //sparse_matrix::col_iter j;
  init ();
  
  for (size_t i=0; i<this -> size (); ++i)
  {
    if ((*this)[i].size () >0)
    {
      nnz += (*this)[i].size ();
      
      size_t tmp = (*this)[i].rbegin () -> first + 1; //loop over j
      
      m = std::max (m,tmp); 
    }
  }
}


template <class T> std::ostream &
operator << (std::ostream &stream, sparse_matrix<T>  &M)
{
  typename sparse_matrix<T>::col_iter j;
  
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

template <class T> void 
sparse_matrix<T>::operator += (sparse_matrix<T> &rhs) // A += B;
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
  
template <class T> std::vector <T> 
operator*(sparse_matrix<T> & M, const std::vector<T> &x) //yi = Aij xj
{
  M.update_properties ();
  //assert
  typename std::vector <T> y (M.rows (), 0.0);
  typename sparse_matrix <T>::col_iter j;
  for (size_t i = 0; i < M.size (); ++i)
    if (M[i].size())
      for (j = M[i].begin (); j!= M[i].end (); ++j)
        y[i] += M.col_value (j) * x [M.col_idx (j)];
  return y;
}

template <class T> void 
sparse_matrix<T>::csr (std::vector <T> &a,
                    std::vector <int> &col_ind,
                    std::vector <int> &row_ptr)
{
  int idx =0, idr =1;
  sparse_matrix<T>::col_iter j;
  
  for (size_t i=0; i < this->size (); ++i)
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



















