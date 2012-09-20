#include "filter.hpp"
#include "variant.hpp"
#include "json.hpp"
#include <string>
#include <iostream>

int main()
{
  using namespace yangacer;
  using namespace std;

  std::string input(
    "{"
      "\"required\":123,"
      "\"discard1\":456,"
      "\"discard2\":"
        "{"
          "\"required\":789"
        "}",
      "\"array\":"
        "["
          "{"
            "\"required\":null"
          "}"
        "]"
    "}"
  );
  json::var_t v;
  auto beg(input.begin()), end(input.end());

  if(!json::phrase_parse(beg, end, v)){
    cerr << "Parsing failed\n";
  }else{
    cout << "Parsing successed\n";
    json::print prt(cout);
    cout << "== Origin =========\n";
    prt(v);
    boost::regex regex("^re");
    json::inplace_field_filter(v, regex);
    cout << "== Filtered =======\n";
    prt(v);
  }
  return 0;
}
