#pragma once

#include <nall/function.hpp>

namespace nall {

template<typename T>
struct property {
  template<typename... P>
  inline property(P&&... p) {
    new((void*)(&instance.object)) T(forward<P>(p)...);
    get = [&]() -> T { return instance.object; };
    set = [&](T value) -> void { instance.object = value; };
  }

  inline property(T* pointer) {
    instance.pointer = pointer;
    get = [&]() -> T { return *instance.pointer; };
    set = [&](T value) -> void { *instance.pointer = value; };
  }

  inline property(function<T ()> get, function<void (T)> set) : get(get), set(set) {
  }

  inline operator T() { return get(); }
  inline operator const T() const { return get(); }

  inline auto operator()() -> T { return get(); }
  inline auto operator()() const -> const T { return get(); }

  inline auto operator++(int) { T value = get(); set(get() + 1); return value; }
  inline auto operator--(int) { T value = get(); set(get() - 1); return value; }

  inline auto& operator++() { return set(get() + 1), *this; }
  inline auto& operator--() { return set(get() - 1), *this; }

  template<typename U> inline auto& operator  =(const U& value) { return set(         value), *this; }
  template<typename U> inline auto& operator *=(const U& value) { return set(get()  * value), *this; }
  template<typename U> inline auto& operator /=(const U& value) { return set(get()  / value), *this; }
  template<typename U> inline auto& operator %=(const U& value) { return set(get()  % value), *this; }
  template<typename U> inline auto& operator +=(const U& value) { return set(get()  + value), *this; }
  template<typename U> inline auto& operator -=(const U& value) { return set(get()  - value), *this; }
  template<typename U> inline auto& operator<<=(const U& value) { return set(get() << value), *this; }
  template<typename U> inline auto& operator>>=(const U& value) { return set(get() >> value), *this; }
  template<typename U> inline auto& operator &=(const U& value) { return set(get()  & value), *this; }
  template<typename U> inline auto& operator ^=(const U& value) { return set(get()  ^ value), *this; }
  template<typename U> inline auto& operator |=(const U& value) { return set(get()  | value), *this; }

private:
  function<T ()> get;
  function<void (T)> set;
  union Union {
    Union() {}
    ~Union() {}
    T object;
    T* pointer;
    char storage[sizeof(T)];
  } instance;
};

}
