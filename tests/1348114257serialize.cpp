#include "variant.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/deque.hpp>
#include <sstream>
#include "util.hpp"
#include <iostream>

int main()
{
  using namespace yangacer;
  using std::cout;
  namespace archive = boost::archive;

  std::stringstream cvt;

  {
    archive::text_oarchive oa(cvt);
    json::object_t obj1, obj2;
    obj1["name"] = std::string("obj1");
    obj1["ver"] = 1;
    obj2["name"] = std::string("obj2");
    obj2["ver"] = 2;
    cout<<"Object1:\n";
    json::prt(obj1);
    cout<<"Object2:\n";
    prt(obj2);
    oa<<obj1<<obj2;
  }

  {
    cvt.clear();
    cvt.seekg(0);
    archive::text_iarchive ia(cvt);
    json::object_t obj1, obj2;
    ia>>obj1>>obj2;
    json::print prt(cout);
    cout<<"After restored ==============\n";
    cout<<"Object1:\n";
    prt(obj1);
    cout<<"Object2:\n";
    prt(obj2);
  }

  return 0;
}
