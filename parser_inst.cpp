#include "parser_def.hpp"

namespace yangacer{
namespace json{

template grammar<std::string::const_iterator>::grammar();
template grammar<std::string::iterator>::grammar();
template grammar<char const*>::grammar();
template grammar<istream_iterator>::grammar();

}} // namespace yangacer::json

namespace boost {
//template make_recursive_variant_over<yangacer::json::types>::type::variant();
} // namespace boost
