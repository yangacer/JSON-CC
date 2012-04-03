#ifndef JSON_DEF_HPP_
#define JSON_DEF_HPP_

#include "json.hpp"
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace yangacer {
namespace json {

namespace phoenix = boost::phoenix;

template <typename T>
struct strict_real_policies 
: qi::real_policies<T>
{
    static bool const expect_dot = true;
};

template<typename Iter>
grammar<Iter>::grammar() 
: grammar::base_type(object_r)
{
  using qi::char_;
  using qi::int_;
  using qi::lexeme;
  using qi::skip;
  using qi::space;
  using qi::double_;
  using qi::true_;
  using qi::false_;
  using namespace qi::labels;

  typedef qi::int_parser< boost::int64_t > int64_parser;
  int64_parser int64_;
  qi::real_parser< double, strict_real_policies<double> > real_;

  unesc_char.add  ("\\\"", '\"')  ("\\\\", '\\')
    ("\\/", '/')  ("\\b", '\b')   ("\\f", '\f') 
    ("\\n", '\n') ("\\r", '\r')   ("\\t", '\t');

  object_r %=  skip(space)[
    '{' >> *( pair_r >> *(',' >> pair_r) ) >>  '}'
    ]
    ;

  pair_r %= skip(space)[
    string_r >> (':' >> var_r)
    ];

  array_r %= skip(space)[
    '[' >>
    var_r >> *( ',' >> var_r ) >>
    ']'];

  var_r = 
    ( 
      true_     | false_  |
      int_      | int64_  | real_ |
      string_r  | 
      object_r  | array_r 
    );

  // Accept UNICODE (no verification of any UNICODE rule)
  string_r %= lexeme['"' >> 
    *(unesc_char |  (qi::byte_ - '"') /*| ("\\u" >> qi::hex)*/ )>> '"'];

}



}} // namespace yangacer::json

#endif 
