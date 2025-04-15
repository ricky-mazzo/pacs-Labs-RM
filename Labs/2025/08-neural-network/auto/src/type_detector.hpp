#ifndef TYPE_DETECT_H
#define TYPE_DETECT_H

#ifdef CPP17
#include <string_view>

template <typename T>
constexpr auto type_name() {
  std::string_view name, prefix, suffix;
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}
#else

#include <type_traits>
#include <typeinfo>
#include <memory>
#include <string>
#include <cstdlib>

template <class T>
std::string type_name(){
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own(nullptr,std::free);
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

#endif

#endif
