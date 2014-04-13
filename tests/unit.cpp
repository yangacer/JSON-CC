#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE json
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include "json.hpp"
#include "accessor.hpp"

namespace json = yangacer::json;

BOOST_AUTO_TEST_CASE( assign_and_compare_literal_string )
{

  json::var_t v;
  char const *literal = "literal string testing";
  mbof(v)["text"] = "literal string testing";
  BOOST_CHECK(cmbof(v)["text"] == literal);
  BOOST_CHECK(cmbof(v)["text"] < "literal string testing BIG");
}

BOOST_AUTO_TEST_CASE( assign_and_compare_literal_integer )
{
  json::var_t v;
  short lit_short = 123;
  unsigned short lit_ushort = 123;
  int lit_int = 123;
  unsigned int lit_uint = 123;
  long lit_long = 123;
  long long lit_long_long = 123;

  mbof(v)["integer"] = lit_short;
  BOOST_CHECK(cmbof(v)["integer"] == 123);
  BOOST_CHECK(cmbof(v)["integer"] < 456);
  mbof(v)["integer"] = lit_ushort;
  BOOST_CHECK(cmbof(v)["integer"] == 123);
  BOOST_CHECK(cmbof(v)["integer"] < 456);
  mbof(v)["integer"] = lit_int;
  BOOST_CHECK(cmbof(v)["integer"] == 123);
  BOOST_CHECK(cmbof(v)["integer"] < 456);
  mbof(v)["integer"] = lit_uint;
  BOOST_CHECK(cmbof(v)["integer"] == 123u);
  BOOST_CHECK(cmbof(v)["integer"] < 456u);
  mbof(v)["integer"] = lit_long;
  BOOST_CHECK(cmbof(v)["integer"] == 123l);
  BOOST_CHECK(cmbof(v)["integer"] < 456l);
  mbof(v)["integer"] = lit_long_long;
  BOOST_CHECK(cmbof(v)["integer"] == 123ll);
  BOOST_CHECK(cmbof(v)["integer"] < 456ll);
}

BOOST_AUTO_TEST_CASE( assign_and_compare_std_string )
{
  json::var_t v;
  std::string s = "std::string testing";
  mbof(v)["text"] = s;
  BOOST_CHECK(cmbof(v)["text"] == s);
  s += "suffix";
  BOOST_CHECK(cmbof(v)["text"] < s);
}

BOOST_AUTO_TEST_CASE( assign_and_compare_object )
{
  json::var_t v;
  json::var_t o;
  mbof(o)["f1"] = "test";
  mbof(o)["f2"] = 123;
  mbof(v)["object"] = o;
  BOOST_CHECK(cmbof(v)["object"] == cmbof(o).object());
}

BOOST_AUTO_TEST_CASE( assign_and_compare_array )
{
  json::var_t v;
  json::var_t a = json::array_t(2);
  mbof(a)(0) = "test";
  mbof(a)(1) = 123;
  mbof(v)["array"] = a;
  BOOST_CHECK(cmbof(v)["array"] == cmbof(a).array());
}

