#include "json.hpp"
#include "util.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
  using namespace std;
  using namespace yangacer;
  
  json::istream_iterator beg(cin), end;

  //string input = argv[1];
  json::grammar<json::istream_iterator> grammar; // default iterator type is string::const_iterator
  json::object_t o;
  
  //json::grammar<>::iterator_type beg = input.begin(), end = input.end();

  if(!json::phrase_parse(beg, end, grammar, json::space, o)){
    cerr<<"Parsing failed\n";
  }else{
    cout<<o.size()<<"\n";
    json::var_t v = o;
    apply_visitor(json::print(cout), v);
  }
  return 0;
}
