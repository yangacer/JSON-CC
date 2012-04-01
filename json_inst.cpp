#include "json_def.hpp"

namespace yangacer{
namespace json{

template grammar<std::string::const_iterator>::grammar();
template grammar<char const*>::grammar();

}} // namespace yangacer::json
