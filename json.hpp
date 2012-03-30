#ifndef JSON_PARSER_HPP_
#define JSON_PARSER_HPP_
//#define BOOST_SPIRIT_UNICODE
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/mpl/vector.hpp>

#include <vector>
#include <map>
#include <string>
#include <iostream>

namespace json_parser {

using namespace boost;
using namespace std;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
//namespace unicode = boost::spirit::unicode;
namespace phoenix = boost::phoenix;

typedef mpl::vector<
  int64_t,
  string,
  double,
  bool,
  vector<recursive_variant_>,
  map<string, recursive_variant_ >
  >::type types;

typedef make_recursive_variant_over<types>::type var_t;
typedef vector<var_t> array_t;
typedef map<string, var_t> object_t;

template <typename T>
struct strict_real_policies : qi::real_policies<T>
{
    static bool const expect_dot = true;
};

qi::real_parser< double, strict_real_policies<double> > real_;

template<typename Iterator>
struct json_grammar
: qi::grammar<Iterator, object_t()>
{
  json_grammar() : json_grammar::base_type(object_r)
  {
    using qi::char_;
    using qi::lexeme;
    using qi::skip;
    using qi::space;
    using qi::double_;
    using qi::true_;
    using qi::false_;
    using namespace qi::labels;

    typedef qi::int_parser< boost::int64_t > int64_parser;
    int64_parser int64_;

    unesc_char.add  ("\\\"", '\"')  ("\\\\", '\\')
      ("\\/", '/')  ("\\b", '\b')   ("\\f", '\f') 
      ("\\n", '\n') ("\\r", '\r')   ("\\t", '\t');
    
    object_r %=  skip(space)[
      '{' >> *( pair_r >> *(',' >> pair_r) ) >>  '}'
      ]
      ;

    pair_r %= skip(space)[
      string_r >> -(':' >> var_r)
      ];

    array_r %= skip(space)[
      '[' >>
        var_r >> *( qi::lit(',') >> var_r ) >>
      ']'];

    var_r = 
      ( object_r | array_r | 
        string_r | real_   | int64_ |
        true_    | false_ );

    string_r %= lexeme['"' >> 
      *(unesc_char |  (qi::byte_ - '"') | ("\\x" >> qi::hex) )>> '"'];

  }

  qi::rule<Iterator, object_t()> object_r;
  qi::rule<Iterator, array_t()> array_r;
  qi::rule<Iterator, pair<object_t::key_type, object_t::mapped_type>() > pair_r;
  qi::rule<Iterator, string()> string_r;
  qi::rule<Iterator, var_t()> var_r;

  qi::symbols<char const, char const> unesc_char;

};

struct print
: static_visitor<>
{
  template<typename T>
  void operator()(T const& v) const
  { cout<<v; }

  void operator()(string const& s) const
  { cout<<"\""<<s<<"\""; }

  template<typename T>
  void operator()(vector<T> const &v) const
  {
    cout<<"[";
    for(size_t i=0;i<v.size();++i){
      boost::apply_visitor(*this, v[i]);
      if(i + 1 != v.size())
        cout<<",";
    }
    cout<<"]";
  }

  template<typename T>
  void operator()(map<string, T> const &m) const
  {
    cout<<"{";
    typename map<string,T>::const_iterator i = m.begin();
    typename map<string,T>::const_iterator j = m.begin();
    while(i != m.end()){
      (*this)(i->first);
      cout<<" : ";
      boost::apply_visitor(*this, i->second);
      if(++i != m.end())
        cout<<",\n";
    }
    cout<<"}\n";
  }
};


}// namespace json_parser

#endif // header guard
