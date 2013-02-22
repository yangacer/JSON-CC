#ifndef JSONCC_ACCESSOR_HPP_
#define JSONCC_ACCESSOR_HPP_

#include <string>
#include <boost/mpl/find.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant/apply_visitor.hpp>
#include "variant.hpp"
#include "cast.hpp"

namespace yangacer {
namespace json {

struct member_of 
  : private boost::noncopyable
{
  explicit member_of(var_t &v);
  member_of &operator[](char const* member);
  member_of &operator()(std::size_t offset);

  operator bool() const;

  template<typename T>
  T &value() { return boost::get<T>(*v_ptr_); }

  template<typename T>
  T cast() 
  { return boost::apply_visitor(json::cast_<T>(), *v_ptr_); }
    
  template<typename T>
  T const &value() const { return boost::get<T>(*v_ptr_); }

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
  bool is_null() const;
private:
  var_t *v_ptr_;
};

template<typename T>
bool operator==(member_of const &lhs, T const &rhs)
{
  return lhs && lhs.value<T>() == rhs;
}
template<typename T>
bool operator==(T const &lhs, member_of const &rhs)
{
  return rhs && lhs == rhs.value<T>();
}

template<typename T>
bool operator < (member_of const &lhs, T const &rhs)
{
  return lhs ? lhs.value<T>() < rhs : true; 
}

template<typename T>
bool operator < (T const &lhs, member_of const &rhs)
{
  return rhs ? lhs < rhs.value<T>() : false;
}

struct const_member_of 
  : private boost::noncopyable
{
  explicit const_member_of(var_t const &v);
  const_member_of &operator[](char const* member);
  const_member_of &operator[](std::size_t offset);

  operator bool() const;

  template<typename T>
  T const &value() const { return boost::get<T>(*v_ptr_); }

  template<typename T>
  T cast() const { return boost::apply_visitor(json::cast_<T>(), *v_ptr_); }

  var_t const&          var();
  object_t const&       object();
  array_t const&        array();
  std::string const&    string();
  unsigned int const&   uint();
  boost::int64_t const& int64();
  bool is_null() const;
private:
  var_t const *v_ptr_;
};

template<typename T>
bool operator==(const_member_of const &lhs, T const &rhs)
{
  return lhs && lhs.value<T>() == rhs;
}
template<typename T>
bool operator==(T const &lhs, const_member_of const &rhs)
{
  return rhs && lhs == rhs.value<T>();
}

template<typename T>
bool operator < (const_member_of const &lhs, T const &rhs)
{
  return lhs ? lhs.value<T>() < rhs : true; 
}

template<typename T>
bool operator < (T const &lhs, const_member_of const &rhs)
{
  return rhs ? lhs < rhs.value<T>() : false;
}

}} // namespace yangacer::json
// brief alias
#define mbof(X) (yangacer::json::member_of(X))
#define cmbof(X) (yangacer::json::const_member_of(X))

#endif
