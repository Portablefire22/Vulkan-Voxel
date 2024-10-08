#ifndef CHUNK_POOL_H
#define CHUNK_POOL_H

#include "QueueSafe.hpp"
#include "ThreadPool.hpp"
#include <functional>

#include "../vulkan/vk_engine.h"

class ChunkPool: protected ThreadPool {
  QueueSafe<RenderObject> _chunkMeshQueue;
  QueueSafe<std::function<RenderObject()>> _task_queue;
  ChunkPool(int num_threads);
  ChunkPool();
  void enqueue(std::function<RenderObject()>);
}; 

#endif
