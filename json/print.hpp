#ifndef JSON_PRINT_HPP_
#define JSON_PRINT_HPP_

#include "variant.hpp"
#include <boost/variant/static_visitor.hpp>

namespace yangacer {
namespace json {

struct print
: boost::static_visitor<>
{
  enum mode_t { compact=0, pretty };

  print(std::ostream &os, mode_t mode=pretty);

  template<typename T>
  void operator()(T const& v) const  { (*os_)<<v; }
 
  void operator()(boost::blank const &b) const;

  void operator()(bool const b) const;

  void operator()(std::string const& s) const;

  void operator()(array_t const &v) const;

  void operator()(object_t const &m) const;

private:
  std::ostream *os_;
  mode_t mode_;
  mutable int depth_;
};

} // namespace json
} // namespace yangacer

#endif // header guard
