/**
* @Filename: main.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  November 1st, 2019 [10:37am]
* @Modified: November 1st, 2019 [5:41pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <ctime>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#include "TS_pool.hpp"
#include "UTL_assert.h"

static const size_t NUM_TESTS = 100000;
static const size_t ITER = 10000;
static size_t *randomNumbers;
volatile std::atomic<size_t> total = 0;
static std::atomic<size_t> completed = 0;

std::mutex mtx;

double get_cpu_clock_time(clockid_t clk_id = CLOCK_PROCESS_CPUTIME_ID);
void task(size_t num, const std::string &name);
void singleThreadTest();
void multiThreadTest();
void threadpoolTest();

double get_cpu_clock_time(clockid_t clk_id) {
  struct timespec currTime;
  [[maybe_unused]] int status = clock_gettime(clk_id, &currTime);
  UTL_assert(status == 0);
  return currTime.tv_sec + (static_cast<double>(currTime.tv_nsec) * (1.0 / std::nano::den));
}


void task(size_t num, const std::string &name) {
  size_t val = 0;
  for (size_t i = 0; i < ITER; ++i) {
    val += randomNumbers[i] % (num + 1);
  }
  total += val % 1000;
  {
    std::unique_lock<std::mutex> lck(mtx);
    completed++;
    std::cout << "\rRunning " << name << " test: " << completed << "/" << NUM_TESTS << std::flush;
  }
}


void singleThreadTest() {
  const std::string name = "single-thread";
  total = 0;
  completed = 0;
  for (size_t i = 0; i < NUM_TESTS; ++i) {
    task(i * 10, name);
  }
}


void multiThreadTest() {
  const std::string name = "multi-thread";
  total = 0;
  completed = 0;
  std::thread threads[NUM_TESTS];
  for (size_t i = 0; i < NUM_TESTS; ++i) {
    threads[i] = std::thread(&task, i * 10, name);
  }

  for (size_t i = 0; i < NUM_TESTS; ++i) {
    threads[i].join();
  }
}


void threadpoolTest() {
  const std::string name = "threadpool";
  total = 0;
  completed = 0;
  TS::pool<uint8_t> pool(std::min(NUM_TESTS, static_cast<size_t>(std::thread::hardware_concurrency())));
  for (size_t i = 0; i < NUM_TESTS; ++i) {
    pool.enqueue(0, &task, i * 10, name);
  }

  pool.wait();
}


int main() {
  // Initialization
  std::cout << "Initializing thread benchmark: " << std::flush;
  std::chrono::high_resolution_clock::time_point initStartReal = std::chrono::high_resolution_clock::now();
  double initStartCPU = get_cpu_clock_time();
  randomNumbers = new size_t[ITER * 3];
  std::random_device rd;
  for (size_t i = 0; i < ITER; ++i) {
    if (i % 5) {
      randomNumbers[i] = rd();
    }
    else {
      if (i == 0) {
        randomNumbers[i] = rd();
      }
      else {
        randomNumbers[i] = randomNumbers[i - 1];
      }
    }
    if (i % 10000 == 0) {
      std::cout << "\rInitializing thread benchmark: " << (i % ITER) / (ITER / 100) << "%" << std::flush;
    }
  }

  std::chrono::high_resolution_clock::time_point initEndReal = std::chrono::high_resolution_clock::now();
  double initEndCPU = get_cpu_clock_time();
  std::cout << "\rInitializing thread benchmark: Done                     \n";

  // Single thread test
  std::cout << "Running single-thread test: " << std::flush;
  std::chrono::high_resolution_clock::time_point singleThreadStartReal = std::chrono::high_resolution_clock::now();
  double singleThreadStartCPU = get_cpu_clock_time();
  singleThreadTest();
  while (completed != NUM_TESTS) {
    std::this_thread::yield();
  }
  std::chrono::high_resolution_clock::time_point singleThreadEndReal = std::chrono::high_resolution_clock::now();
  double singleThreadEndCPU = get_cpu_clock_time();
  std::cout << "\rRunning single-thread test: Done           \n" << std::flush;

  // Multi thread test
  std::cout << "Running multi-thread test: " << std::flush;
  std::chrono::high_resolution_clock::time_point mutliThreadStartReal = std::chrono::high_resolution_clock::now();
  double multiThreadStartCPU = get_cpu_clock_time();
  multiThreadTest();
  while (completed != NUM_TESTS) {
    std::this_thread::yield();
  }
  std::chrono::high_resolution_clock::time_point multiThreadEndReal = std::chrono::high_resolution_clock::now();
  double multiThreadEndCPU = get_cpu_clock_time();
  std::cout << "\rRunning multi-thread test: Done            \n" << std::flush;

  // Threadpool test
  std::cout << "Running threadpool test: " << std::flush;
  std::chrono::high_resolution_clock::time_point threadpoolStartReal = std::chrono::high_resolution_clock::now();
  double threadpoolStartCPU = get_cpu_clock_time();
  threadpoolTest();
  while (completed != NUM_TESTS) {
    std::this_thread::yield();
  }
  std::chrono::high_resolution_clock::time_point threadpoolEndReal = std::chrono::high_resolution_clock::now();
  double threadpoolEndCPU = get_cpu_clock_time();
  std::cout << "\rRunning threadpool test: Done              \n" << std::flush;

  delete[] randomNumbers;

  // Report time statistics
  std::cout << "Time Statistics:\n\n";

  std::cout << "  Initialization:\n";
  std::cout << "    Real: "
            << std::chrono::duration_cast<std::chrono::duration<double>>(initEndReal - initStartReal).count()
            << " seconds.\n";
  std::cout << "    CPU: " << (initEndCPU - initStartCPU) << " seconds\n\n";

  std::cout << "  single-thread Test:\n";
  std::cout
    << "    Real: "
    << std::chrono::duration_cast<std::chrono::duration<double>>(singleThreadEndReal - singleThreadStartReal).count()
    << " seconds.\n";
  std::cout << "    CPU: " << (singleThreadEndCPU - singleThreadStartCPU) << " seconds\n\n";

  std::cout << "  multi-thread Test:\n";
  std::cout
    << "    Real: "
    << std::chrono::duration_cast<std::chrono::duration<double>>(multiThreadEndReal - mutliThreadStartReal).count()
    << " seconds.\n";
  std::cout << "    CPU: " << (multiThreadEndCPU - multiThreadStartCPU) << " seconds\n\n";

  std::cout << "  threadpool Test:\n";
  std::cout
    << "    Real: "
    << std::chrono::duration_cast<std::chrono::duration<double>>(threadpoolEndReal - threadpoolStartReal).count()
    << " seconds.\n";
  std::cout << "    CPU: " << (threadpoolEndCPU - threadpoolStartCPU) << " seconds\n\n";

  std::cout << "Thread benchmark complete.\n";
  return 0;
}
