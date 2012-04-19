#ifndef JSON_UTIL_HPP_
#define JSON_UTIL_HPP_

#include "json.hpp"
#include <boost/variant/static_visitor.hpp>
#include <boost/spirit/include/karma.hpp>

namespace yangacer {
namespace json {
namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct escaped_string
: karma::grammar<OutputIterator, std::string()>
{
    escaped_string();
    karma::rule<OutputIterator, std::string()> esc_str;
    karma::symbols<char, char const*> esc_char;
};

struct print
: boost::static_visitor<>
{
  print(std::ostream &os):os_(&os){}

  template<typename T>
  void operator()(T const& v) const
  { (*os_)<<v; }
  
  void operator()(void*) const;

  void operator()(bool const b) const;

  void operator()(std::string const& s) const;

  void operator()(array_t const &v) const;

  void operator()(object_t const &m) const;

  static escaped_string<boost::spirit::ostream_iterator> str_gen;
  std::ostream *os_;
};

}} // namespace yangacer::json

#endif
