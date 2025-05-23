#include "matrix.h"
#include <cassert>
#include <algorithm>
#include <ctime>

#define dgemm dgemm_
#define DGEMM dgemm_

extern "C"
{
  void
  dgemm (const char *TRANSA, const char *TRANSB, const int *M,
         const int *N, const int *K, const double *ALPHA,
         const double *A, const int *LDA, const double *B,
         const int *LDB, const double *BETA, double *C,
         const int *LDC);
}

#ifdef USE_DGEMM
matrix
operator* (const matrix& A, const matrix& B)
{

  int M = A.get_rows ();
  int N = B.get_cols ();
  int K = A.get_cols ();
  assert (K == B.get_rows ());
  
  char ntr = 'n';
  double one = 1.0;
  double zero = 0.0;

  matrix retval (M, N);
  
  dgemm (&ntr, &ntr, &M, &N, &K,
         &one, A.get_data (), &M,
         B.get_data (), &K, &zero,
         retval.get_data (), &M);

  return (retval);
}
#else
matrix
operator* (const matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());
  for (unsigned int i = 0; i < retval.get_rows (); ++i)
    for (unsigned int j = 0; j < retval.get_cols (); ++j)
      for (unsigned int k = 0; k < A.get_cols (); ++k)
        retval(i,j) += A(i,k) * B(k,j);
  return (retval);
}
#endif
