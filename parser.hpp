#ifndef JSON_PARSER_HPP_
#define JSON_PARSER_HPP_

#include "variant.hpp"
//#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

namespace yangacer {
namespace json {

// namespace ascii = boost::spirit::ascii;

//using qi::parse;
//using qi::phrase_parse;
//using qi::space;
using boost::spirit::istream_iterator;

template<typename Iterator = std::string::const_iterator>
struct grammar
: boost::spirit::qi::grammar<Iterator, object_t()>
{
  grammar();

  boost::spirit::qi::rule<Iterator, object_t()> object_r;
  boost::spirit::qi::rule<Iterator, array_t()> array_r;
  boost::spirit::qi::rule<Iterator, std::pair<object_t::key_type, object_t::mapped_type>() > pair_r;
  boost::spirit::qi::rule<Iterator, std::string()> string_r;
  boost::spirit::qi::rule<Iterator, var_t()> var_r;

  boost::spirit::qi::symbols<char const, char const> unesc_char;

};

template<typename Iter>
bool phrase_parse(Iter &beg, Iter &end, object_t &o)
{
  namespace qi = boost::spirit::qi;
  static grammar<Iter> parser;
  return qi::phrase_parse(beg, end, parser, qi::space, o);
}

template<typename Iter>
bool parse(Iter &beg, Iter &end, object_t &o)
{
  namespace qi = boost::spirit::qi;
  static grammar<Iter> parser;
  return qi::parse(beg, end, parser, qi::space, o);
}

} }// namespace yangacer::json

#endif // header guard
