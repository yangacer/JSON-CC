#include "accessor.hpp"

namespace yangacer {
namespace json {

member_of::member_of(var_t &v)
  : v_ptr_(&v)
{}

member_of& 
member_of::operator[](std::string const &member)
{
  v_ptr_ = &(boost::get<object_t>(*v_ptr_)[member]);
  return *this;
}

member_of&
member_of::operator[](std::size_t offset)
{
  v_ptr_ = &(boost::get<array_t>(*v_ptr_)[offset]);
  return *this;
}

var_t&          member_of::var()    { return *v_ptr_; }
object_t&       member_of::object() { return value<object_t>(); }
array_t&        member_of::array()  { return value<array_t>(); }
std::string&    member_of::string() { return value<std::string>(); };
unsigned int&   member_of::uint()   { return value<unsigned int>(); };
boost::int64_t& member_of::int64()  { return value<boost::int64_t>(); };


}} // namespace yangacer::json
