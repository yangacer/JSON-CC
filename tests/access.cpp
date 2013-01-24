#include <iostream>
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
  mbof(mix)["error"] = 404u; // OK
  
  // "error" is already an uint
  // Follwing access is non sense. Consequently, we
  // got return value 'false'
  assert( false == mbof(mix)["error"]["wrong"] ); 
  
  // Assign to the undefeined field is also
  // equavalent to nop
  mbof(mix)["error"]["wrong"] = string("acer"); 

  try {
    // Force to get a string from an uint
    // will throw exception
    mbof(mix)["error"].string(); // Throw
  } catch(boost::bad_get &e) {
    catched = true;  
  }
  assert(catched);

  // Adding "mix.object" and "mix.object.msg" if they do not exist.
  // Test whether mix.object.msg have any value. If not, assign 
  // "default_msg" to it.
  mbof(mix)["object"]["msg"].test(string("default_msg"));

  // Test to mix.object.msg 
  mbof(mix)["object"]["msg"].test(string("error_msg"));

  assert( "default_msg" == 
          mbof(mix)["object"]["msg"].test(string("error_msg")));

  // Let's see what we got.
  json::pretty_print(cout, mix);

  return 0;
}
