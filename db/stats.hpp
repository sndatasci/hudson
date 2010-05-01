#ifndef _STATS_HPP_
#define _STATS_HPP_

// STD
#include <iostream>

#include "args.hpp"

class stats
{
public:
  stats(const args& a);

  bool calc();
  std::ostream& print(std::ostream& os) const;

protected:
  args m_a;
};

inline std::ostream& operator<<(std::ostream& os, const stats& s)
{
  return s.print(os);
}

#endif // _STATS_HPP_
