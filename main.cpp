#include "json.hpp"
#include "util.hpp"
#include <string>

int main(int argc, char** argv)
{
  using namespace std;
  using namespace yangacer;

  string input = argv[1];
  json::grammar<> grammar; // default iterator type is string::const_iterator
  json::object_t o;
  
  json::grammar<>::iterator_type beg = input.begin(), end = input.end();

  if(!json::parse(beg, end, grammar, o)){
    cerr<<"Parsing failed at: "<<(beg - input.begin())<<"\n";
  }else{
    cout<<o.size()<<"\n";
    json::var_t v = o;
    apply_visitor(json::print(cout), v);
  }
  return 0;
}
