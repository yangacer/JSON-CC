#include "filter.hpp"
#include "variant.hpp"
#include "json.hpp"
#include <string>
#include <iostream>

int main()
{
  using namespace yangacer;
  using namespace std;

  std::string input("{\"required\":123,\"dicard\":456}");
  json::object_t o;
  auto beg(input.begin()), end(input.end());

  if(!json::phrase_parse(beg, end, o)){
    cerr << "Parsing failed\n";
  }else{
    cout << "Parsing successed\n";
    json::inplace_field_filter(o, "required");
    json::print prt(cout);
    prt(o);
  }
  return 0;
}
