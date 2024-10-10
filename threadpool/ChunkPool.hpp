#ifndef CHUNK_POOL_H
#define CHUNK_POOL_H

#include "QueueSafe.hpp"
#include "ThreadPool.hpp"
#include <functional>

template <typename T>
class ChunkPool: protected ThreadPool {
public:
  QueueSafe<std::function<T()>> _task_queue;
  QueueSafe<T> _chunkQueue;
  ChunkPool<T>(int num_threads);
  ChunkPool<T>();
  void enqueue(std::function<T()>);
};

#endif
