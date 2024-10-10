
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "QueueSafe.hpp"
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>
class ThreadPool
{
  protected:
    std::vector<std::thread> _threads;
    std::mutex _mtx;
    std::mutex _data_mtx;
    std::condition_variable _cv;
    int _n_threads;
    bool _stop;

  public:
    explicit ThreadPool(int num_threads);
    explicit ThreadPool();

    QueueSafe<std::function<void()>> _task_queue;
    ~ThreadPool();

  template<typename T> auto enqueue(T task) -> std::future<decltype(task())> {
    auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
    _task_queue.push([=] {
      (*wrapper)();
    });
    _cv.notify_one();
    return wrapper->get_future();
  }
};

#endif
