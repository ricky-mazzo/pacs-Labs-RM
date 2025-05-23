#ifndef Derivatives
#define Derivatives

#include <functional>
#include <type_traits>

namespace DifferenceType
{
  struct FORWARD; //forward declarations

  struct BACKWARD {
    using otherType = FORWARD;
   };
  
  struct FORWARD {
    using otherType = BACKWARD;
  };
}

template <unsigned N,
          typename F,
          typename T = double,
          typename DT = DifferenceType::FORWARD>
class NthDerivative 
{
  public:
    using PreviousDerivative = NthDerivative <N-1, F, T, DT>;//typename DT::otherType>;
    
    NthDerivative (const F & f, const T & h_) : pDerivative {f,h_}, h(h_) {} //not pDerivative (f) only!
  
    T operator () (const T &x) const
    { 
      if constexpr (std::is_same<DifferenceType::FORWARD, DT>::value) //is_same_v
        return (pDerivative (x+h) - pDerivative (x) )/h;
      else
        return (pDerivative (x) - pDerivative (x-h) )/h;
    }
  
  private:
    PreviousDerivative pDerivative;
    T h;
  
};

// Specialisation for first order derivative
// to stop recursive template class istantation...

template <//unsigned N,
          typename F,
          typename T,
          typename DT>
class NthDerivative <1u, F, T, DT>
{
  public:
    
    NthDerivative (const F & f, const T & h_) : pDerivative {f}, h(h_) {}
  
    T operator () (const T &x) const
    { 
      if constexpr (std::is_same<DifferenceType::FORWARD, DT>::value) //is_same_v
        return (pDerivative (x+h) - pDerivative (x) )/h;
      else
        return (pDerivative (x) - pDerivative (x-h) )/h;
    }
  
  private:
    F pDerivative;
    T h;
  
};



#endif
