#ifndef RKFTRAITS_HPP
#define RKFTRAITS_HPP

#include <Eigen/Dense>

#include <functional>
#include <cmath>

enum class RKFKind
{
  SCALAR = 0,
  VECTOR = 1,
  MATRIX = 2
};

template <RKFKind Kind>
struct RKFTraits {};

//scalars
template <>
struct RKFTraits <RKFKind :: SCALAR>
{
  using VariableType = double;
  using ForcingTermType = std::function <VariableType(const double &, const VariableType &)>; //t, y
  
  //we won't do 
  //object.norm (...)
  static double norm (VariableType const & x) {return std::abs(x);}
};

//vectors
template <>
struct RKFTraits <RKFKind :: VECTOR>
{
  using VariableType = Eigen::VectorXd;
  using ForcingTermType = std::function <VariableType(const double &, const VariableType &)>; //t, y
  
  //we won't do 
  //object.norm (...)
  static double norm (VariableType const & x) {return x.norm ();}
};

//matrices
template <>
struct RKFTraits <RKFKind :: MATRIX>
{
  using VariableType = Eigen::MatrixXd;
  using ForcingTermType = std::function <VariableType(const double &, const VariableType &)>; //t, y
  
  //we won't do 
  //object.norm (...)
  static double norm (VariableType const & x) {return x.norm ();}
  // RKFTr... :: norm 
};

#endif /* RKFTRAITS_HPP */
