#ifndef JSON_UTIL_HPP_
#define JSON_UTIL_HPP_

#include "variant.hpp"
#include "print.hpp"

namespace yangacer {
namespace json {

void pretty_print(std::ostream &os, 
                  var_t const & var, 
                  print::mode_t mode=print::pretty);


}} // namespace yangacer::json

#endif
