#ifndef _ARGS_HPP_
#define _ARGS_HPP_

// STD
#include <string>

struct Args
{
  std::string database;
  std::string table;
  unsigned cpus;

  std::string config;
  std::string symbol;
  std::string begin_date;
  std::string end_date;
};

#endif // _ARGS_HPP_
