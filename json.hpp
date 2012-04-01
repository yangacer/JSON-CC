#ifndef JSON_PARSER_HPP_
#define JSON_PARSER_HPP_
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

namespace yangacer {
namespace json {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

using qi::parse;

typedef boost::mpl::vector<
  int64_t,
  std::string,
  double,
  bool,
  std::vector<boost::recursive_variant_>,
  std::map<std::string, boost::recursive_variant_ >
  >::type types;

// variable type
typedef boost::make_recursive_variant_over<types>::type var_t;

// array type
typedef std::vector<var_t> array_t;

// object type
typedef std::map<std::string, var_t> object_t;


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

struct print
: boost::static_visitor<>
{
  template<typename T>
  void operator()(T const& v) const
  { std::cout<<v; }

  void operator()(std::string const& s) const
  { std::cout<<"\""<<s<<"\""; }

  template<typename T>
  void operator()(std::vector<T> const &v) const
  {
    std::cout<<"[";
    for(size_t i=0;i<v.size();++i){
      boost::apply_visitor(*this, v[i]);
      if(i + 1 != v.size())
        std::cout<<",";
    }
    std::cout<<"]";
  }

  template<typename T>
  void operator()(std::map<std::string, T> const &m) const
  {
    using namespace std;
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

} }// namespace yangacer::json

#endif // header guard
