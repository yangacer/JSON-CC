#include "accessor.hpp"

#ifdef JSON_ENABLE_ACCESS_TRACKING
#include <iostream>
#define JSON_ACCESS_TRACKING(X) std::cerr << this << " ." << X << "\n";
#else
#define JSON_ACCESS_TRACKING(X) {}
#endif

namespace yangacer {
namespace json {

enum { 
  ARRAY_WHICH   = boost::mpl::find<var_t::types,array_t>::type::pos::value, 
  OBJECT_WHICH  = boost::mpl::find<var_t::types,object_t>::type::pos::value
};

member_of::member_of(var_t &v)
  : v_ptr_(&v)
{}

member_of&  member_of::operator =(char const *lit)
{
  if(v_ptr_)
    *v_ptr_ = std::string(lit);
  return *this;
}

member_of::operator bool() const
{ return v_ptr_ != 0; }

member_of& 
member_of::operator[](char const* member)
{
  JSON_ACCESS_TRACKING(member);
  if(v_ptr_) {
    if( 0 == v_ptr_->which() ) 
      *v_ptr_ = object_t();

    if(OBJECT_WHICH == v_ptr_->which()) {
      v_ptr_ = &(boost::get<object_t>(*v_ptr_)[member]);
    } else {
      v_ptr_ = 0;
    }
  }
  return *this;
}

member_of& 
member_of::operator[](std::string const &member)
{
  return this->operator[](member.c_str());
}

member_of&
member_of::operator()(std::size_t offset)
{
  JSON_ACCESS_TRACKING(offset);
  if(v_ptr_) {
    if( 0 == v_ptr_->which() ) 
      *v_ptr_ = array_t();

    if(ARRAY_WHICH == v_ptr_->which()) {
      array_t &a = boost::get<array_t>(*v_ptr_);
      if(a.size() <= offset)
         a.resize(offset+1);
      v_ptr_ = &(a[offset]);
    } else {
      v_ptr_ = 0;
    }
  }
  return *this;
}

bool operator==(char const *lhs, member_of const &rhs)
{
  return lhs == const_cast<member_of&>(rhs).string();
}

bool operator==(member_of const &lhs, char const *rhs)
{
  return const_cast<member_of&>(lhs).string() == rhs;
}

bool operator<(char const *lhs, member_of const &rhs)
{
  return lhs < const_cast<member_of&>(rhs).string();
}

bool operator<(member_of const &lhs, char const *rhs)
{
  return const_cast<member_of&>(lhs).string() < rhs;
}

var_t&          member_of::var()    { return *v_ptr_; }
object_t&       member_of::object() { return value<object_t>(); }
array_t&        member_of::array()  { return value<array_t>(); }
std::string&    member_of::string() { return value<std::string>(); }
boost::intmax_t& member_of::intmax()  { return value<boost::intmax_t>(); }
bool member_of::is_null() const { return 0 == v_ptr_ || v_ptr_->which() == 0; }

// ---- const_member_of impl ----

const_member_of::const_member_of(var_t const &v)
  : v_ptr_(&v)
{}

const_member_of& const_member_of::operator[](char const* member)
{
  JSON_ACCESS_TRACKING(member);
  if(v_ptr_) {
    if(OBJECT_WHICH == v_ptr_->which()) {
      object_t const &o = boost::get<object_t>(*v_ptr_);
      auto i = o.find(member);
      v_ptr_ = (i != o.end()) ? &(i->second) : 0;
    } else {
      v_ptr_ = 0;
    }
  }
  return *this;
}

const_member_of& const_member_of::operator[](std::string const &member)
{
  return this->operator[](member.c_str());  
}

const_member_of& const_member_of::operator()(std::size_t offset)
{
  JSON_ACCESS_TRACKING(offset);
  if(v_ptr_) {
    if(ARRAY_WHICH == v_ptr_->which()) {
      array_t const &a = boost::get<array_t>(*v_ptr_);
      v_ptr_ = (offset < a.size()) ? &(a[offset]) : 0;
    } else {
      v_ptr_ = 0;
    }
  }
  return *this;
}

const_member_of::operator bool() const
{
  return v_ptr_ != 0;
}

bool operator==(char const *lhs, const_member_of const &rhs)
{
  return lhs == rhs.string();
}

bool operator==(const_member_of const &lhs, char const *rhs)
{
  return lhs.string() == rhs;
}

bool operator<(char const *lhs, const_member_of const &rhs)
{
  return lhs < rhs.string();
}

bool operator<(const_member_of const &lhs, char const *rhs)
{
  return lhs.string() < rhs;
}

var_t const&          const_member_of::var()     const { return *v_ptr_; }
object_t const&       const_member_of::object()  const { return value<object_t>(); }
array_t const&        const_member_of::array()   const { return value<array_t>(); }
std::string const&    const_member_of::string()  const { return value<std::string>(); }
boost::intmax_t const& const_member_of::intmax() const { return value<boost::intmax_t>(); }
bool const_member_of::is_null() const { return 0 == v_ptr_ || v_ptr_->which() == 0; }

}} // namespace yangacer::json
