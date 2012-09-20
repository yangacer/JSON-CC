#include "util.hpp"
#include "print.hpp"

namespace yangacer {
namespace json {

void pretty_print(
  std::ostream &os, 
  var_t const & var)
{
  print pr(os);
  boost::apply_visitor(pr, var);
}

}} // namespace yangacer::json
