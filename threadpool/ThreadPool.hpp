
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
  private:
    int _n_threads;
    QueueSafe<std::function<void()>> _task_queue;
    std::vector<std::thread> _threads;
    std::mutex _mtx;
    std::condition_variable _cv;
    bool _stop;

  public:
    explicit ThreadPool(int num_threads);
    explicit ThreadPool();
    ~ThreadPool();

    // template<typename T> std::future<decltype(T())> enqueue(std::function<void()> task);
    // template<typename T> auto enqueue(T task) -> std::future<decltype(task())>;
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
