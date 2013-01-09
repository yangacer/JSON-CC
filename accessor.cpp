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

}} // namespace yangacer::json
