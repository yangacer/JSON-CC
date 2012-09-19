#include "filter.hpp"

namespace yangacer {
namespace json {
  
#define OBJECT_T_WHICH 6

  bool field_filter(object_t &o, boost::regex const& regex)
  {
    bool any_matched = false;

    for(auto attr = o.begin(); attr != o.end(); ++attr){

      bool child_matched = false;

      if(OBJECT_T_WHICH == attr->second.which()){
        child_matched = field_filter(boost::get<object_t>(attr->second), regex);
      }
      if(child_matched || boost::regex_search(attr->first, regex))
        any_matched = true;
      else
        attr = o.erase(attr);
      if(!o.size()) break;
    }
    return any_matched;
  }

  void inplace_field_filter(object_t &o, boost::regex const& regex)
  {
    field_filter(o, regex);
  }

  void inplace_field_filter(object_t &o, boost::regex const& regex, 
                            int depth_begin, int depth_end)
  {
    
  }

} // namespace json
} // namespace yangacer

