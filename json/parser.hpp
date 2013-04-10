#ifndef JSON_PARSER_HPP_
#define JSON_PARSER_HPP_
#include "variant.hpp"
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

namespace yangacer {
namespace json {

using boost::spirit::istream_iterator;

template<typename Iterator = std::string::const_iterator>
struct grammar
: boost::spirit::qi::grammar<Iterator, var_t(), boost::spirit::qi::space_type>
{
  typedef boost::spirit::qi::space_type space_type;
  grammar();

  boost::spirit::qi::rule<Iterator, object_t(), space_type> object_r;
  boost::spirit::qi::rule<Iterator, array_t(), space_type> array_r;
  boost::spirit::qi::rule<Iterator, std::pair<object_t::key_type, object_t::mapped_type>(), space_type > pair_r;
  boost::spirit::qi::rule<Iterator, std::string(), space_type> string_r;
  boost::spirit::qi::rule<Iterator, var_t(), space_type> var_r;

  boost::spirit::qi::symbols<char const, char const> unesc_char;

};

template<typename Iter>
bool phrase_parse(Iter &beg, Iter &end, var_t &v)
{
  namespace qi = boost::spirit::qi;
  grammar<Iter> parser;
  return qi::phrase_parse(beg, end, parser, qi::space, v);
}

template<typename Iter>
bool parse(Iter &beg, Iter &end, var_t &v)
{
  namespace qi = boost::spirit::qi;
  grammar<Iter> parser;
  return qi::parse(beg, end, parser, qi::space, v);
}

} }// namespace yangacer::json

#endif // header guard
