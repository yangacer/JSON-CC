#ifndef JSON_PARSER_HPP_
#define JSON_PARSER_HPP_

#include "variant.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

namespace yangacer {
namespace json {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using qi::parse;
using qi::phrase_parse;
using qi::space;
using boost::spirit::istream_iterator;

/*
typedef boost::mpl::vector<
  bool,
  int,
  int64_t,
  double,
  std::string,
  void*,
  std::deque<boost::recursive_variant_>,
  std::map<std::string, boost::recursive_variant_ >
  >::type types;

// variable type
typedef boost::make_recursive_variant_over<types>::type var_t;

// array type
typedef std::deque<var_t> array_t;

// object type
typedef std::map<std::string, var_t> object_t;
*/

template<typename Iterator = std::string::const_iterator>
struct grammar
: qi::grammar<Iterator, object_t()>
{
  grammar();

  qi::rule<Iterator, object_t()> object_r;
  qi::rule<Iterator, array_t()> array_r;
  qi::rule<Iterator, std::pair<object_t::key_type, object_t::mapped_type>() > pair_r;
  qi::rule<Iterator, std::string()> string_r;
  qi::rule<Iterator, var_t()> var_r;

  qi::symbols<char const, char const> unesc_char;

};

} }// namespace yangacer::json

#endif // header guard