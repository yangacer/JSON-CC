#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <boost/variant/static_visitor.hpp>
#include <boost/spirit/include/qi.hpp>
#include "json/json.hpp"

namespace yangacer {
namespace json {

using namespace boost::spirit::qi;

typedef boost::variant<
  std::string, 
  std::size_t
  > attribute;

template<typename Iterator = std::string::const_iterator>
struct jsav_grammar
: boost::spirit::qi::grammar<
  Iterator, std::vector<attribute>(), space_type>
{
  typedef uint_parser<std::size_t> pos_parser_t;
  typedef boost::spirit::qi::grammar<
      Iterator, std::vector<attribute>(), space_type
    > super_t;

  jsav_grammar()
    : super_t::base_type(list_r)
  {
    pos_parser_t position;

    unesc_char.add  
      ("\\\"", '\"')  ("\\\\", '\\') 
      ("\\/", '/')  ("\\b", '\b')   ("\\f", '\f') 
      ("\\n", '\n') ("\\r", '\r')   ("\\t", '\t');

    string_r %= lexeme['"' >>
      *( (&lit('\\') >> unesc_char) |  (byte_ - '"')  ) >> '"'];

    attribute_r %=
       position 
       | string_r 
      ;
  
    list_r %= *( '.' >> attribute_r);
  }
  
  rule<Iterator, std::string(), space_type> string_r;
  rule<Iterator, attribute(), space_type> attribute_r;
  rule<Iterator, std::vector<attribute>(), space_type> list_r;
  symbols<char const, char const> unesc_char;
};

template<typename Iter>
bool parse_attribute(Iter &beg, Iter &end, std::vector<attribute> &a)
{
  jsav_grammar<Iter> parser;
  return phrase_parse(beg, end, parser, space, a);
}

struct evaluate_access_method
: boost::static_visitor<>
{
  evaluate_access_method(json::const_member_of &cm)
    : cm_(cm)
  {}

  void operator()(std::string const &str) const
  {
    if(cm_)
      cm_[str.c_str()];
  }
  
  void operator()(std::size_t const &size) const
  {
    if(cm_) 
      cm_(size);
  }

  void operator()(attribute const &attrib) const
  {
    boost::apply_visitor(*this, attrib);
  }
  
  json::const_member_of &cm_;
};

} // namespace json
} // namespace yangacer

int main(int argc, char **argv)
{
  using namespace std;
  using namespace yangacer;

  if(argc < 2) {
    cerr << 
      "Usage: jsav <access_method> [input_file]\n\n"
      "   echo '{\"test\" : [123,456] }' | jsav '.\"test\".2'\n"
      "   456\n"
      ;

    exit(1);
  }
  
  if(argc > 2) {
    ifstream fin(argv[2], ios::in | ios::binary);
    cin.rdbuf(fin.rdbuf());
  }
  // parse access_method
  vector<json::attribute> access_method;
  {
    char const *beg(argv[1]), *end(beg + strlen(argv[1]));
    if(!json::parse_attribute(beg, end, access_method)) {
      exit(1);
    }
  }
  // read input
  string buf(1024, 0);
  size_t total=0;
  while(!cin.eof()) {
    cin.read(&buf[total], 1024);
    total += cin.gcount();
    buf.resize(buf.size() << 1);
  }
  buf.resize(total);
  // parse input
  json::var_t var;
  {
    auto beg(buf.begin()), end(buf.end());
    if(!json::phrase_parse(beg, end, var)) {
      exit(1);
    }
  }
  json::const_member_of cm(var);
  json::evaluate_access_method eam(cm);
  for(auto i = access_method.begin(); i != access_method.end();++i)
    eam(*i);
  
  if(!cm) exit(1);

  json::pretty_print(cout, cm.var());
  
  cout << "\n";
  return 0;
}

