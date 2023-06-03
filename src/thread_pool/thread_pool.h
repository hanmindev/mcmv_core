#ifndef MCMV_CORE_SRC_THREAD_POOL_H_
#define MCMV_CORE_SRC_THREAD_POOL_H_

#include <functional>
#include <mutex>
#include <queue>

// modified from https://stackoverflow.com/questions/15752659/thread-pooling-in-c11

class ThreadPool {
 public:
    explicit ThreadPool(int max_t);
    ~ThreadPool();
  void queue_job(const std::function<void()>& job);
  void stop();
  bool busy();

 private:
  void thread_loop();

  bool should_terminate = false;
  int max_t;
  int active_t = 0;
  std::mutex queue_mutex;
  std::condition_variable mutex_condition;
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> jobs;
};

#endif //MCMV_CORE_SRC_THREAD_POOL_H_
