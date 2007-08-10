/*
* Print.hpp
*/

#include <iostream>

template <class T>
inline void PRINT_ELEMENTS(const T& coll, const char* optstr = "")
{
  typename T::const_iterator iter;

  std::cout << optstr;
  for( iter = coll.begin(); iter != coll.end(); ++iter )
    std::cout << *iter << ' ';

  std::cout << std::endl;
}
