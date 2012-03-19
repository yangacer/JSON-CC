#include "json.hpp"

int main(int argc, char** argv)
{
  using namespace json_parser;
  string input = argv[1];
  json_grammar<string::iterator> grammar;
  object_t o;

  string::iterator beg = input.begin(), end = input.end();

  if(!qi::parse(beg, end, grammar, o)){
    cerr<<"parsing failed at: "<<(beg - input.begin())<<"\n";
  }else{
    cout<<o.size()<<"\n";
    var_t v = o;
    apply_visitor(print(), v);
  }
  return 0;
}
