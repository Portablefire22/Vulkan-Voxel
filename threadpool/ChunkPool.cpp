#include "ChunkPool.hpp"

template <typename T>
void
ChunkPool<T>::enqueue(std::function<T()> task)
{
    _task_queue.push(task);
    _cv.notify_one();
}

template <typename T>
ChunkPool<T>::ChunkPool(int num_threads)
{
    _n_threads = num_threads;
    // Create the threads
    for (int i = 0; i < _n_threads; i++) {
        _threads.emplace_back([this] {
            std::queue<T> cache_queue;
            while (true) {
                std::function<T()> task;
                // New scope so it unlocks before executing, allowing other
                // threads to queue tasks
                {
                    std::unique_lock<std::mutex> lock(_mtx);
                    // If there is nothing to do, then submit meshes for
                    // rendering

                    // Wait until there is a task, or if the queue was stopped
                    _cv.wait(lock, [=, &cache_queue, this] {
                        return !_task_queue.empty() || _stop;
                    });

                    if (_task_queue.empty() && _stop)
                        return;
                    task = _task_queue.pop();
                }
                // This specialised chunkpool should always return a chunk-mesh
                // from it's tasks.
                cache_queue.push(task());

                if (_task_queue.empty() || (cache_queue.size() % 25 == 0 &&
                                            cache_queue.size() != 0)) {
                    if (_data_mtx.try_lock()) {
                        // std::unique_lock<std::mutex> lock(_mtx);
                        while (!cache_queue.empty()) {
                            auto x = std::move(cache_queue.front());
                            cache_queue.pop();
                            _chunkQueue.push(std::move(x));
                        }
                        _data_mtx.unlock();
                    }
                }
                // If there is nothing to do, then submit meshes for rendering
            }
        });
    }
}

template <typename T>
ChunkPool<T>::ChunkPool()
  : ChunkPool::ChunkPool(std::thread::hardware_concurrency())
{
}

#include "../vulkan/vk_engine.h"
#include "../world/Chunk.h"

template void
ChunkPool<RenderObject>::enqueue(std::function<RenderObject()>);
template
ChunkPool<RenderObject>::ChunkPool(int num_threads);
template
ChunkPool<RenderObject>::ChunkPool();

template void
ChunkPool<chunk::Chunk*>::enqueue(std::function<chunk::Chunk*()>);
template
ChunkPool<chunk::Chunk*>::ChunkPool(int num_threads);
template
ChunkPool<chunk::Chunk*>::ChunkPool();
