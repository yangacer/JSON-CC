#ifndef JSONCC_CAST_HPP_
#define JSONCC_CAST_HPP_

#include <boost/variant/static_visitor.hpp>
#include <boost/lexical_cast.hpp>
#include "variant.hpp"

namespace yangacer {
namespace json {

template<typename T>
struct cast_ : boost::static_visitor<T>
{
  template<typename U>
  T operator()(U const &val) const
  {
    return boost::lexical_cast<T>(val);    
  }

  T operator()(object_t const &) const
  { 
    throw boost::bad_lexical_cast();
  }

  T operator()(array_t const &) const
  {
    throw boost::bad_lexical_cast();
  }
};

} // json
} // yangacer

#endif
