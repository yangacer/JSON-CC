#ifndef JSON_DEF_HPP_
#define JSON_DEF_HPP_

#include "parser.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <iostream>
#include <limits>

namespace yangacer {
namespace json {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

template <typename T>
struct strict_real_policies 
: qi::real_policies<T>
{
    static bool const expect_dot = true;
};


template<typename Iter>
grammar<Iter>::grammar() 
: grammar::base_type(var_r, "object")
{
  using qi::char_;
  using qi::lit;
  using qi::uint_;
  using qi::lexeme;
  using qi::skip;
  using qi::space;
  using qi::double_;
  using qi::bool_;
  using phoenix::construct;
  using phoenix::val;
  using namespace qi::labels;

  qi::real_parser< double, strict_real_policies<double> > real_;
  typedef qi::int_parser< boost::int64_t > int64_parser;
  int64_parser int64_;

  unesc_char.add  ("\\\"", '\"')  ("\\\\", '\\')
    ("\\/", '/')  ("\\b", '\b')   ("\\f", '\f') 
    ("\\n", '\n') ("\\r", '\r')   ("\\t", '\t');

  object_r %=  skip(space)[
    '{' > *( pair_r >> *(',' >> pair_r) ) >  '}'
    ]
    ;

  pair_r %= skip(space)[
    string_r > (':' > var_r)
    ];

  array_r %= skip(space)[
    '[' >
    var_r > *( ',' > var_r ) >>
    ']'];

  var_r %= 
    lit("null") 
      [ _val = 
          std::numeric_limits<boost::int64_t>::max()
      ]
    | bool_  
    | real_ | uint_ | int64_ 
    | (&lit('"') > string_r)  
    | (&lit('{') > object_r)  
    | (&lit('[') > array_r )
    ;

  // Accept UNICODE (no verification of any UNICODE rule)
  string_r %= lexeme['"' >
    *( (&lit('\\') >> unesc_char) |  (qi::byte_ - '"')  ) > '"'];

#ifdef JSON_PARSER_DEBUG_
  object_r.name("object");
  pair_r.name("pair");
  array_r.name("array");
  var_r.name("var");
  string_r.name("string");

  qi::on_error<qi::fail>
  ( object_r ,
    std::cout<<
      val("Error! Expecting ")<<
      _4<<
      val(" here: ")<<
      construct<std::string>(_3,_2)<<
      std::endl
  );
  
  debug(object_r);
  debug(pair);
  debug(array);
  debug(var);
  debug(string);
#endif
}

}} // namespace yangacer::json

#endif 
