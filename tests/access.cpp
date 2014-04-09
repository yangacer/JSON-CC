#include <iostream>
#include <boost/lexical_cast.hpp>
#include "json.hpp"
#include "accessor.hpp"

int main()
{
  using namespace std;
  using namespace yangacer;

  json::var_t mix;
  bool catched=false;

  // Changes the mix variable to an object_t object and
  // insert a member "error" of value 404u
  mbof(mix)["error"] = boost::intmax_t(404); // OK

  // "error" is already an uint
  // Follwing access is non sense. Consequently, we
  // got return value 'false'
  assert( !mbof(mix)["error"]["wrong"] ); 

  // Assign to the undefeined field is also
  // equavalent to nop
  mbof(mix)["error"]["wrong"] = string("acer"); 

  try {
    // Force to get a string from an uint
    // will throw exception
    mbof(mix)["error"].string(); // Throw
  } catch(boost::bad_get &) {
    catched = true;  
  }
  assert(catched);

  // Adding "mix.object" and "mix.object.msg" if they do not exist.
  // Test whether mix.object.msg have any value. If not, assign 
  // "default_msg" to it.
  mbof(mix)["object"]["msg"].test(string("default_msg"));

  // Test to mix.object.msg 
  mbof(mix)["object"]["msg"].test(string("error_msg"));

  // Test integer assignment
  mbof(mix)["int"] = boost::intmax_t(4592);

  cout << (mbof(mix)["int"].cast<boost::int64_t>()) ;
  cout << "\n";

  assert( "default_msg" == 
          mbof(mix)["object"]["msg"].test(string("error_msg")));

  // Test std::string as attribute name
  string out_attrib = "outter",
         in_attrib = "inner";
  mbof(mix)[out_attrib][in_attrib] = string("value");

  assert( "value" == 
          mbof(mix)[out_attrib][in_attrib].string());

  // Let's see what we got.
  json::pretty_print(cout, mix);

  json::var_t const & cmix(mix);

  // Const version member_of
  assert (!cmbof(cmix)["error"]["wrong"] );
  assert( cmbof(cmix)["object"]["msg"] == string("default_msg") );
  if( cmbof(cmix)["invalid"] || cmbof(cmix)(32124) ) {
    assert(false && "Never reach here.");
  }

  return 0;
}
