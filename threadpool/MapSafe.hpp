#ifndef MAP_SAFE_H
#define MAP_SAFE_H
#include <iterator>
#include <map>
#include <mutex>

template<typename T, typename X>
class MapSafe
{
 
  private:
    std::mutex _mtx;
    std::map<T, X> _map;

public:
    void insert(T key, X value);
    X at(T key);
    bool contains(T key);
    X& operator[](T key);
    X& at_ptr(T key);
    void del();

  auto begin() -> decltype(_map.begin());
  // bool isEmpty(T key);
  auto end();
  
  MapSafe<T,X>() {
    _map = std::map<T, X>();
  }

};

#endif
