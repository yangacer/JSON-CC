#ifndef JSON_UTIL_HPP_
#define JSON_UTIL_HPP_

#include "variant.hpp"

namespace yangacer {
namespace json {

void pretty_print(std::ostream &os, var_t const & var);


}} // namespace yangacer::json

#endif
