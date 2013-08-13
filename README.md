<link href="markdown.css" rel="stylesheet" type="text/css"><link/>

Following description uses yangacer::json and JSON-CC to refer this library
interchangeably.

# Qickstart

1. Build and install library <p/>

    cd JSON-CC;mkdir build;cd build;cmake ..;make install

2. A hello world example:

```C++
  #include "json/json.hpp"
  #include <string>
  #include <iostream>

  int 
  main(int argc, char** argv)
  {
    using namespace std;
    using namespace yangacer;
    
    string input = "{\"data\":\"Hello world!\"}";
    json::var_t v;
    string::iterator 
      beg = input.begin(),
      end = input.end();

    // or use json::parse(beg, end, v) to parse line-by-line
    if(!json::phrase_parse(beg, end, v)){ 
      cerr<<"Parsing failed\n";
    }else{
      cout<<"Parsing successed\n";
      json::print prt(cout);
      prt(v);
    }
    return 0;
  }    
```
3. Read util.\* to see how to write a visitor for reading/processing a JSON
   object.

# Tool

  JSON-CC contains a command line tool `jsav' that can parse and retrieve 
  attribute values of a JSON object. e.g.

```
  $echo '{"test" : [ 0, 2, { "inner": 123 } ]}' | jsav '."test".2."inner"'
  123
```

# Features

  - A C++ JSON Object

  - libjson.so

  - Convinent Access Method

  - Unicode Compactiable

## A C++ JSON object

A JSON object is modeled by std::map&lt;std::string, json::var\_t&gt;. 

```C++
  using namespace yangacer;
  json::object_t obj;
  obj["str"] = "string"; 
  obj["arr"] = json::array_t(3);
  typedef json::array_t::iterator iter;
  json::array_t &arr = boost::get<json::array_t>(obj["arr"]);
  for(iter i = arr.begin(); i != arr.end();++i)
    // process elements in array
```

An instance of var\_t can be one of following types

    unsigned int, int64_t, double, string, null
    array_t, object_t

The var\_t is actually of type boost::variant, to get familiar with it, please 
check the [boost::variant document](http://www.boost.org/doc/libs/1_49_0/doc/html/variant/tutorial.html).

## libjson.so: Pre-built grammar of frequently used iterator types

Supplied types are std::string::const\_iterator, char const\*, and boost::spirit::istream\_iterator.
Note that the boost::spirit::istream\_iterator is an adaptor that allow a parser to
read input from std::istream class. In case of using theose types, compile time of user code can be
reduced largely via linking the pre-built library.

If you require to use different iterator types, you can simply include "json/parser\_def.hpp" 
in your code and define a grammar of the iterator type. e.g.

```C++
  #include "json/parser.hpp"
  #include "json/parser_def.hpp"
  // ...
  using namespace yangacer;

  my_string input(...);
  json::grammar<my_iterator_type> grammar;
  json::var_t variable;
  my_iterator_type begin(input.begin()), end(input.end());

  json::phrase_parse(begin, end, variable);
```

## Convinent Access Method

You can get a reference to any value of a deep nested object via mbof() and
cmbof(). Let's see an example first.

```C++
  #include <iostream>
  #include "json/parser.hpp"
  #include "json/accessor.hpp"
  // ...
  using namespace yangacer;
  
  std::string input = 
    "{" 
    " \"object_1\" :"
    " {"
    "   \"object_2\" : "
    "   {"
    "     \"array\" : [123, \"Hi there!\"] "
    "   }"
    " }"
    "}"
    ;
  auto beg(input.begin()), end(input.end());
  json::var_t variable;
  json::phrase_parse(beg, end, variable);
  // Note we use operator() as subscript method of arrays
  try {
    std::cout << 
      (json::member_of(variable))["object_1"]["object_2"]["array"](1).string() << 
      "\n";
  } catch ( boost::bad_get &e) {
    std::cerr << e.what() << "\n";
  }
  // result:
  // Hi there!
```

The accessing method use boost::get internally, hence throw boost::bad_get
exception when types do not match. To ease tracking bugs, one can define 
```JSON_ENABLE_ACCESS_TRACKING``` in compile time. Once the macro is defined,
all access to json variant will be output to std::cerr. e.g.

```
Variable addr  Attribute name
-----------------------------
0x7fffd5e72bd0 .segment_map
0x7fffd5e72be0 .count
0x7fffd5e72bf0 .size
0x7fffd5e72bd0 .segment_map
0x7fffd5e72be0 .count
0x7fffd5e72be0 .count
0x7fffd5e72bf0 .size
0x7fffd5e72bd0 .segment_map
0x7fffd5e72be0 .count
0x7fffd5e72be0 .count
0x7fffd5e72bf0 .size
```

Furthermore:

```C++
  // Do some js like test or set
  var_t v;
  var_t const &cv;
  
  // Set 
  // XXX This creates elements only if v.is_null() is true.
  mbof(v)(0)["say"] = string("yo");

  // Test
  if( cmbof(cv)["not_exist"] ) {
    assert( false && "Never reach." );
  } 
  
```

The mbof and its constant version, cmbof, are two macro that abbreviated from json::member_of and
json::const_member_of, respectively. They add parentheses around original
json::member_of and json::const_member_of. If you do not like macro, you still can  
do it on your own. i.e. 

```C++
  (json::memboer_of(var))["something"](123).string() ; // and
  if( (json::const_member_of(c_var))["others"] ) {
    // ...
  }
```

See test/accessor.cpp for more examples.

## Processing of Characters 
  
  - Support ASCII and UNICODE (other than that will not work). 

  - Support escaped string.

# JSON-CC v.s. JSON SPIRIT

[JSON SPIRIT](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented) 
is a famous JSON parser that has some similar implementation with
yangacer::json.

1. json\_spirit wraps boost::variant as an internal structure and dose not
   allow access to a terminal value. e.g. numeric and string. Either
   boost::apply\_visitor can not be applied.

2. json\_spirit can parse data a little bit faster. Hereby are some
   benchmarks:
  
  <pre>
    input(mb) yangacer(sec) json_spirit yangacer(premodel)
    5.6       0.52          0.44        0.49
    11        1.02          0.88        0.97
    16        1.54          1.30        1.47
    21        2.03          1.77        1.93
    37        3.55          3.14        3.4
  </pre>

# Tips

## Boost parsing speed with premodel

If you have knowledge of what fields and what types will be constructed
before parsing. You can premodel it. e.g.

```C++
  // Data to be parsed
  // {"data":[1,2], "name":"acer", "ref":{}}

  using namespace yangacer;
  json::object_t obj;
  obj["data"] = json::array_t();
  obj["name"] = std::string();
  obj["ref"] = json::object_t();

  // do parse here after
```

## Read from std::istream

```C++
  #include "parse.hpp"
  #include "json/variant.hpp"
  #include "json/parser.hpp"

  bool parse(std::istream &in, json::var_t &result)
  {
    using namespace yangacer;
    
    in.usetf(std::ios::skipws); // !!!Important
    json::istream_iterator beg(in), end;
    return json::phrase_parse(beg, end, result);
  }
```
## Deal with numeric type

Idealy, we would like to write code like

```C++
json::var_t var;
mbof(var)["number"] = 123;
```

Though, this results in ambiguous assignment of boost::variant since
there are several numeric types supplied in a variant which can be
converted to each other implicitly. For different architecture, one
can use different workrounds. 

For amd64:

```C++
mbof(var)["number"] = 123l; // read as `123 L'
```
For x86:

```C++
mbof(var)["number"] = 123ll; // read as `123 L L'
```

The most portable way is to use std::intmax_t or boost::intmax_t 
provided by \<cstdint> or \<boost/cstdint.hpp>.

```C++
mbof(var)["number"] = intmax_t(123);
```

