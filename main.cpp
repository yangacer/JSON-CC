#include "json.hpp"
#include "util.hpp"
#include <string>
#include <iostream>

/**
 * Read JSON from standard input, parse it and print with
 * json::print visitor.
 */
int main(int argc, char** argv)
{
  using namespace std;
  using namespace yangacer;
  
  json::istream_iterator beg(cin), end;

  json::grammar<json::istream_iterator> grammar; 
  json::object_t o;
  
  // or use json::parse(beg, end, grammar, o) to parse line-by-line
  if(!json::phrase_parse(beg, end, grammar, json::space, o)){ 
    cerr<<"Parsing failed\n";
  }else{
    cout<<o.size()<<"\n";
    json::print prt(cout);
    prt(o);
  }
  return 0;
}
