#ifndef JSON_VARIANT_HPP_
#define JSON_VARIANT_HPP_

#include <boost/variant.hpp>
#include <boost/mpl/vector.hpp>
#include <deque>
#include <map>
#include <string>

namespace yangacer{
namespace json{

using boost::get;

typedef boost::mpl::vector<
  boost::blank,
  bool,
  unsigned int,
  int64_t,
  double,
  std::string,
  std::deque<boost::recursive_variant_>,
  std::map<std::string, boost::recursive_variant_ >
  >::type types;

// variable type
typedef boost::make_recursive_variant_over<types>::type var_t;
typedef std::deque<var_t> array_t;
typedef std::map<std::string, var_t> object_t;
typedef boost::blank null_t;
}}

namespace boost { namespace serialization 
{ 
  template<class Archive> 
  void serialize(Archive &, boost::blank &, const unsigned int) 
  { /*Nothing to do here*/   } 
}} // namespace boost::serialization

#endif
