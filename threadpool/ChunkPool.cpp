#include "ChunkPool.hpp"
void
ChunkPool::enqueue(std::function<RenderObject()> task)
{
    _task_queue.push(task);
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

                    // Wait until there is a task, or if the queue was stopped
                    _cv.wait(lock,
                             [this] { return !_task_queue.empty() || _stop; });

                    if (_task_queue.empty() && _stop)
                        return;

                    // If there is nothing to do, then submit meshes for rendering
                    if (_task_queue.empty()) {
                        {
                            std::unique_lock<std::mutex> lock(_mtx);
                            while (!render_queue.empty()) {
                                _chunkMeshQueue.push(std::move(render_queue.front()));
                                render_queue.pop();
                            }
                        }
                    }

                    task = _task_queue.pop();
                }
                // This specialised chunkpool should always return a chunk-mesh from it's tasks. 
                
                render_queue.push(task());
            }
        });
    }
}

ChunkPool::ChunkPool()
  : ChunkPool::ChunkPool(std::thread::hardware_concurrency())
{
}
