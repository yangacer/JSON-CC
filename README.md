<link href="markdown.css" rel="stylesheet" type="text/css"><link/>

Following description uses yangacer::json and JSON-CC to refer this library
interchangeably.

#Qickstart

1. Build and install library <p/>

    cd JSON-CC;mkdir build;cd build;cmake ..;make install

2. A hello world example:

```C++
    #include "json.hpp"
    #include "util.hpp"
    #include &lt;string>
    #include &lt;iostream>

    int 
    main(int argc, char** argv)
    {
      using namespace std;
      using namespace yangacer;
      
      string input = "{\"data\":\"Hello world!\"}";
      json::grammar<> grammar; 
      json::object_t o;
      string::iterator 
        beg = input.begin(),
        end = input.end();

      // or use json::parse(beg, end, grammar, o) to parse line-by-line
      if(!json::phrase_parse(beg, end, grammar, json::space, o)){ 
        cerr<<"Parsing failed\n";
      }else{
        cout<<"Parsing success\n";
        json::print prt(cout);
        prt(o);
      }
      return 0;
    }    
```
3. Read util.\* to see how to write a visitor for reading/processing a JSON
   object.

#Features

##A C++ JSON object 

A JSON object is modeled by std::map&lt;std::string, json::var\_t&gt;. 

    using namespace yangacer;
    json::object_t obj;
    obj["str"] = "string"; 
    obj["arr"] = json::array_t(3);
    typedef json::array_t::iterator iter;
    for(iter i = obj["arr"].begin(); i != obj["arr"].end();++i)
      // process elements in array

An instance of var\_t can be one of following types

    int, int64_t, double, string, null
    array_t, object_t

The var\_t is actually a boost::variant class, to get familiar with using it,
please check the [boost::variant
document](http://www.boost.org/doc/libs/1_49_0/doc/html/variant/tutorial.html)

##libjson.so: Pre-built grammar of frequently used iterator types

The types are std::string::const\_iterator, char const\*, and boost::spirit::istream\_iterator.
Note that the boost::spirit::istream\_iterator is an adaptor that allow a parser to
read input from std::istream class. In case of using theose types, compilation time of user code can be
reduced largely via linking the pre-built library.

If you require to use different iterator types, you can simply include both
"json.hpp" and "json\_def.hpp" in your code and define a grammar of the
iterator type. e.g.

    #include "json.hpp"
    #include "json_def.hpp"

    // ...
    using namespace yangacer;
    json::grammar<my_iterator_type> grammar;
    json::phrase_parse(...);

##Processing of Characters 
  
  - Support ASCII and UNICODE (other than that will not work). 

  - Support escaped string.

#JSON-CC v.s. JSON SPIRIT

[JSON SPIRIT](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented) 
is a famous JSON parser that has some similar implementation with
yangacer::json.

1. json\_spirit wraps boost::variant as an internal structure and dose not
   allow access to a terminal value. e.g. numeric and string. Either
   boost::apply\_visitor can not be used.

2. json\_spirit can parse data a little bit faster. Hereby are some
   benchmarks:
  
  <pre>
    input(mb) yangacer(sec) json_spirit
    5.6       0.52          0.44
    11        1.02          0.88
    16        1.54          1.30
    21        2.03          1.77
    37        3.55          3.14
  </pre>

#Tip

##Boost parsing speed with premodel

  If you have knowledge of what fields and their type will be constructed
  before parsing. You can premodel it. e.g.

    // Data to be parsed
    // {"data":[1,2], "name":"acer", "ref":{}}

    using namespace yangacer;
    json::object_t obj;
    obj["data"] = json::array_t();
    obj["name"] = std::string();
    obj["ref"] = json::object_t();

