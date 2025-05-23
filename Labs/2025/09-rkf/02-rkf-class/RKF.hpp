#ifndef RKF_HPP
#define RKF_HPP

#include "ButcherRKF.hpp"
#include "RKFTraits.hpp"

#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>
#include <utility>
  
template <RKFKind KIND>
struct RKFResult
{
  std::vector <double> time; 
  std::vector <typename RKFTraits<KIND>::VariableType> y;
  size_t expansions = 0;
  size_t contractions = 0;
  double estimatedError = 0.0;
  bool failed = false;
};


template <class B,RKFKind KIND = RKFKind::SCALAR>
class RKF : public RKFTraits<KIND>
{
  public:
    using VariableType = typename RKFTraits<KIND>::VariableType;
    using Function = typename RKFTraits<KIND>::ForcingTermType;
    
    RKF (B const & bt, Function const &f):
      M_f (f), ButcherTable (bt){};
    
    RKF () = default;
    
    void set_function (const Function & f){M_f = f;}
    void set_ButcherArray (const B & bt) {ButcherTable = bt;}
    
    RKFResult <KIND> //missing
    operator () (const double & T0,
                 const double & T,
                 VariableType & y0,
                 const double & hInit,
                 const double & tol,
                 const size_t maxStep = 2000) const;
  
  private:
    Function M_f;
    B ButcherTable;
    
    /*std::pair<VariableType, VariableType>*/
    auto 
    RKFstep (const double & tstart,
             const VariableType & y0,
             const double & h) const
    -> std::pair <VariableType, VariableType>;
};

inline std::ostream &
operator<<(std::ostream &out, const RKFResult<RKFKind::SCALAR> &res);

inline std::ostream &
operator<<(std::ostream &out, const RKFResult<RKFKind::VECTOR> &res);

// Implementations.

/*std::pair<typename RKFTraits::VariableType, ypename RKFTraits::VariableType>*/
template <class B, RKFKind KIND>
auto 
RKF<B, KIND> :: RKFstep (const double & tstart,
                         const VariableType & y0,
                         const double & h) const
    -> std::pair <VariableType, VariableType>
{

  auto constexpr Nstages = B::Nstages ();
  std::array <VariableType, Nstages> K;
  
  //references
  typename B::Atable const & A = ButcherTable.A;
  std::array <double, Nstages> const & c{ButcherTable.c};
  std::array <double, Nstages> const & b1{ButcherTable.b1};
  std::array <double, Nstages> const & b2{ButcherTable.b2};
  
  //Euler step
  K[0] = M_f (tstart, y0) * h;
  
  
  for (size_t i = 1; i < Nstages; ++i)
  {
    double time = tstart + c[i] * h;
    VariableType value = y0;
    for (size_t j = 0; j < i; ++j)
      value += A[i][j] * K[j];
    K[i] = M_f (time, value) * h;
  }
  
  VariableType v1 = y0;
  VariableType v2 = y0;
  
  for (size_t i = 0; i < Nstages; ++i)
  {
    v1 += K[i] * b1[i];
    v2 += K[i] * b2[i];
  }
  return std::make_pair (v1, v2);
}
template <class B, RKFKind KIND>
RKFResult <KIND>
RKF<B, KIND> :: operator () (const double & T0,
                             const double & T,
                             VariableType & y0,
                             const double & hInit,
                             const double & tol,
                             const size_t maxSteps) const
{
  RKFResult <KIND> res;
  //aliases
  std::vector <double> & time = res.time; 
  std::vector <VariableType> & y = res.y;
  auto & expansions = res.expansions;
  auto & contractions = res.contractions;
  auto & estimatedError = res.estimatedError; /**/
  auto &failed = res.failed;
  
  //init before loop
  estimatedError = 0.0;
  failed = false;
  
  int expectedSteps = std::min (std::max (static_cast <size_t> (1), 1+static_cast <size_t> ((T-T0)/hInit)),maxSteps);
  time.reserve (expectedSteps);
  y.reserve (expectedSteps);
  
  time.push_back (T0);
  y.push_back (y0);
  
  bool rejected (false);
  double constexpr reductionFactor = 0.95;
  double constexpr expansionFactor = 2.;
  
  double factor;
  if (ButcherTable.order <= 2)
    factor = 1.0;
  else 
    factor = 1./(ButcherTable.order - 1);
  
  int iter = 0;
  
  //converge if h "very" small
  double hmin = 100 * (T-T0) * std::numeric_limits<double>::epsilon ();
  double h = std::max (hInit, hmin);
  double t = T0;
  VariableType ycurr = y0;
  double delta = T-T0;
  
  while ( t < T && iter <= maxSteps)
  {
    ++iter;
    
    VariableType ylow, yhigh;
    
    double errorPerTimeStep = tol * h / delta;
    
    if (t+h>=T)
    {
      h = T - t;
      if (h < hmin)
      {
        ylow = ycurr;
        yhigh = ycurr;
      }
      else
        std::tie (ylow, yhigh) = RKFstep (t, ycurr, h); //last step
    }
    else
      std::tie (ylow, yhigh) = RKFstep (t, ycurr, h);
    
    double currentError = this -> norm (ylow - yhigh);
  /*}*/
    double mu = std::pow (errorPerTimeStep / currentError, factor);
    
    if (currentError <= errorPerTimeStep)
    {
      t = t+h;
      time.push_back (yhigh);
      ycurr = yhigh;
      estimatedError += currentError;
      if ( (mu >= 1.) && !rejected && (t<T))
      {
        h *= std::min (expansionFactor, mu);
        ++expansions;
      }
      rejected = false;
      } /**/
      else
        {
          rejected = true;
          h *= mu * reductionFactor;
          ++contractions;
          h = h <= hmin ? hmin : h;
        }
    }
    
    if (iter > maxSteps)
      failed = true;
      
   
   return res;
  
}

std::ostream &
operator<<(std::ostream &out, const RKFResult<RKFKind::SCALAR> &res)
{
  out << "# Number ot time steps:" << res.time.size()
      << " N. contractions:" << res.contractions
      << " N. expansions:" << res.expansions << std::endl;
  out << "#   t    y   Estimated error=" << res.estimatedError
      << std::endl;
  double hmin = res.time[1] - res.time[0];
  double hmax = hmin;
  for (unsigned int i = 0; i < res.time.size() - 1; ++i)
    {
      auto delta = res.time[i + 1] - res.time[i];
      hmax       = std::max(hmax, delta);
      hmin       = std::min(hmin, delta);
    }
  out << "# hmin:" << hmin << " hmax:" << hmax << std::endl;
  std::size_t i = 0;
  for (auto const &t : res.time)
    out << t << " " << res.y[i++] << "\n";
  return out;
}

#endif /* RKF_HPP */
