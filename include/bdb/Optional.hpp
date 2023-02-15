#ifndef BDB_OPTIONAL_HPP
#define BDB_OPTIONAL_HPP

#include <optional>

#include <iostream>
#include <string_view>
#include <variant>//std::monostate
#include <source_location>

namespace bdb{

  template<typename T=std::monostate>
  class Optional : private std::optional<T>{
    public:
    using std::optional<T>::optional;

    using std::optional<T>::operator=;
    using std::optional<T>::emplace;
    using std::optional<T>::swap;
    using std::optional<T>::reset; 

    using std::optional<T>::operator*;
    using std::optional<T>::operator->;
    using std::optional<T>::operator bool;
    using std::optional<T>::has_value;
    using std::optional<T>::value;
    using std::optional<T>::value_or;

    //bool constructor
    template<typename BOOL>
    Optional(BOOL b) noexcept requires std::same_as<T,std::monostate> && std::same_as<BOOL,bool>{
      if(b) *this = std::monostate{};
      else *this = std::nullopt;
    };

    constexpr bool operator*() const& noexcept requires std::same_as<T,std::monostate>{
      return has_value();
    }

    Optional<T> err_log(std::string_view sv="",std::ostream& os = std::cerr,std::source_location sl = std::source_location::current()) const noexcept{
      if(*this) return *this;
      else{
        os << sv<<" @ "<<"err_log["<<sl.file_name()<<":"<<sl.line()<<"]" <<std::endl;
        return std::nullopt;
      }
    }
    friend inline std::ostream& operator<<(std::ostream& os,const Optional<T>& opt) noexcept{
      if constexpr(!std::is_same_v<T,std::monostate>){
        if(opt){return os<<opt.value();}else{return os<<"nullopt";};
      }else{
        if(opt){return os<<"true";}else{return os<<"false";};
      }
    }
    
  };

  //compare
  template<typename T>
  inline constexpr bool operator==(const Optional<T>& lhs,const Optional<T>& rhs){
    return lhs && rhs ? (*lhs == *rhs) : (static_cast<bool>(lhs) == static_cast<bool>(rhs));
  }
  template<typename T>
  inline constexpr auto operator<=>(const Optional<T>& lhs, const Optional<T>& rhs){
    return lhs && rhs ? (*lhs <=> *rhs) : (static_cast<bool>(lhs) <=> static_cast<bool>(rhs));
  }

  //compare with nullopt
  template<typename T>
  inline constexpr bool operator==(const T& lhs,std::nullopt_t){return !lhs;}
  template<typename T>
  inline constexpr auto operator<=>(const Optional<T>& lhs, std::nullopt_t){
    return static_cast<bool>(lhs) <=> false;
  }
  //compare with value
  template<typename T>
  inline constexpr bool operator==(const Optional<T>& lhs,const T& rhs){
    return lhs ? (*lhs == rhs) : false;
  }
  template<typename T>
  inline constexpr auto operator<=>(const Optional<T>& lhs, const T& rhs){
    return lhs ? (*lhs <=> rhs) : std::strong_ordering::less;
  }
  
  template<typename BOOL>
  inline constexpr bool operator==(const Optional<std::monostate>& lhs,BOOL rhs)
  requires std::same_as<bool,BOOL>{
    return *lhs == rhs;
  }

  //deduction guid
  Optional(bool) -> Optional<std::monostate>;

  template<typename T>
  Optional(T) -> Optional<T>;

}//ns bdb

#endif