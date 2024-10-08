
#ifndef QUEUE_SAFE_H
#define QUEUE_SAFE_H

#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class QueueSafe
{
  private:
    std::queue<T> _queue;
    std::condition_variable _cv;
    std::mutex _mtx;

  public:
    void push(T const val);

    T pop();
    bool empty();
    size_t size();

};

#endif
