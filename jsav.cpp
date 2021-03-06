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

typedef std::vector<attribute> attribute_list;

template<typename Iterator = std::string::const_iterator>
struct jsav_grammar
: boost::spirit::qi::grammar<
  Iterator, attribute_list(), space_type>
{
  typedef uint_parser<std::size_t> pos_parser_t;
  typedef boost::spirit::qi::grammar<
      Iterator, attribute_list(), space_type
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
  rule<Iterator, yangacer::json::attribute(), space_type> attribute_r;
  rule<Iterator, attribute_list(), space_type> list_r;
  symbols<char const, char const> unesc_char;
};

template<typename Iter>
bool parse_attribute(Iter &beg, Iter &end, attribute_list &a)
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
      "Usage: jsav [-n|-e] <access_method> [input_file]\n\n"
      "   -n Do not escape string.\n"
      "   -e Ecsape all arguments as an array of strings.\n"
      "   Example:\n"
      "     $ echo '{\"test\" : [\"acer\", 123, 456] }' | jsav '.\"test\".2'\n"
      "     456\n"
      ;

    exit(1);
  }
  argc--; argv++;

  json::print::mode_t mode = json::print::pretty;
  if(0 == strcmp("-n", argv[0])) {
    mode = json::print::noescape;
    argc--; argv++;
  } else if(0 == strcmp("-e", argv[0])) {
    json::array_t array;
    for(int i =1; i < argc; ++i) {
      array.push_back(string(argv[i]));
    }
    json::pretty_print(cout, array);
    cout << "\n";
    return 0;
  }
  // parse access_method
  vector<json::attribute> access_method;
  {
    char const *beg(argv[0]), *end(beg + strlen(argv[0]));
    if(!json::parse_attribute(beg, end, access_method)) {
      exit(2);
    }
  }
  if(argc > 1) {
    ifstream fin(argv[1], ios::in | ios::binary);
    cin.rdbuf(fin.rdbuf());
  }
  // read input
  string buf(1024, 0);
  std::streamsize total=0;
  while(!cin.eof()) {
    cin.read(&buf[(unsigned int)total], 1024);
    total += cin.gcount();
    buf.resize(buf.size() << 1);
  }
  buf.resize((unsigned int)total);
  // parse input
  json::var_t var;
  {
    auto beg(buf.begin()), end(buf.end());
    if(!json::phrase_parse(beg, end, var)) {
      exit(3);
    }
  }
  json::const_member_of cm(var);
  json::evaluate_access_method eam(cm);
  for(auto i = access_method.begin(); i != access_method.end();++i)
    eam(*i);
  
  if(!cm) exit(4);

  json::pretty_print(cout, cm.var(), mode);
  
  cout << "\n";
  return 0;
}

