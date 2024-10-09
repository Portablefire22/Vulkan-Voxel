#ifndef MAP_SAFE_H
#define MAP_SAFE_H
#include <map>
#include <mutex>

template<typename T, typename X>
class MapSafe
{
  public:
    void insert(T key, X value);
    X at(T key);
    bool contains(T key);
    X& operator[](T key);
    X& at_ptr(T key);  
  // bool isEmpty(T key);
  
  MapSafe<T,X>() {
    _map = std::map<T, X>();
  }

  private:
    std::mutex _mtx;
    std::map<T, X> _map;
};

#endif
