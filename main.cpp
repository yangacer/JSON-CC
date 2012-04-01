#include "json.hpp"
#include <string>

int main(int argc, char** argv)
{
  using namespace std;
  using namespace yangacer;

  string input = argv[1];
  json::grammar<> grammar;
  json::object_t o;

  string::const_iterator beg = input.begin(), end = input.end();

  if(!json::parse(beg, end, grammar, o)){
    cerr<<"parsing failed at: "<<(beg - input.begin())<<"\n";
  }else{
    cout<<o.size()<<"\n";
    json::var_t v = o;
    apply_visitor(json::print(), v);
  }
  return 0;
}
