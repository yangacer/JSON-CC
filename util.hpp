#ifndef JSON_UTIL_HPP_
#define JSON_UTIL_HPP_

#include "variant.hpp"
#include <boost/variant/static_visitor.hpp>

namespace yangacer {
namespace json {

struct print
: boost::static_visitor<>
{
  print(std::ostream &os):os_(&os),depth_(0){}

  template<typename T>
  void operator()(T const& v) const
  { (*os_)<<v; }
  
  void operator()(boost::int64_t const& i) const;

  void operator()(bool const b) const;

  void operator()(std::string const& s) const;

  void operator()(array_t const &v) const;

  void operator()(object_t const &m) const;

private:
  std::ostream *os_;
  mutable int depth_;
};

}} // namespace yangacer::json

#endif
