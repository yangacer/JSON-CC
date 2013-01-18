#include "filter.hpp"
#include <boost/mpl/find.hpp>

#define INDEX_OF_TYPE(T, List) \
  boost::mpl::find<List, T>::type::pos::value

namespace yangacer {
namespace json {

  /*
  enum WHICH_TYPE {
    IS_ARRAY_T = INDEX_OF_TYPE(array_t, types),
    IS_OBJECT_T = INDEX_OF_TYPE(var_t, types)
  };
  */
  /*
  bool field_filter(array_t &a, boost::regex const &regex)
  {
    for(auto val = a.begin(); val != a.end(); ++ val){
      
    }
  }
  */
  // TODO Adopt var_t rather than object_t
  bool field_filter(var_t &v, boost::regex const& regex)
  {
    bool any_matched = false;

    for(auto attr = v.begin(); attr != v.end(); ++attr){

      bool child_matched = false;
      
      switch(attr->second.which()){
      case 5:
        
        break;
      case 6:
        child_matched = field_filter(boost::get<object_t>(attr->second), regex);
        break;
      }
      
      if(child_matched || boost::regex_search(attr->first, regex))
        any_matched = true;
      else
        attr = v.erase(attr);
      if(!v.size()) break;
    }
    return any_matched;
  }

  void inplace_field_filter(var_t &v, boost::regex const& regex)
  {
    field_filter(v, regex);
  }

  void inplace_field_filter(var_t &v, boost::regex const& regex, 
                            int depth_begin, int depth_end)
  {
    
  }

} // namespace json
} // namespace yangacer

