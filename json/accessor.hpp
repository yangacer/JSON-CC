#ifndef JSONCC_ACCESSOR_HPP_
#define JSONCC_ACCESSOR_HPP_

#include <string>
#include <boost/mpl/find.hpp>
#include "boost/noncopyable.hpp"
#include "variant.hpp"

namespace yangacer {
namespace json {

struct member_of 
  : private boost::noncopyable
{
  explicit member_of(var_t &v);
  member_of &operator[](char const* member);
  member_of &operator[](std::size_t offset);

  operator bool();

  template<typename T>
  T &value() { return boost::get<T>(*v_ptr_); }

  template<typename T>
  void value(T&& val) { if(v_ptr_) *v_ptr_ = val; }

  template<typename T>
  member_of& operator=(T&& val) { if(v_ptr_) *v_ptr_ = val; return *this; }

  template<typename T>
  T &test(T &&default_value)
  {
    typedef typename boost::mpl::find<var_t::types,T>::type::pos which;

    if(v_ptr_) {
      if( 0 == v_ptr_->which() ) 
        *v_ptr_ = default_value;
    }
    assert( v_ptr_->which() == which::value );
    return value<T>();  
  }

  var_t&          var();
  object_t&       object();
  array_t&        array();
  std::string&    string();
  unsigned int&   uint();
  boost::int64_t& int64();

private:
  var_t *v_ptr_;
};

template<typename T>
bool operator==(member_of const &lhs, T const &rhs)
{
  return lhs.value<T>() == rhs;
}
template<typename T>
bool operator==(T const &lhs, member_of const &rhs)
{
  return lhs == rhs.value<T>();
}

template<typename T>
bool operator < (member_of const &lhs, T const &rhs)
{
  return lhs.value<T>() < rhs; 
}

template<typename T>
bool operator < (T const &lhs, member_of const &rhs)
{
  return lhs < rhs.value<T>();
}


}}
// brief alias
#define mbof(X) (yangacer::json::member_of(X))

#endif
