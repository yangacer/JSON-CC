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
    ('\"', "\\\"")('/',"\\/")
    ;
  esc_str =   karma::lit("\"")
    << *(esc_char | karma::byte_)
    <<  karma::lit("\"")
    ;
}

template <typename Iter>
void generate_escaped_string(Iter &sink, std::string const& str)
{
  escaped_string<boost::spirit::ostream_iterator> str_gen;
  karma::generate(sink, str_gen, str);
}

// print visitor impl -----------------
#define PRINT_SPACE(Depth, OStream) \
  for(int i__ =0; i__ < Depth; ++i__){ (OStream) << "  "; }

print::print(std::ostream &os, mode_t mode)
: os_(&os), mode_(mode), depth_(0) 
{}

void print::operator()(boost::blank const &b) const
{ 
  (*os_) << "null" ;
}

void print::operator()(bool const b) const
{
  b?(*os_)<<"true":(*os_)<<"false";
}

void print::operator()(std::string const& s) const
{ 
  if( noescape == mode_ ) {
    (*os_) << s;    
  } else {
    boost::spirit::ostream_iterator sink(*os_);
    generate_escaped_string(sink, s);
  }
}

void print::operator()(array_t const &v) const
{
  if(mode_) {
    (*os_) << "\n";
    PRINT_SPACE(depth_, *os_);
  }
  (*os_) << "[";
  if(mode_) (*os_) << "\n";
  ++depth_;
  for(size_t i=0;i<v.size();++i){
    if(i != 0) { 
      (*os_)<<",";
      if(mode_) (*os_) << "\n";
    }
    if(mode_) PRINT_SPACE(depth_, *os_);
    boost::apply_visitor(*this, v[i]);
  }
  --depth_;
  if(mode_) {
    (*os_) << "\n";
    PRINT_SPACE(depth_, *os_);
  }
  (*os_)<<"]";
}

void print::operator()(object_t const &m) const
{
  using namespace std;
  
  if(mode_) {
    if(depth_)
      (*os_) << "\n";
    PRINT_SPACE(depth_, *os_);
  }

  (*os_) << "{";
  if(mode_) (*os_) << "\n";
  object_t::const_iterator i = m.begin();
  object_t::const_iterator j = m.begin();
  ++depth_;
  while(i != m.end()){
    if(mode_) PRINT_SPACE(depth_, *os_);
    (*this)(i->first);
    (*os_) << " : ";
    boost::apply_visitor(*this, i->second);
    if(++i != m.end()) {
      (*os_) << ",";
      if(mode_) (*os_) << "\n";
    }
  }
  depth_--;
  if(mode_) {
    (*os_) << "\n";
    PRINT_SPACE(depth_, *os_);
  }
  (*os_) << "}";
  if(mode_ && !depth_) 
    (*os_) << "\n";

}

}}

