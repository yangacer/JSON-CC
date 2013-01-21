#ifndef JSONCC_ACCESSOR_HPP_
#define JSONCC_ACCESSOR_HPP_

#include <string>
#include "variant.hpp"

namespace yangacer {
namespace json {

struct member_of
{
  member_of(var_t &v);
  member_of &operator[](std::string const &member);
  member_of &operator[](std::size_t offset);

  template<typename T>
  T &value() { return boost::get<T>(*v_ptr_); }

  template<typename T>
  void value(T&& val) { *v_ptr_ = val; }

  template<typename T>
  member_of& operator=(T&& val) { *v_ptr_ = val; return *this; }

  var_t &operator*();

private:
  var_t *v_ptr_;
};


}}
// brief alias
#define mbof(X) (yangacer::json::member_of(X))

#endif
