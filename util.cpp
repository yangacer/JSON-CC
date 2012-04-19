#include "util.hpp"
#include <boost/spirit/include/support_ostream_iterator.hpp>
#include <boost/variant/apply_visitor.hpp>

namespace yangacer{
namespace json{
  
template <typename OIter>
escaped_string<OIter>::escaped_string()
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

template escaped_string<boost::spirit::ostream_iterator>::escaped_string();

escaped_string<boost::spirit::ostream_iterator> print::str_gen;

// print visitor impl -----------------

void print::operator()(void*) const
{ (*os_)<<"null"; }

void print::operator()(bool const b) const
{
  b?(*os_)<<"true":(*os_)<<"false";
}

void print::operator()(std::string const& s) const
{ 
  escaped_string<boost::spirit::ostream_iterator> str_gen;
  boost::spirit::ostream_iterator sink(*os_);
  karma::generate(sink, str_gen, s);
}

void print::operator()(array_t const &v) const
{
  (*os_)<<"[";
  for(size_t i=0;i<v.size();++i){
    boost::apply_visitor(*this, v[i]);
    if(i + 1 != v.size())
      (*os_)<<",";
  }
  (*os_)<<"]";
}

void print::operator()(object_t const &m) const
{
  using namespace std;
  (*os_)<<"{";
  object_t::const_iterator i = m.begin();
  object_t::const_iterator j = m.begin();
  while(i != m.end()){
    (*this)(i->first);
    (*os_)<<" : ";
    boost::apply_visitor(*this, i->second);
    if(++i != m.end())
      (*os_)<<",\n";
  }
  (*os_)<<"}\n";
}

}}

