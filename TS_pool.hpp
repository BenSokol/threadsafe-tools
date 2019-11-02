/**
* @Filename: TS_pool.hpp
* @Author:   Ben Sokol
* @Email:    ben@bensokol.com
* @Created:  March 26th, 2019 [8:17am]
* @Modified: November 1st, 2019 [5:43pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_POOL_HPP
#define TS_POOL_HPP

#include <cstdint>  // intmax_t
#include <cstdlib>  // size_t

#include <algorithm>           // std::min
#include <cassert>             // assert
#include <condition_variable>  // std::condition_variable
#include <functional>          // std::bind, std::function
#include <future>              // std::future, std::packaged_task
#include <limits>              // std::numeric_limits
#include <memory>              // std::make_shared
#include <mutex>               // std::mutex, std::unique_lock
#include <queue>               // std::priority_queue
#include <stdexcept>           // std::runtime_error
#include <thread>              // std::thread
#include <tuple>               // std::tuple, std::get
#include <type_traits>         // std::result_of, std::is_arithmetic
#include <utility>             // std::forward
#include <vector>              // std::vector

namespace TS {
  template <typename priority_type = intmax_t>
  class pool {
  public:
    enum class priority : priority_type {
      low = std::numeric_limits<priority_type>::min(),
      normal = 0,
      high = std::numeric_limits<priority_type>::max()
    } priority;

    explicit pool(std::size_t threads = std::max<size_t>(2, std::thread::hardware_concurrency()));

    ~pool();

    // enqueue with priority
    template <typename function_type, typename... Args>
    std::future<typename std::result_of<function_type(Args...)>::type> enqueue(priority_type aPriority,
                                                                               function_type &&f,
                                                                               Args &&... args);

    // enqueue with normal (0) priority
    template <typename function_type, typename... Args>
    std::future<typename std::result_of<function_type(Args...)>::type> enqueue(function_type &&f, Args &&... args);

    // wait for all tasks to complete
    void wait();

    // get number of tasks remaining
    size_t remainingTasks() const;

  private:
    typedef std::tuple<priority_type, std::function<void()>> prioritized_task;
    typedef std::function<bool(const prioritized_task &, const prioritized_task &)> prioritizedTaskCompare;

    static bool prioritizedTaskCompareFunc(prioritized_task const &l, prioritized_task const &r);
    void taskThread();

    bool mStop;
    std::vector<std::thread> mWorkers;
    std::priority_queue<prioritized_task, std::vector<prioritized_task>, prioritizedTaskCompare> mTasks;
    std::mutex mQueueMutex;
    std::mutex mTaskFinishedMutex;
    std::condition_variable mQueueUpdatedCondition;
    std::condition_variable mTaskFinishedCondition;
  };


  template <typename priority_type>
  /* static */ bool pool<priority_type>::prioritizedTaskCompareFunc(prioritized_task const &l,
                                                                    prioritized_task const &r) {
    return std::get<0>(l) < std::get<0>(r);
  }


  template <typename priority_type>
  void pool<priority_type>::taskThread() {
    // run tasks in queue until mStop == true || mTasks.empty()
    for (;;) {
      std::function<void()> function_task;
      {
        std::unique_lock<std::mutex> lock(mQueueMutex);
        mQueueUpdatedCondition.wait(lock, [this]() {
          return (mStop || !mTasks.empty());
        });

        //End the worker thread immediately if it is asked to stop
        if (mStop) {
          return;
        }
        else {
          function_task = std::get<1>(std::move(mTasks.top()));
          mTasks.pop();
        }
      }

      // run task and notify when complete.
      function_task();
      mTaskFinishedCondition.notify_all();
    }
  }


  template <typename priority_type>
  pool<priority_type>::pool(size_t aThreads) {
    // Verify priority type is valid
    static_assert(std::is_integral<priority_type>::value, "priority_type must be an integral type.");

    // Initialize member variables
    mStop = false;
    mWorkers = std::vector<std::thread>();
    mTasks = std::priority_queue<prioritized_task, std::vector<prioritized_task>, prioritizedTaskCompare>(
      prioritizedTaskCompareFunc);

    // Start task threads
    for (size_t i = 0; i < aThreads; i++) {
      mWorkers.emplace_back(&pool::taskThread, this);
    }
  }


  template <typename priority_type>
  pool<priority_type>::~pool() {
    {
      std::unique_lock<std::mutex> lock(mQueueMutex);
      mStop = true;
    }

    mQueueUpdatedCondition.notify_all();
    for (std::thread &worker : mWorkers) {
      worker.join();
    }
  }


  template <typename priority_type>
  template <typename function_type, typename... Args>
  std::future<typename std::result_of<function_type(Args...)>::type>
    pool<priority_type>::enqueue(priority_type aPriority, function_type &&f, Args &&... args) {
    auto packaged_task = std::make_shared<std::packaged_task<typename std::result_of<function_type(Args...)>::type()>>(
      std::bind(std::forward<function_type>(f), std::forward<Args>(args)...));

    std::future<typename std::result_of<function_type(Args...)>::type> res = packaged_task->get_future();

    {
      std::unique_lock<std::mutex> lock(mQueueMutex);

      // don't allow enqueueing after stopping the pool
      if (mStop) {
        throw std::runtime_error("enqueue on stopped ThreadPool");
      }

      mTasks.emplace(prioritized_task(aPriority, [packaged_task]() {
        (*packaged_task)();
      }));
    }

    mQueueUpdatedCondition.notify_one();
    return res;
  }


  template <typename priority_type>
  template <typename function_type, typename... Args>
  std::future<typename std::result_of<function_type(Args...)>::type> pool<priority_type>::enqueue(function_type &&f,
                                                                                                  Args &&... args) {
    return enqueue(priority::normal, f, args...);
  }


  template <typename priority_type>
  void pool<priority_type>::wait() {
    if (std::any_of(mWorkers.begin(), mWorkers.end(), [&](const auto &worker) {
          return std::this_thread::get_id() == worker.get_id();
        })) {
      throw std::runtime_error("Cannot wait on threadpool from within a task");
    }

    while (!mTasks.empty()) {
      std::unique_lock<std::mutex> lock(mTaskFinishedMutex);
      mTaskFinishedCondition.wait(lock, [this]() {
        return mTasks.empty();
      });
    }
  }


  template <typename priority_type>
  size_t pool<priority_type>::remainingTasks() const {
    return mTasks.size();
  }

}  // namespace TS


#endif  // #ifndef TS_POOL_HPP
