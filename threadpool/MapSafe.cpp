
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
    std::unique_lock<std::shared_mutex> lock(_mtx);
    _map.insert(std::make_pair(key, value));
}

template<typename T, typename X>
X
MapSafe<T, X>::at(T key) {
  std::shared_lock<std::shared_mutex> lock(_mtx);
  return _map.at(key);
}

template<typename T, typename X>
bool
MapSafe<T, X>::contains(T key)
{
    std::shared_lock<std::shared_mutex> lock(_mtx);
    return _map.contains(key);
}

template<typename T, typename X>
X&
MapSafe<T, X>::operator[](T key)
{
    std::shared_lock<std::shared_mutex> lock(_mtx);
    return _map[key];
}

template<typename T, typename X>
X&
MapSafe<T, X>::at_ptr(T key)
{
    std::shared_lock<std::shared_mutex> lock(_mtx);
    return _map[key];
}

template<typename T, typename X>
auto MapSafe<T, X>::begin() -> decltype(_map.begin())
{
    std::shared_lock<std::shared_mutex> lock(_mtx);
    return _map.begin();
}
template<typename T, typename X>
auto MapSafe<T, X>::end()
{
    std::shared_lock<std::shared_mutex> lock(_mtx);
    return _map.end();
}

template<typename T, typename X>
void MapSafe<T, X>::del() {
  for (auto const& [key, val] : _map) {
    delete(val);
  }
}

    // MapSafe<int, ChunkInformation> ChunkInfo;
template void MapSafe<std::pair<int, int>, Region*>::insert(std::pair<int, int>, Region*);
template Region* MapSafe<std::pair<int, int>, Region*>::at(std::pair<int, int>);
template bool MapSafe<std::pair<int, int>, Region*>::contains(std::pair<int, int>);
template void MapSafe<std::pair<int, int>, Region*>::del();

template void MapSafe<int, chunk::Chunk*>::insert(int, chunk::Chunk*);
template chunk::Chunk* MapSafe<int, chunk::Chunk*>::at(int);
template bool MapSafe<int, chunk::Chunk*>::contains(int);
template void MapSafe<int, chunk::Chunk*>::del();

template void MapSafe<int, ChunkInformation>::insert(int, ChunkInformation);
template ChunkInformation MapSafe<int, ChunkInformation>::at(int);
template bool MapSafe<int, ChunkInformation>::contains(int);
template ChunkInformation& MapSafe<int, ChunkInformation>::operator[](int);
template ChunkInformation& MapSafe<int, ChunkInformation>::at_ptr(int);
