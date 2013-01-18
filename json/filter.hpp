#ifndef JSON_FILTER_HPP_
#define JSON_FILTER_HPP_

#include "variant.hpp"
#include <boost/regex.hpp>

namespace yangacer {
namespace json {
  
  void inplace_field_filter(object_t &o, boost::regex const& regex);
  void inplace_field_filter(object_t &o, boost::regex const& regex, int depth_begin, int depth_end);

} // namespace json
} // namespace yangacer

#endif // header guard
