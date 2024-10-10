#include "ChunkPool.hpp"
#include <iostream>
#include <ostream>
#include <syncstream>
void
ChunkPool::enqueue(std::function<RenderObject()> task)
{
    _task_queue.push(task);
    _cv.notify_one();
}

ChunkPool::ChunkPool(int num_threads)
{
    // Create the threads
    for (int i = 0; i < _n_threads; i++) {
        _threads.emplace_back([this] {
            std::queue<RenderObject> render_queue;
            while (true) {
                std::function<RenderObject()> task;
                // New scope so it unlocks before executing, allowing other
                // threads to queue tasks
                {
                    std::unique_lock<std::mutex> lock(_mtx);
                    // If there is nothing to do, then submit meshes for
                    // rendering

                    // Wait until there is a task, or if the queue was stopped
                    _cv.wait(lock, [=, &render_queue, this] {
                        return !_task_queue.empty() || _stop;
                    });

                    if (_task_queue.empty() && _stop)
                        return;
                    task = _task_queue.pop();
                }
                // This specialised chunkpool should always return a chunk-mesh
                // from it's tasks.
                render_queue.push(task());

                if (_task_queue.empty() || (render_queue.size() % 25 == 0 &&
                                            render_queue.size() != 0)) {
                    if (_data_mtx.try_lock()) {
                        // std::unique_lock<std::mutex> lock(_mtx);
                        while (!render_queue.empty()) {
                            auto x = std::move(render_queue.front());
                            render_queue.pop();
                            _chunkMeshQueue.push(std::move(x));
                        }
                        _data_mtx.unlock();
                    }
                }
                // auto x = std::move(render_queue.front());
                //                         render_queue.pop();
                //                         _chunkMeshQueue.push(std::move(x));
                // If there is nothing to do, then submit meshes for rendering
            }
        });
    }
}

ChunkPool::ChunkPool()
  : ChunkPool::ChunkPool(std::thread::hardware_concurrency())
{
}
