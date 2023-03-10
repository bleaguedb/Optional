
# Optional

C++ optional class with error log method

- header only library
- required C++20 or later
- No dependencies except for standard libraries

## example

```C++

#include "bdb/Optional.hpp"

#include<cassert>

using bdb::Optional;

int main(){
  Optional<int> oi = 1;//basic usage
  Optional oi2 = 2; //deduce template parameter
  std::cout << oi <<" "<< oi2 << std::endl;//has stream operator

  Optional<> oT = true; //boolean is also available;
  //Optional<bool> ob = ture; //ileagal use (T=bool)
  Optional oF = false; //T=std::monostate when using boolean
  assert(!oF.has_value());//false is expressed as std::nullopt
  std::cout << oT <<" "<< oF << std::endl;//has stream operator for bool

  //error log  method
  auto maybe_fail = [](bool b)->Optional<int>{if(b) return 3; else return std::nullopt;};
  const auto result1 = maybe_fail(true).err_log("result1");//does nothing
  assert(result1 == 3);

  const auto result2 = maybe_fail(false).err_log("result2"); //result2 @ err_log[src/test.cpp:29]
  assert(result2 == std::nullopt);

  //same error handling interface for boolean
  auto maybe_false = [](bool b)->Optional<>{return b;};
  const auto result3 = maybe_false(true).err_log("result3");
  assert(result3 == true);
  const auto result4 = maybe_false(false).err_log("result4"); //result4 @ err_log[src/test.cpp:36]
  assert(result4 == false);
  assert(result4 == std::nullopt);
}

```

## motivation

In C++23, monadic methods will be introduced, but support for void-returning functions has been removed. Therefore, if you want to implement a log method for debugging, the code will be...

```C++
auto maybe_fail = [](){return std::nullopt;};

//desired
auto hoge = maybe_fail().or_else{[]{std::cout<<"message"<<std::endl;}};
//or
auto log = []{std::cout<<"message"<<std::endl;};
auto hoge = maybe_fial().or_else(log);

//actual
auto hoge = maybe_fail().or_else{[]{
    std::cout<<"message"<<std::endl;
    return std::nullopt;
  }
}

//and log function is maybe impossible because return type of log
//function is not able to determin

```

So, I made a simple optional wrapper-like class with an error log method. It is easy to use for simple debugging.




