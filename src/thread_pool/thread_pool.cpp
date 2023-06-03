#include "thread_pool.h"

void ThreadPool::thread_loop() {
  while (true) {
    std::function<void()> job;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(lock, [this] {
        return !jobs.empty() || should_terminate;
      });
      if (should_terminate) {
        return;
      }
      job = jobs.front();
      jobs.pop();
    }
    job();
  }
}

void ThreadPool::queue_job(const std::function<void()>& job) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.push(job);
  }
  mutex_condition.notify_one();
}

void ThreadPool::stop() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    should_terminate = true;
  }
  mutex_condition.notify_all();
  for (std::thread& active_thread : threads) {
    active_thread.join();
  }
  threads.clear();
}

bool ThreadPool::busy() {
  bool pool_busy;
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    pool_busy = !jobs.empty();
  }
  return pool_busy;
}

ThreadPool::ThreadPool(int max_threads) {
  max_t = std::min((int) std::thread::hardware_concurrency(), max_threads);
  threads.resize(max_t);
  for (uint32_t i = 0; i < max_t; i++) {
    threads.at(i) = std::thread(&ThreadPool::thread_loop, this);
  }
}
ThreadPool::~ThreadPool() {
  stop();
}
