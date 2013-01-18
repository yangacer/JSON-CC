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
    json::object_t &o =
      boost::get<json::object_t>(v);
    cout << "Parsing successed\n";
    json::pretty_print(cout, v);
    cout << "== Origin =========\n";
    boost::regex regex("^re");
    json::inplace_field_filter(o, regex);
    cout << "== Filtered =======\n";
    json::pretty_print(cout, v);
  }
  return 0;
}
