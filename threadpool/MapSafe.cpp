
#include "MapSafe.hpp"
#include <glm/ext/vector_float2.hpp>
#include <map>
#include <mutex>
#include <utility>

#include "../world/Region.h"


template<typename T, typename X>
void
MapSafe<T, X>::insert(T key, X value)
{
    std::unique_lock<std::mutex> lock(_mtx);
    _map.insert(std::make_pair(key, value));
}

template<typename T, typename X>
X
MapSafe<T, X>::at(T key) {
  std::unique_lock<std::mutex> lock(_mtx);
  return _map.at(key);
}

template<typename T, typename X>
bool
MapSafe<T, X>::contains(T key)
{
    std::unique_lock<std::mutex> lock(_mtx);
    return _map.contains(key);
}

template<typename T, typename X>
X&
MapSafe<T, X>::operator[](T key)
{
    std::unique_lock<std::mutex> lock(_mtx);
    return _map[key];
}

template<typename T, typename X>
X&
MapSafe<T, X>::at_ptr(T key)
{
    std::unique_lock<std::mutex> lock(_mtx);
    return _map[key];
}


    // MapSafe<int, ChunkInformation> ChunkInfo;
template void MapSafe<std::pair<int, int>, Region>::insert(std::pair<int, int>, Region);
template Region MapSafe<std::pair<int, int>, Region>::at(std::pair<int, int>);
template bool MapSafe<std::pair<int, int>, Region>::contains(std::pair<int, int>);
template Region& MapSafe<std::pair<int, int>, Region>::operator[](std::pair<int, int>);
template Region& MapSafe<std::pair<int, int>, Region>::at_ptr(std::pair<int, int>);

template void MapSafe<int, ChunkInformation>::insert(int, ChunkInformation);
template ChunkInformation MapSafe<int, ChunkInformation>::at(int);
template bool MapSafe<int, ChunkInformation>::contains(int);
template ChunkInformation& MapSafe<int, ChunkInformation>::operator[](int);
template ChunkInformation& MapSafe<int, ChunkInformation>::at_ptr(int);
