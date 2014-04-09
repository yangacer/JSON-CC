#ifndef JSONCC_ACCESSOR_HPP_
#define JSONCC_ACCESSOR_HPP_

#include <string>
#include <type_traits>
#include <boost/mpl/find.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant/apply_visitor.hpp>
#include "variant.hpp"
#include "cast.hpp"

// TODO Should I extend boost::variant to make itfit JSON ?
// This should largly improve access methods and eliminate
// usage of proxy classes, i.e. member_of

namespace yangacer {
namespace json {

// --
template<typename T, typename E=void>
struct safe_convert
{
  typedef T type;
};

template<typename T>
struct safe_convert<
  T,
  typename std::enable_if<std::is_integral<T>::value>::type
  >
{
  typedef std::intmax_t type;
};

template<>
struct safe_convert<std::uintmax_t>
{
  typedef void type;
};

// --
struct member_of
  : private boost::noncopyable
{
  explicit member_of(var_t &v);
  member_of &operator[](char const* member);
  member_of &operator[](std::string const &member);
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
  member_of& operator=(T&& val)
  {
    typedef typename safe_convert<
      typename std::remove_reference<T>::type
      >::type type;
    if(v_ptr_) {
      *v_ptr_ = (type)val; 
    }
    return *this;
  }

  member_of& operator=(char const* lit);

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
  boost::intmax_t& intmax();
  bool is_null() const;
private:
  var_t *v_ptr_;
};

// -- binary operators for member_of
template<typename T>
bool operator==(member_of const &lhs, T const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return lhs && lhs.value<type>() == rhs;
}
template<typename T>
bool operator==(T const &lhs, member_of const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return rhs && lhs == rhs.value<type>();
}

bool operator==(char const *lhs, member_of const &rhs);
bool operator==(member_of const &lhs, char const *rhs);

template<typename T>
bool operator < (member_of const &lhs, T const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return lhs ? lhs.value<type>() < rhs : true; 
}

template<typename T>
bool operator < (T const &lhs, member_of const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return rhs ? lhs < rhs.value<type>() : false;
}

// --
struct const_member_of 
  : private boost::noncopyable
{
  explicit const_member_of(var_t const &v);
  const_member_of &operator[](char const* member);
  const_member_of &operator[](std::string const &member);
  const_member_of &operator()(std::size_t offset);

  operator bool() const;

  template<typename T>
  T const &value() const { return boost::get<T>(*v_ptr_); }

  template<typename T>
  T cast() const { return boost::apply_visitor(json::cast_<T>(), *v_ptr_); }

  var_t const&          var() const;
  object_t const&       object() const;
  array_t const&        array() const;
  std::string const&    string() const;
  boost::intmax_t const& intmax() const;
  bool is_null() const;
private:
  var_t const *v_ptr_;
};


template<typename T>
bool operator==(const_member_of const &lhs, T const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return lhs && lhs.value<type>() == rhs;
}
template<typename T>
bool operator==(T const &lhs, const_member_of const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return rhs && lhs == rhs.value<type>();
}

bool operator==(char const *lhs, const_member_of const &rhs);
bool operator==(const_member_of const &lhs, char const *rhs);

template<typename T>
bool operator < (const_member_of const &lhs, T const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return lhs ? lhs.value<type>() < rhs : true; 
}

template<typename T>
bool operator < (T const &lhs, const_member_of const &rhs)
{
  typedef typename safe_convert<T>::type type;
  return rhs ? lhs < rhs.value<type>() : false;
}

}} // namespace yangacer::json
// brief alias
#define mbof(X) (yangacer::json::member_of(X))
#define cmbof(X) (yangacer::json::const_member_of(X))

#endif
