#ifndef JSON_UTIL_HPP_
#define JSON_UTIL_HPP_

#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/support_ostream_iterator.hpp>

namespace yangacer {
namespace json {
namespace karma = boost::spirit::karma;

template <typename OutputIterator>
struct escaped_string
  : karma::grammar<OutputIterator, std::string()>
{
    escaped_string()
      : escaped_string::base_type(esc_str)
    {
        esc_char.add('\b', "\\b")('\f', "\\f")('\n', "\\n")
                    ('\r', "\\r")('\t', "\\t")('\\', "\\\\")
                    ('\'', "\\\'")('\"', "\\\"")('/',"\\/")
            ;
        esc_str =   karma::lit("\"")
                << *(esc_char | karma::byte_)
                <<  karma::lit("\"")
            ;
    }
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
  
  void operator()(bool const b) const
  {
    b?(*os_)<<"true":(*os_)<<"false";
  }

  void operator()(std::string const& s) const
  { 
    escaped_string<boost::spirit::ostream_iterator> str_gen;
    boost::spirit::ostream_iterator sink(*os_);
    karma::generate(sink, str_gen, s);
    //(*os_)<<"\""<<s<<"\""; 
  }

  template<typename T>
  void operator()(std::deque<T> const &v) const
  {
    (*os_)<<"[";
    for(size_t i=0;i<v.size();++i){
      boost::apply_visitor(*this, v[i]);
      if(i + 1 != v.size())
        (*os_)<<",";
    }
    (*os_)<<"]";
  }

  template<typename T>
  void operator()(std::map<std::string, T> const &m) const
  {
    using namespace std;
    (*os_)<<"{";
    typename map<string,T>::const_iterator i = m.begin();
    typename map<string,T>::const_iterator j = m.begin();
    while(i != m.end()){
      (*this)(i->first);
      (*os_)<<" : ";
      boost::apply_visitor(*this, i->second);
      if(++i != m.end())
        (*os_)<<",\n";
    }
    (*os_)<<"}\n";
  }

  std::ostream *os_;
};

}} // namespace yangacer::json

#endif
