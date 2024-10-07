
#include "QueueSafe.hpp"
#include <algorithm>
#include <functional>
#include <mutex>

template<typename T>
void
QueueSafe<T>::push(T const val)
{
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _queue.push(move(val));
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

template bool QueueSafe<std::function<void()>>::empty();
template void QueueSafe<std::function<void()>>::push(std::function<void()> const val);
template std::function<void()> QueueSafe<std::function<void()>>::pop();
