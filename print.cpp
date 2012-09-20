#include "print.hpp"
#include <boost/spirit/include/support_ostream_iterator.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

namespace karma = boost::spirit::karma;

namespace yangacer{
namespace json{
  
template <typename OutputIterator>
struct escaped_string
: karma::grammar<OutputIterator, std::string()>
{
    escaped_string();
    karma::rule<OutputIterator, std::string()> esc_str;
    karma::symbols<char, char const*> esc_char;
};

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

template <typename Iter>
void generate_escaped_string(Iter &sink, std::string const& str)
{
  static escaped_string<boost::spirit::ostream_iterator> str_gen;
  karma::generate(sink, str_gen, str);
}

// print visitor impl -----------------
#define PRINT_SPACE(Depth, OStream) \
  for(int i__ =0; i__ < Depth; ++i__){ (OStream) << "  "; }

void print::operator()(boost::int64_t const& i) const
{ 
  if(i == std::numeric_limits<boost::int64_t>::max())
    (*os_) << "null";
  else
    (*os_) << i;
}

void print::operator()(bool const b) const
{
  b?(*os_)<<"true":(*os_)<<"false";
}

void print::operator()(std::string const& s) const
{ 
  boost::spirit::ostream_iterator sink(*os_);
  generate_escaped_string(sink, s);
}

void print::operator()(array_t const &v) const
{
  (*os_) << "\n";
  PRINT_SPACE(depth_, *os_);
  (*os_) << "[\n";
  ++depth_;
  for(size_t i=0;i<v.size();++i){
    PRINT_SPACE(depth_, *os_);
    boost::apply_visitor(*this, v[i]);
    if(i + 1 != v.size())
      (*os_)<<",\n";
  }
  --depth_;
  (*os_) << "\n";
  PRINT_SPACE(depth_, *os_);
  (*os_)<<"]";
}

void print::operator()(object_t const &m) const
{
  using namespace std;
  
  if(depth_)
    (*os_) << "\n";
  PRINT_SPACE(depth_, *os_);

  (*os_) << "{\n";
  object_t::const_iterator i = m.begin();
  object_t::const_iterator j = m.begin();
  ++depth_;
  while(i != m.end()){
    PRINT_SPACE(depth_, *os_);
    (*this)(i->first);
    (*os_) << " : ";
    boost::apply_visitor(*this, i->second);
    if(++i != m.end())
      (*os_) << ",\n";
  }
  depth_--;
  (*os_) << "\n";
  PRINT_SPACE(depth_, *os_);
  (*os_) << "}";
  if(!depth_) 
    (*os_) << "\n";

}

}}

