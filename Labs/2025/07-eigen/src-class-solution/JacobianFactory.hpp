#ifndef JACOBIANFACTORY_HPP
#define JACOBIANFACTORY_HPP

#include "Jacobian.hpp"
#include <type_traits>
#include <memory>

      //auto jac_full = make_jacobian<JacobianType::Full>(jacobian_fun);
      //std::unique_ptr <JacobianBase> jac_full = std::make_unique <FullJacobian> (jacobian_fun);

      //auto jac_discrete = make_jacobian<JacobianType::Discrete>(system, h);
      
      //std::unique_ptr <JacobianBase> jac_discrete =  std::make_unique <DiscreteJacobian> (system, h);

template <class JacobianType, class ... Args>
std::unique_ptr <JacobianBase> 
make_jacobian ( const Args & ... args)
{
  if constexpr( std::is_same_v <JacobianType, FullJacobian> )
    return std::make_unique <FullJacobian> (args...);
  else
    return std::make_unique <DiscreteJacobian> (args...);
}

#endif /* JACOBIANFACTORY_HPP */
