<link href="markdown.css" rel="stylesheet" type="text/css"><link/>
#Qickstart

1. Build and install library

>cd YA-JSON;mkdir build;cd build;cmake ..;make install

2. Read main.cpp to see how to use parser.

3. Read util.\* to see how a visitor looked like.

#Features

##1. A JSON object is modeled by std::map&lt;std::string, boost::variant&gt;. 

    using namespace yangacer;
    json::object_t obj;
    obj["str"] = "string"; 
    obj["arr"] = json::array_t(3);
    typedef json::array_t::iterator iter;
    for(iter i = obj["arr"].begin(); i != obj["arr"].end();++i)
      // process elements in array

##2. Use boost::spirit::qi to parse data.

##3. A library provides pre-built grammar of frequently used char iterator types. 

The types are std::string::const\_iterator, char const\*, and boost::spirit::istream\_iterator. 
The boost::spirit::istream\_iterator is an adaptor that allow a parser to
read input from std::istream class.

##4. A simple json::print visitor can be used to print a JSON variant.

##5. Support ASCII and UNICODE (other than that will not work). 

##6. Support escaped string.

#yangacer::json v.s. JSON SPIRIT

[JSON SPIRIT](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented) 
is a famous JSON parser that has some similar implementation with
yangacer::json.

##1. json\_spirit wraps boost::variant as an internal structure and dose not
   allow access to a terminal value. e.g. numeric and string. Either
   boost::apply\_visitor can not be used.

##2. json\_spirit can parse data a little bit faster. Hereby are some
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

