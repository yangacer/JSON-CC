#ifndef JSON_UTIL_HPP_
#define JSON_UTIL_HPP_

#include <boost/variant/apply_visitor.hpp>

namespace yangacer {
namespace json {

struct print
: boost::static_visitor<>
{
  print(std::ostream &os):os_(&os){}

  template<typename T>
  void operator()(T const& v) const
  { (*os_)<<v; }

  void operator()(std::string const& s) const
  { (*os_)<<"\""<<s<<"\""; }

  template<typename T>
  void operator()(std::deque<T> const &v) const
  {
    (*os_)<<"[";
    for(size_t i=0;i<v.size();++i){
      boost::apply_visitor(*this, v[i]);
      if(i + 1 != v.size())
        (*os_)<<",";
    }
    (*os_)<<"]";
  }

  template<typename T>
  void operator()(std::map<std::string, T> const &m) const
  {
    using namespace std;
    (*os_)<<"{";
    typename map<string,T>::const_iterator i = m.begin();
    typename map<string,T>::const_iterator j = m.begin();
    while(i != m.end()){
      (*this)(i->first);
      (*os_)<<" : ";
      boost::apply_visitor(*this, i->second);
      if(++i != m.end())
        (*os_)<<",\n";
    }
    (*os_)<<"}\n";
  }

  std::ostream *os_;
};

}} // namespace yangacer::json

#endif
