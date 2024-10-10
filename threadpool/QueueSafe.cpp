
#include "QueueSafe.hpp"
#include <algorithm>
#include <functional>
#include <mutex>

#include "../vulkan/vk_engine.h"
template<typename T>
void
QueueSafe<T>::push(T const val)
{
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _queue.push(std::move(val));
    }
    _cv.notify_one();
}

template<typename T>
T
QueueSafe<T>::pop()
{
    T task;
    {
        std::unique_lock<std::mutex> lock(_mtx);
        task = std::move(_queue.front());
        _queue.pop();
    }
    return task;
}

template<typename T>
bool
QueueSafe<T>::empty()
{
    return _queue.empty();
}

template<typename T>
size_t
QueueSafe<T>::size()
{
    return _queue.size();
}

template bool
QueueSafe<std::function<void()>>::empty();
template void
QueueSafe<std::function<void()>>::push(std::function<void()> const val);
template std::function<void()>
QueueSafe<std::function<void()>>::pop();

template bool
QueueSafe<std::function<RenderObject()>>::empty();
template void
QueueSafe<std::function<RenderObject()>>::push(
  std::function<RenderObject()> const val);
template size_t
QueueSafe<std::function<RenderObject()>>::size();
template std::function<RenderObject()>
QueueSafe<std::function<RenderObject()>>::pop();

template RenderObject
QueueSafe<RenderObject>::pop();
template bool
QueueSafe<RenderObject>::empty();
template void
QueueSafe<RenderObject>::push(RenderObject const val);

template bool
QueueSafe<std::function<chunk::Chunk*()>>::empty();
template void
QueueSafe<std::function<chunk::Chunk*()>>::push(
  std::function<chunk::Chunk*()> const val);
template size_t
QueueSafe<std::function<chunk::Chunk*()>>::size();
template std::function<chunk::Chunk*()>
QueueSafe<std::function<chunk::Chunk*()>>::pop();

template chunk::Chunk*
QueueSafe<chunk::Chunk*>::pop();
template bool
QueueSafe<chunk::Chunk*>::empty();
template void
QueueSafe<chunk::Chunk*>::push(chunk::Chunk* const val);
