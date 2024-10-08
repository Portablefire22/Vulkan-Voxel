
#include "ThreadPool.hpp"
#include "QueueSafe.hpp"
#include <functional>
#include <future>
#include <mutex>
#include <thread>

ThreadPool::ThreadPool(int num_threads)
  : _n_threads(num_threads)
  , _stop(false)
{
    // Create the threads
    for (int i = 0; i < _n_threads; i++) {
        _threads.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                // New scope so it unlocks before executing, allowing other
                // threads to queue tasks
                {
                    std::unique_lock<std::mutex> lock(_mtx);

                    // Wait until there is a task, or if the queue was stopped
                    _cv.wait(lock,
                             [this] { return !_task_queue.empty() || _stop; });

                    if (_task_queue.empty() && _stop)
                        return;

                    task = _task_queue.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::ThreadPool()
  : ThreadPool::ThreadPool(std::thread::hardware_concurrency())
{
}

ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _stop = true;
    lock.unlock();
    _cv.notify_all();

    for (auto& thread : _threads) {
        thread.join();
    }
}
// void
// ThreadPool::enqueue(std::function<void()> task)
// {
//     _task_queue.push(task);
// }
// template<typename T> std::future<decltype(T())>
// ThreadPool::enqueue(std::function<void()> task) {
//   auto wrapper = std::make_shared<std::packaged_task<decltype(task())
//   ()>>(std::move(task)); _task_queue.push([=] {
//     (*wrapper)();
//   });
//   return wrapper->get_future();
// }
